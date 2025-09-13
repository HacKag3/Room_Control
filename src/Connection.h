#ifndef CONNECTION_H
#define CONNECTION_H

#include <Arduino.h>
#include <WiFi.h>
#include "Dati.h"
#include "secret.h"

/**
 * @brief Classe che gestisce la connessione alla rete
 */
class Connection {
  public:
    static constexpr int LED = 32;

    Connection();
    /**
     * @brief Inizializzazione
     */
    void begin();
    /**
     * @brief Controllo dello stato della connettivita' del sistema con tentaivo di riconnessione in caso di disconnessione
     */
    void checkStatus();
  
  private:
    /**
     * @brief Tenta di connettersi alla rete
     */
    void tryConnection();
};

#endif // CONNECTION_H