#include <Arduino.h>

#include <ESP8266WiFi.h>

#include <Sensor.h>
#include <Screen.h>
#include <DatabaseManagement.h>

#include "configMain.h"
#include "../credentialsWiFi.h"

Sensor sensor(pinSensor, typeSensor);
Screen screen(sensor, pinSCL, pinSDA);
DatabaseManagement database(sensor, true, 5);

void setup() {
  Serial.begin(baudRate);
  Serial.flush();
  
  // Serial debug
  WiFi.begin(wifiSSID, wifiPassword);
  WiFi.hostname(hostname);
  Serial.print("\nConnessione WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n\tIndirizzo IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("\tHostname: ");
  Serial.println(WiFi.hostname());

  sensor.begin();
  screen.begin();
  database.begin();
}

void loop() {
  sensor.check();
}