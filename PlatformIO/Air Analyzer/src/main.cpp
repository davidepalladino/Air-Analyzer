#include <Arduino.h>

#include <ESP8266WiFi.h>

#include <Sensor.h>
#include <Screen.h>
#include <DatabaseManagement.h>

#include "configMain.h"
#include "../configWiFi.h"

Sensor sensor(pinSensor, typeSensor);
Screen screen(sensor, pinSCL, pinSDA);
DatabaseManagement database(sensor, databaseTimezone, databaseMinutesUpdate);

void setup() {
  Serial.begin(baudRate);
  
  WiFi.begin(wifiSSID, wifiPassword);
  WiFi.hostname(nameDevice + "-" + databaseRoomName);
  Serial.print("\nConnection to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n\tIP address: ");
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