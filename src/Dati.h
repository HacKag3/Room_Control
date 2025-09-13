#ifndef DATI_H
#define DATI_H

#include <Arduino.h>

/**
 * @brief Classe Globale per la gestione dei dati importanti per il sistema
 * (quelli che sono salvati nel database come dati importanti)
 */
class Dati {
  public:
    // di stato del sistema
    bool someoneInRoom;
    float humidity, temperature;
    // di controllo del sistema
    float updateRate;
    int agentStatus;
    bool secureMode;
    bool sleepyhead;

    Dati();

    /**
     * @brief Inizializzazione
     */
    void begin();
    /**
     * @brief Toggle della modalita di sleep (dice se accendere o meno i led del sistema)
     */
    void sleepy();
    /**
     * @brief Toggle di someoneInRoom (ad ora il progetto prevede l'unica persona in ingresso uscita, non piu di uno)
     */
    void crossDetected();
    /**
     * @brief Stampa seriale dei valori
     */
    void print();
    /**
     * @brief Imposta i valori di umidita e temperatura
     *
     * @param h umidita
     * @param t temperatura
     */
    void setAmbient(float h, float t);
    /**
     * @brief Imposta i valori di controllo del sistema
     *
     * @param u updateRate
     * @param a stato del Servo
     * @param s modalita di sicurezza
     */
    void setCtrl(float u, int a, bool s);
    /**
     * @brief Modifica i DATI in nase al JSON
     *
     * @param u updateRate
     */
    void updateFromJSON(String json);
    /**
     * @brief Restituisce i dati di controllo del sistema in formato JSON
     *
     * @return stringa in formato json
     */
    String toJSON_ctrl();
    /**
     * @brief Restituisce i dati di stato della stanza in formato JSON
     *
     * @return stringa in formato json
     */
    String toJSON_roomstatus();
    /**
     * @brief Restituisce i DATI del sistema in formato JSON
     *
     * @return stringa in formato json
     */
    String toJSON();
    /**
     * @brief Restituisce il valore in stringa corrispondente alla chiave del JSON
     *
     * @param key stringa della chiave da cercare nel JSON
     * @param json stringa in formato JSON
     * @return stringa del valore corrispondete a key
     */
    static String getValueJsonOf(String key, String json);

  private:
    /**
     * @brief Restituisce i dati di controllo del sistema per l'implementazione in un oggetto JSON
     *
     * @return stringa parziale JSON dei dati di controllo
     */
    String ctrlList();
    /**
     * @brief Restituisce i dati di stato della stanza per l'implementazione in un oggetto JSON
     *
     * @return stringa parziale JSON dei dati della stanza
     */
    String roomstatusList();
};

/* Globalizzazione della singola istanza Dati per l'intero sistema */
extern Dati DATI;

#endif // DATI_H