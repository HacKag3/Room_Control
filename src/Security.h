#ifndef SECURITY_H
#define SECURITY_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Dati.h"
#include "secret.h"

/**
 * @brief Classe che gestisce interazione con Bot Telegram per la gestione della sicurezza della stanza
 */
class Security {
  public:
    static constexpr int LED = 33;
    static constexpr int BUTTON = 12;

    Security();
    /**
     * @brief Inizializzazione
     */
    void begin();
    /**
     * @brief Toggle della modalita di sicurezza di DATI
     */
    void toggleSecureMode();
    /**
     * @brief Esegue azione specifica di sicurezza
     */
    void exec();
    /**
     * @brief Controlla se richiesta la modalita di sicurezza da telegram o manuale (Call Of Duty)
     */
    void listenCOD();

  private:
    bool lastState;
    int lastUpdateId;

    /**
     * @brief Spedisce il messaggio specifico al bot telegram
     *
     * @param message messaggio da inviare al bot
     */
    void sendTelegramMessage(const String& message);
    /**
     * @brief Controllo se al bot si ha scritto un messaggio con il comando specificato
     *
     * @param command comando da cercare
     */
    int checkTelegramMessages(const String& command);
};

#endif // SECURITY_H