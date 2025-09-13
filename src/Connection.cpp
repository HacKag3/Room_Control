#include "Connection.h"

Connection::Connection() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void Connection::begin() {
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false); // Disattiva la modalità di risparmio energetico
  
  Serial.print("🔗 Connessione a WiFi...");
  tryConnection();
}

void Connection::checkStatus() {
  static unsigned long lastReconnectAttempt = 0;
  static unsigned long reconnectInterval  = 60000;
  unsigned long currentMillis = millis();

  if (WiFi.status() == WL_CONNECTED) {
    if (DATI.sleepyhead) {
      digitalWrite(LED, LOW);
    } else {
      digitalWrite(LED, HIGH);
    }
    lastReconnectAttempt = 0;
    reconnectInterval = 60000;
  } else if (lastReconnectAttempt == 0 || currentMillis - lastReconnectAttempt >= reconnectInterval) {
    Serial.print("⚠️ Connessione Persa. Riconnessione...");
    if (WiFi.status() == WL_DISCONNECTED) {
      WiFi.disconnect();
      delay(500);
    }
    tryConnection();
    lastReconnectAttempt = currentMillis;
    reconnectInterval = min(reconnectInterval * 2, 900000UL); // Aumenta il tempo di ritentativo (backoff esponenziale con max 15 min)
  }
}

void Connection::tryConnection() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int max_attempts = 20; // Numero massimo di tentativi (circa 10 secondi)
  unsigned long connectionStartMillis = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - connectionStartMillis < 10000 && max_attempts > 0) {
    digitalWrite(LED, HIGH);
    delay(250);
    digitalWrite(LED, LOW);
    delay(250);
    Serial.print(".");
    max_attempts--;
  }
  Serial.println();
    
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("✅ Connesso ad internet!");
    Serial.print("📡 IP Address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(LED, HIGH);
  } else {
    Serial.println("❌ ERRORE: Impossibile connettersi al WiFi!");
    digitalWrite(LED, LOW);
  }
}
