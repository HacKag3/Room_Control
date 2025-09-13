/**
 * Classe del modulo di misurazione dati ambiente di:
 *  Temperatura
 *  Umidita'
*/
#ifndef AMBIENTMETER_H
#define AMBIENTMETER_H

#include <Arduino.h>
#include <DHT.h>
#include "Dati.h"

/**
 * Personalizzazione della classe generica arduino DHT
 *  si adattano/uniscono le funzioni basi del DHT per il caso specifico della nostra applicazione
 */
class MyDHT : public DHT {
  public:
    MyDHT(int SENSOR_PIN, uint8_t SENSOR_TYPE);
    /**
     * @brief Inizializzazione del DHT
     */
    void begin();
    /**
     * @brief Lettura di umidita' e temperatura
     *
     * @param humidity valore per riferimento alla varibile che contiene il valore dell'umidita (ne modifica quindi il contenuto di quella passata)
     * @param temperature valore per riferimento alla varibile che contiene il valore della temperatura (ne modifica quindi il contenuto di quella passata)
     */
    void read(float& humidity, float& temperature);
};

class MyDHT11 : public MyDHT {
  public:
    MyDHT11(int SENSOR_PIN);
};

/**
 * @brief Misuratore dell'ambiene
 */
class AmbientMeter {
  public:
    static constexpr int PIN = 25;

    AmbientMeter();
    /**
     * @brief Inizializzazione del componente
     */
    void begin();
    /**
     * @brief Esegue misurazione su temperatura e umidita di DATI (classe globale)
     */
    void measure();

  private:
    MyDHT11 meter;
};

#endif // AMBIENTMETER_H