#ifndef CROSSDETECTOR_H
#define CROSSDETECTOR_H

#include <Arduino.h>

/**
 * @brief Classe che controllare il varco di una soglia
 */
class CrossDetector {
  public:
    static constexpr int ECHO = 27;
    static constexpr int TRIG = 26;
    static constexpr int TRESHOLD = 4;

    CrossDetector();
    /**
     * @brief Esegue una misurazione della distanza attuale
     */
    void ping();
    /**
     * @brief Inizializzazione
     */
    void begin();
    /**
     * @brief Verifica se si ha un passaggio della soglia
     */
    bool detect();

  private:
    static constexpr long PULSE_TIMEOUT = 150000L;
    long duration;  // Durata del segnale
    long distance; // Distanza calcolata
    int startingDistance, lastDistance;

    
    /**
     * @brief Conversione da durata di risposta della misurazione a distanza misurata
     *
     * @param duration durata in millisecondi
     */
    long MicroSecToCm(long duration);
};

#endif // CROSSDETECTOR_H