#include "Security.h"

Security::Security() 
    : lastState(DATI.someoneInRoom), lastUpdateId(0){
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(BUTTON, INPUT_PULLUP);
}

void Security::begin() {
  if (WiFi.status() == WL_CONNECTED) {
    sendTelegramMessage("✅ RoomController Online 👀");
  }
}

void Security::toggleSecureMode() {
  DATI.secureMode = !DATI.secureMode;

  digitalWrite(LED, DATI.secureMode ? HIGH : LOW);
  sendTelegramMessage(DATI.secureMode ? "🔒 Modalità sicurezza ATTIVATA" : "🔓 Modalità sicurezza DISATTIVATA");
}

void Security::listenCOD() {
  if (DATI.secureMode) {
    digitalWrite(LED, DATI.sleepyhead ? LOW : HIGH);
  } else {
    digitalWrite(LED, LOW);
  }

  // controllo fisico
  static bool isPressed = false;
  static unsigned long lastDebounceTime = 0;  // Variabile per debouncing
  unsigned long debounceDelay = 50;           // Ritardo per il debouncing

  if (digitalRead(BUTTON)==LOW && !isPressed && (millis() - lastDebounceTime) > debounceDelay) {        // low = bottone premuto
    isPressed = true;
    lastDebounceTime = millis();
  } else if (digitalRead(BUTTON)==HIGH && isPressed && (millis() - lastDebounceTime) > debounceDelay) { // low = bottone non premuto
    isPressed = false;
    toggleSecureMode();
    lastDebounceTime = millis();
  }

  // controllo dal bot
  static unsigned long lastMessageCheckTime = 0;
  unsigned long messageCheckInterval = 10000;     // controlla tra almeno 10secondi

  if (millis() - lastMessageCheckTime > messageCheckInterval) {
    int count  = checkTelegramMessages("/secureMode");
    if (count%2!=0) {
      toggleSecureMode();
    }
    lastMessageCheckTime = millis();
  }
}

void Security::exec() {
  if (!DATI.secureMode) return;
  if (DATI.someoneInRoom == lastState) return;  // Solo se c'è un cambio di stato
  
  sendTelegramMessage(DATI.someoneInRoom ? "⚠️ ATTENZIONE: Qualcuno è entrato nella stanza!" : "✅ Nessuno più presente nella stanza.");

  lastState = DATI.someoneInRoom;
}

void Security::sendTelegramMessage(const String &message) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  String url = "https://api.telegram.org/bot" + String(BOT_TOKEN) +
               "/sendMessage?chat_id=" + String(CHAT_ID) + 
               "&text=" + message;
  http.begin(url);
  http.GET();
  http.end();
}

int Security::checkTelegramMessages(const String &command) {
  int count = 0;
  if (WiFi.status() != WL_CONNECTED) return count;

  HTTPClient http;
  String url = "https://api.telegram.org/bot" + String(BOT_TOKEN) + "/getUpdates";
  if (lastUpdateId > 0) {
    url += "?offset=" + String(lastUpdateId + 1);
  }
  http.begin(url);
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    String payload = http.getString();

    int startI = 0;
    while ((startI = payload.indexOf("\"update_id\":", startI)) != -1) {
      startI += 12;
      int endI = payload.indexOf(",", startI);
      if (endI == -1) { 
        endI = payload.indexOf("}", startI); // Se non c'è virgola, trova la fine della stringa
      }
      String updateIdStr = payload.substring(startI, endI);
      long updateId = updateIdStr.toInt();
      if (updateId > lastUpdateId) {
        lastUpdateId = updateId;
      }

      int textStartI = payload.indexOf("\"text\":\"", endI);
      if (textStartI != -1) {
        textStartI += 8;
        int textEndI = payload.indexOf("\"", textStartI);
        String commandText = payload.substring(textStartI, textEndI);
        if (commandText.indexOf(command) >= 0) {
          count++;
        }
      }
      startI = endI;
    }
  }
  http.end();

  return count;
}