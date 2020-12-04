#include <Arduino.h>

#include <Sensor.h>
#include <Screen.h>
#include <DatabaseManagement.h>

#include "configMain.h"

Sensor sensor(pinSensor, typeSensor);
Screen screen(sensor, pinSCL, pinSDA);
DatabaseManagement database(sensor);

void setup() {
  Serial.begin(baudRate);
  sensor.begin();
  screen.begin();
}

void loop() {
  sensor.check();
}