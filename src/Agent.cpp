#include "Agent.h"

Agent::Agent() {}

void Agent::begin() {
  this->attach(PIN);    // Associazione del PIN per il segnale
  exec();               // Set dello stato iniziale
}

void Agent::exec() {
  if (DATI.someoneInRoom && DATI.agentStatus!=1) {          // Caso di accensione
    lightTurnOn();
  } else if (!DATI.someoneInRoom && DATI.agentStatus!=0) {  // Caso di spegnimento
    lightTurnOff();
  } else if (DATI.agentStatus!=0 && DATI.agentStatus!=1) {  // Caso ignoto o di stallo
    moveTo(IDLE);
  }
}

void Agent::lightTurnOn() {
  if (DATI.agentStatus == 1) return;
  DATI.agentStatus = 1;
  smoothMoveTo(END_ON, 5);
  smoothMoveTo(IDLE, 5);
}

void Agent::lightTurnOff() {
  if (DATI.agentStatus == 0) return;
  DATI.agentStatus = 0;
  smoothMoveTo(END_OFF, 5);
  smoothMoveTo(IDLE, 5);
}

void Agent::moveTo(int angle) {
  this->write(angle);
  actualAngle = angle;
}

void Agent::smoothMoveTo(int angle, int delayTime) {
  int step = (angle > actualAngle) ? 1 : -1;

  for (int pos = actualAngle; pos != angle + step; pos += step) {
    this->write(pos);
    delay(delayTime);
  }
  
  actualAngle = angle;
}