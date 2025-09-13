#ifndef AGENT_H
#define AGENT_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include "Dati.h"

/**
 * Classe del componente che svolge l'azione fisica
 *  (accensione/spegnimento interruttore luce)
 * Modulo: Motor Servo
 */
class Agent : public Servo {
  public:
    static constexpr int PIN = 13;
    static constexpr int START = 90;
    static constexpr int IDLE = START+0;
    static constexpr int END_ON = START+45;
    static constexpr int END_OFF = START-45;

    Agent();
    /**
     * @brief Inizializzazione del componente
     */
    void begin();
    /**
     * @brief Esegue azione di accensione o spegnimento in base allo stato dei DATI (classe globale)
     */
    void exec();
  
  private:
    int actualAngle;
    
    /**
     * @brief Movimento veloce verso l'angolo desiderato 
     *
     * @param angle angolo di destinazione
     */
    void moveTo(int angle);
    /**
     * @brief Movimento graduale  verso l'angolo desiderato 
     *
     * @param angle angolo di destinazione
     * @param delayTime tempo di atessa tra un grado ed un altro
     */
    void smoothMoveTo(int angle, int delayTime);
    /**
     * @brief Movimento di accensione luce (modifcare in base al proprio genere di interrutore)
     */
    void lightTurnOn();
    /**
     * @brief Movimento di spegnimento luce (modifcare in base al proprio genere di interrutore)
     */
    void lightTurnOff();
};

#endif // AGENT_H