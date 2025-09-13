#include "src/Dati.h"
#include "src/Connection.h"
#include "src/AmbientMeter.h"
#include "src/CrossDetector.h"
#include "src/Agent.h"
#include "src/DataExchanger.h"
#include "src/Security.h"

constexpr int BUTTON_SLEEP = 14;
constexpr unsigned long DEBOUNCE_DELAY = 50;

unsigned long lastUpdateRateCheckTime = 0, lastUpdateTime = 0;
unsigned long lastDebounceTime = 0;
bool buttonPressed = false;

Connection net;
AmbientMeter ambient;
CrossDetector crosser;
Agent agent;
Security cop;
DataExchanger dataExchanger;

/*
 * Inizializzazione dei moduli e Calibrazione Iniziale 
 */
void setup() {
  pinMode(BUTTON_SLEEP, INPUT_PULLUP);
  Serial.begin(115200);
  
  DATI.begin();
  net.begin();
  ambient.begin();
  crosser.begin();
  agent.begin();
  cop.begin();
  dataExchanger.begin();
  
  unsigned long now = millis();
  lastUpdateTime = now;
  lastUpdateRateCheckTime = now;
}

// Controllo varco soglia ed esecuzione delle corrispettive azioni in caso positivo
void handleCrossDetection() {
  if (!crosser.detect()) return;

  DATI.crossDetected();

  if (DATI.secureMode) {
    cop.exec();
  } else {
    agent.exec();
  }

  delay(10);
  dataExchanger.updateCtrl();
}
// Controllo per prelevare periodicamente l'updateRate dalla base di dati
void handleUpdateRate() {
  unsigned long now = millis();
  int rate = DATI.updateRate > 1 ? DATI.updateRate : 1;  // almeno 1 minuto

  if (now - lastUpdateRateCheckTime >= rate * 60000UL) {
    dataExchanger.catchUpdateRate();
    lastUpdateRateCheckTime = now;
  }
}
// Controllo per efettuare l'update dello stato della stanza
void handlePeriodicUpdate() {
  unsigned long now = millis();
  if (now - lastUpdateTime >= DATI.updateRate * 60000UL) {
    ambient.measure();
    dataExchanger.addRoomStatus();
    lastUpdateTime = now;
  }
}
// Controllo del bottone di spegnimento led
void handleSleepyButton() {
  unsigned long now = millis();
  bool buttonIsNowPress = digitalRead(BUTTON_SLEEP) == LOW;

  if ((now - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (buttonIsNowPress && !buttonPressed) {
      buttonPressed = true;
    } else if (!buttonIsNowPress && buttonPressed) {
      buttonPressed = false;
      DATI.sleepy();
    }
    lastDebounceTime = now;
  }
}

/* Loop di esecuzione principale */
void loop() {
  net.checkStatus();
  cop.listenCOD();

  handleCrossDetection();
  handleUpdateRate();
  handlePeriodicUpdate();
  handleSleepyButton();
}