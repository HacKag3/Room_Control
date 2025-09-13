const supabaseUrl = "https://csdairgeuzivpmzvpcym.supabase.co";
const supabaseKey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImNzZGFpcmdldXppdnBtenZwY3ltIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NDIzMTE5MzcsImV4cCI6MjA1Nzg4NzkzN30.cVrwAeoDXSilvf1hlq9x8rxXemt7xbLzSEA83KGaKxI";
const supabase = window.supabase.createClient(supabaseUrl, supabaseKey);

// Funzione per formattare una data come 'YYYY-MM-DD'
function formatDate(date) {
    if (!(date instanceof Date)) return null;
    const year = date.getFullYear();
    const month = (date.getMonth() + 1).toString().padStart(2, '0');
    const day = date.getDate().toString().padStart(2, '0');
    return `${year}-${month}-${day}`;
}

document.addEventListener("DOMContentLoaded", function () {
    // Creazione Grafico (vuoto)
    const ctx = document.getElementById('T_H_Chart0').getContext('2d'); // Contesto 2d del canvas su cui disegnare il grafico
    let chart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: [], // etichette asse X
            datasets: [
                {
                    label: "Temperatura (°C)",
                    data: [], // valori asse Y
                    borderColor: "blue", // Colore della linea
                    backgroundColor: "rgba(0, 0, 255, 0.2)", // Colore di riempimento
                    borderWidth: 1, // Spessore linea
                    fill: true, // Riempire l'area sotto la linea
                    pointRadius: 1, // Dimensione dei punti
                    yAxisID: 'y1',
                },
                {
                    label: "Umidità (%)",
                    data: [], // valori asse Y
                    borderColor: "red", // Colore della linea
                    backgroundColor: "rgba(255, 0, 0, 0.2)", // Colore di riempimento
                    borderWidth: 1, // Spessore linea
                    fill: true, // Riempire l'area sotto la linea
                    pointRadius: 1, // Dimensione dei punti
                    yAxisID: 'y2',
                },
            ]
        },
        options: {
            responsive: true, // Adatta il grafico alle dimensioni del contenitore
            maintainAspectRatio: false, // Permette di ridimensionare liberamente
            plugins: {
                legend: { display: true, position: "top" }, // Mostra la legenda
                tooltip: { enabled: true }, // Tooltip al passaggio del mouse
            },
            scales: {
                x: {
                    title: { display: true, text: "h" },
                    type: 'time',
                    time: {
                        unit: 'minute',
                        unitStepSize: 1,
                        tooltipFormat: 'HH:mm', // Formato del tooltip(etichetta del punto nel grafico)
                        displayFormats: {
                            minute: 'HH:mm' // Formato delle etichette nell'asse X
                        }
                    },
                    ticks: {    // Impostazioni per le etichette nell'asse
                        source: 'data',
                        autoSkip: false,
                        stepSize: 1,
                        maxRotation: 0,
                        minRotation: 0,
                        callback: function(value, index, values) {
                            let data = new Date(value);
                            if (data.getMinutes() === 0 || data.getMinutes() === 15 || data.getMinutes() === 30 || data.getMinutes() === 45)
                                return data.toLocaleTimeString([], {hour: '2-digit', minute: '2-digit'});
                            else
                                return "";
                        }
                    }
                },
                y1: {
                    title: { display: true, text: "°C" },
                    beginAtZero: false,
                    position: 'left', // Posizione asse Y per temperatura
                },
                y2: {
                    title: { display: true, text: "%" },
                    beginAtZero: false,
                    position: 'right', // Posizione asse Y per umidità
                },
            }
        }
    });

    // Funzione per aggiornare i dati
    async function fetchDataOf(day) {
        day = formatDate(new Date(day));
        console.log("Fetching data for day: ", day);

        const sqlQuery = `
        SELECT (giorno AT TIME ZONE 'UTC' AT TIME ZONE 'Europe/Rome') AS giorno_locale,
                temperature,
                humidity
        FROM roomstatus
        WHERE (giorno AT TIME ZONE 'UTC' AT TIME ZONE 'Europe/Rome')::date = '${day}'
        `;

        let { data, error } = await supabase
            .rpc('exec_query', { sql_query: sqlQuery });
        if (error) { console.error("Errore nel recupero dati:", error); return; }

        console.log("Data:", data);

        const tDataPoint = data.map(r => ({
            x: new Date(r.giorno_locale),
            y: r.temperature
        }));
        const hDataPoint = data.map(r => ({
            x: new Date(r.giorno_locale),
            y: r.humidity
        }));

        chart.data.datasets[0].data = tDataPoint;
        chart.data.datasets[1].data = hDataPoint;

        chart.update();

        /*if (data.length > 0) {
            document.getElementById("currentHumi").innerText = humidity[humidity.length - 1].toFixed(1)+" °C"; // Umidità attuale
            document.getElementById("currentTemp").innerText = temperature[temperature.length - 1].toFixed(1)+" %"; // Temperatura attuale con 1 cifra decimale
        }*/
    }

    // Gestore della data
    const today = new Date();
    const sevenDaysAgo = new Date(today)
    sevenDaysAgo.setDate(today.getDate() - 7);
    // Imposta i valori minimi e massimi per l'input date
    const dateSelector = document.getElementById('dateSelector');
    dateSelector.value = formatDate(today);                      // Imposta la data di
    dateSelector.setAttribute('min', formatDate(sevenDaysAgo));  // Imposta la data minima
    dateSelector.setAttribute('max', formatDate(today));          // Imposta la data massima

    const loadDataBtn = document.getElementById('loadDataBtn');
    loadDataBtn.addEventListener('click', function () {
        const selectedDate = dateSelector.value;
        if (selectedDate) {
            fetchDataOf(selectedDate);
        } else {
            console.error("Seleziona una data valida");
        }
    });

    fetchDataOf(formatDate(today));
    // Aggiornamento automatico ogni 10 secondi
    function autoLoadData() {
        const selectedDate = dateSelector.value || formatDate(today);  // Usa la data selezionata o quella di oggi
        fetchDataOf(selectedDate);  // Carica i dati per quella data
    }
    setInterval(autoLoadData, 10000);
});