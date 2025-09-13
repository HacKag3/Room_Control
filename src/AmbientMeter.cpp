#include "AmbientMeter.h"

MyDHT::MyDHT(int SENSOR_PIN, uint8_t SENSOR_TYPE) 
    : DHT(SENSOR_PIN, SENSOR_TYPE) {
  begin();
}

void MyDHT::begin() {
  DHT::begin();
}

void MyDHT::read(float &humidity, float &temperature) {
  humidity = readHumidity();
  temperature = readTemperature();
  
  if (isnan(humidity) || isnan(temperature)) {
    humidity = NULL;
    temperature = NULL;
  }
}

MyDHT11::MyDHT11(int SENSOR_PIN) 
    : MyDHT(SENSOR_PIN, DHT11) {}


AmbientMeter::AmbientMeter() 
    : meter(PIN) {}

void AmbientMeter::begin() {
  meter.begin();
}

void AmbientMeter::measure() {
  meter.read(DATI.humidity, DATI.temperature);
}