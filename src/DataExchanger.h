#ifndef DATAEXCHANGER_H
#define DATAEXCHANGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>  // Libreria per parsing JSON
#include "Dati.h"
#include "secret.h"

/**
 * @brief Classe che effettua lo scambio di dati con la base di dati e DATI (classe globale)
 */
class DataExchanger {
  public:
    DataExchanger();

    /**
     * @brief Inizializzazione
     */
    void begin();
    /**
     * @brief prelievo dei dati di controllo dal DB
     */
    void catchCtrl();
    /**
     * @brief prelievo del solo rate per l'update dal DB
     */
    void catchUpdateRate();
    /**
     * @brief modifica l'ultimo inserimento sul DB con nuovi dati rispetto ai dati di controllo
     */
    void updateCtrl();
    /**
     * @brief salva nuovi dati sul DB sullo stato della stanza
     */
    void addRoomStatus();
    /**
     * @brief modifica l'ultimo inserimento sul DB con nuovi dati rispetto allo stato della stanza
     */
    void updateRoomStatus();
    
  private:
    /**
     * @brief Aggiunge i dati contenuti nel json sulla tabella specificata
     * 
     * @param table     stringa contenente il nome della tabella
     * @param dataJSON  stringa contenente il JSON dei dati da inserire
     */
    void ADD(String table, String dataJSON);
    /**
     * @brief Preleva i dati richiesta dalla tabella specificata
     * 
     * @param table   stringa contenente il nome della tabella
     * @param select  stringa contenente il filtro di selezione
     */
    String SELECT(String table, String select="select=*");
    /**
     * @brief Aggiorna i dati della tabella specificata con il filtro dato
     * 
     * @param table   stringa contenente il nome della tabella
     * @param where   stringa contenente il filtro di update
     * @param dataJSON  stringa contenente il JSON dei dati da inserire
     */
    void UPDATE(String table, String where, String dataJSON);
    /**
     * @brief Elimina i dati della tabella specificata con il filtro dato
     * 
     * @param table   stringa contenente il nome della tabella
     * @param where   stringa contenente il filtro di selezione
     */
    void DELETE(String table, String where);
    /**
     * @brief Esegue query SQL specifica
     * 
     * @param query   stringa contenente la query SQL
     */
    String EXEC_QUERY_SQL(String query);
};

#endif // DATAEXCHANGER_H