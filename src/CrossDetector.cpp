#include "CrossDetector.h"

CrossDetector::CrossDetector() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(TRIG, LOW);
  distance = 999;
}

void CrossDetector::begin() {
  ping();
  startingDistance = distance;
  lastDistance = distance;
}

void CrossDetector::ping() {
  static unsigned long lastPing = 0;
  static constexpr unsigned long PING_INTERVAL = 150;
  unsigned long now = millis();
  if (now-lastPing < PING_INTERVAL) return; 
  // crea distanziamento tra ping cosi che evita collisioni tra echo
  // evita echi fantasma e riduce rumore
  lastPing = now;
  duration = 0;

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  
  duration = pulseIn(ECHO, HIGH, PULSE_TIMEOUT);
  distance = (duration > 0) ? MicroSecToCm(duration) : 999;
}

/**
 * Velocità del suono = v = 340m/s = 0.0343 cm/μs
 * La formula della distanza è: d = t * v / 2
 * 2/v = 58.82 ≈ 58 (approssimato per evitare errori interi)
 */
long CrossDetector::MicroSecToCm(long duration) {
  return duration/58;
}

bool CrossDetector::detect() {
  bool detected = false;
  ping();
  if (abs(distance - startingDistance) > TRESHOLD) {        // Se si ha misurato una nuova distanza rispetto l'iniziale (considerando lo scarto di misura)
    if (abs(lastDistance - startingDistance) <= TRESHOLD) { // Se l'ultima misurazione era l'iniziale (considerando lo scarto) (tiene in considerazione che qualcosa possa restare sulla soglia e quindi aggiorna solo quando la varca del tutto)
      detected = true;  
    }
  }
  lastDistance = distance;
  return detected;
}
