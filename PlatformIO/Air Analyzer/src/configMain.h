// Versions
#pragma once

// Versions
const String versionFirmware =                                      "1.3.0";
const uint8_t versionEEPROM =                                       2;

// Serial Monitor
const uint32_t baudRate =                                           115200;

// WiFi
const uint8_t sizeWiFiSSID =                                        33;
const uint8_t sizeWiFiPassword =                                    64;
const uint16_t serverPort =                                         8008;

// EEPROM
const uint8_t sizeEEPROM =                                          105;
const uint8_t addressVersionEEPROM =                                0;
const uint8_t addressWiFiSSID =                                     1;
const uint8_t addressWiFiPassword =                                 34;
const uint8_t addressRoomID =                                       99;
const uint8_t addressUserID =                                       100;
const uint8_t addressTimezone =                                     104;                                                            // This takes two cell for negative number
const uint16_t timeSaveEEPROM =                                     5000;

// Sensor
const uint8_t addressSensor =                                       0x40;
const HDC1080_MeasurementResolution humidityResolution =            HDC1080_RESOLUTION_14BIT;
const HDC1080_MeasurementResolution temperatureResolution =         HDC1080_RESOLUTION_14BIT;

// OLED screen
const uint8_t pinSCL =                                              20;
const uint8_t pinSDA =                                              19;
const uint16_t timeoutLoadingMessage =                              3000;
const uint16_t timeoutMessage =                                     5000;

// Button
const uint8_t pinButton =                                           D5;
const int16_t timeLongPress =                                       3000;