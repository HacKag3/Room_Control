#include "DataExchanger.h"

DataExchanger::DataExchanger() {}

void DataExchanger::begin() {
  catchCtrl();
}

void DataExchanger::catchCtrl() {
  if (WiFi.status() != WL_CONNECTED) return;

  String result = SELECT("ctrl", "id=eq.1");
  DATI.updateFromJSON(result);
}

void DataExchanger::updateCtrl() {
  if (WiFi.status() != WL_CONNECTED) return;

  String dataJSON = DATI.toJSON_ctrl();
  UPDATE("ctrl", "id=eq.1", dataJSON);
}

void DataExchanger::catchUpdateRate() {
  if (WiFi.status() != WL_CONNECTED) return;

  String result = SELECT("ctrl", "id=eq.1");
  DATI.updateRate = Dati::getValueJsonOf("updaterate", result).toFloat();
}

void DataExchanger::addRoomStatus() {
  if (WiFi.status() != WL_CONNECTED) return;

  ADD("roomstatus", DATI.toJSON_roomstatus());
}

void DataExchanger::updateRoomStatus() {
  if (WiFi.status() != WL_CONNECTED) return;

  String queryResult = EXEC_QUERY_SQL("SELECT id FROM roomStatus ORDER BY id DESC LIMIT 1");
  int id = Dati::getValueJsonOf("id", queryResult).toInt();
  if (id <= 0) return;

  UPDATE("roomstatus", "id=eq." + String(id), DATI.toJSON_roomstatus());
}

/**
 * Funzioni per l'interazione con il database
 */

String DataExchanger::SELECT(String table, String condition) {
  if (WiFi.status() != WL_CONNECTED) return "";

  String getUrl = String(DB_URL_API_REST) + "/" + table + "?" + condition;
  HTTPClient http;
  http.begin(getUrl);
  http.addHeader("apikey", DB_APIKEY);
  http.addHeader("Authorization", "Bearer "+String(DB_APIKEY));

  int httpResponseCode = http.GET();
  String response = httpResponseCode>0 ? http.getString() : "";
  http.end();
  return response;
}

void DataExchanger::ADD(String table, String dataJSON) {
  if (WiFi.status() != WL_CONNECTED) return;

  String addUrl = String(DB_URL_API_REST) + "/" + table;
  HTTPClient http;
  http.begin(addUrl);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("apikey", DB_APIKEY);
  http.addHeader("Authorization", "Bearer "+String(DB_APIKEY));

  http.POST(dataJSON);
  http.end();
}

void DataExchanger::UPDATE(String table, String where, String dataJSON) {
  if (WiFi.status() != WL_CONNECTED) return;
  String updateUrl = String(DB_URL_API_REST) + "/" + table + "?" + where;

  HTTPClient http;
  http.begin(updateUrl);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("apikey", DB_APIKEY);
  http.addHeader("Authorization", "Bearer "+String(DB_APIKEY));

  http.PATCH(dataJSON);
  http.end();
}

void DataExchanger::DELETE(String table, String where) {
  if (WiFi.status() != WL_CONNECTED) return;
  
  String deleteUrl = String(DB_URL_API_REST) + "/" + table + "?" + where;
  HTTPClient http;
  http.begin(deleteUrl);
  http.sendRequest("DELETE");
  http.end();
}

String DataExchanger::EXEC_QUERY_SQL(String query) {
  if (WiFi.status() != WL_CONNECTED) return "";

  String queryUrl = String(DB_URL_API_REST)+"/rpc/exec_query";
  HTTPClient http;
  http.begin(queryUrl);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("apikey", DB_APIKEY);
  http.addHeader("Authorization", "Bearer "+String(DB_APIKEY));

  int httpResponseCode = http.POST("{\"sql_query\": \""+query+"\"}");
  String response = httpResponseCode > 0 ? http.getString() : "";

  http.end();
  return response;
}