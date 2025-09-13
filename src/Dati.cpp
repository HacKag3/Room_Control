#include "Dati.h"

Dati DATI; // Inizializzazione della classe Dati

Dati::Dati() {}

void Dati::begin() {
  someoneInRoom = true; // Si presume che all'avvio ci sia qualcuno
  humidity = 0.0;
  temperature = 0.0;
  updateRate = 1.0; // Aggiornamento base ad ogni minuto
  agentStatus = 3;
  secureMode = false;
  sleepyhead = false;
}

void Dati::print() {
  Serial.print(someoneInRoom);
  Serial.print(" ");
  Serial.print(updateRate);
  Serial.print(" ");
  Serial.print(agentStatus);
  Serial.print(" ");
  Serial.print(secureMode);
  Serial.println();
}

void Dati::sleepy() {
  sleepyhead = !sleepyhead;
}

void Dati::crossDetected() {
  someoneInRoom = !someoneInRoom;
}

void Dati::setAmbient(float h, float t) {
  humidity = h;
  temperature = t;
}

void Dati::setCtrl(float u, int a, bool s) {
  updateRate = u;
  agentStatus = a;
  secureMode = s;
}

void Dati::updateFromJSON(String json) {
  String value = getValueJsonOf("updaterate", json);
  if (value != "") updateRate = value.toFloat();
  value = getValueJsonOf("agentstatus", json);
  if (value != "") agentStatus = value.toInt();
  value = getValueJsonOf("securemode", json);
  if (value != "") secureMode = value.toInt();
  value = getValueJsonOf("someoneinroom", json);
  if (value != "") someoneInRoom = value.toInt();
  value = getValueJsonOf("humidity", json);
  if (value != "") humidity = value.toFloat();
  value = getValueJsonOf("temperature", json);
  if (value != "") temperature = value.toFloat();
}

String Dati::ctrlList() {
  String json = "\"updaterate\":" + String(updateRate) + ",";
  json += "\"agentstatus\":" + String(agentStatus) + ",";
  json += "\"securemode\":" + String(secureMode);
  return json;
}

String Dati::toJSON_ctrl() {
  return "{"+ ctrlList() +"}";
}

String Dati::roomstatusList() {
  String json = "\"someoneinroom\":" + String(someoneInRoom) + ",";
  json += "\"humidity\":" + String(humidity) + ",";
  json += "\"temperature\":" + String(temperature);
  return json;
}

String Dati::toJSON_roomstatus() {
  return "{"+ roomstatusList() +"}";
}

String Dati::toJSON() {
  return "{"+ ctrlList() +","+ roomstatusList() +"}";
}

String Dati::getValueJsonOf(String key, String json) {
  int keyIndex = json.indexOf("\"" + key + "\"");
  if (keyIndex == -1) return "";

  int colonIndex = json.indexOf(":", keyIndex);
  if (colonIndex == -1) return "";

  int startI = colonIndex+1; // indice del valore dopo i ":"
  int endI = json.indexOf(",", startI);
  if (endI == -1) { // Se non trova la virgola, cerca la chiusura
    endI = json.indexOf("}", startI);
  }
  if (endI == -1) return "";  // Se non trova nemmeno "}", errore

  String value = json.substring(startI, endI);
  value.trim();
  value.replace("\"", "");
  return value;
}
