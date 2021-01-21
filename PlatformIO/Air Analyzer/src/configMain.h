// Serial Monitor
const uint32_t baudRate =                                           115200;

// WiFi
const uint8_t sizeWiFiSSID =                                        33;
const uint8_t sizeWiFiPassword =                                    64;

// EEPROM
const uint8_t sizeEEPROM =                                          99;
const uint8_t addressVersionEEPROM =                                0;
const uint8_t addressWiFiSSID =                                     1;
const uint8_t addressWiFiPassword =                                 34;
const uint8_t addressRoomID =                                       99;
const uint16_t timeSaveEEPROM =                                     5000;

// Sensor
const uint8_t addressSensor =                                       0x40;
const HDC1080_MeasurementResolution humidityResolution =            HDC1080_RESOLUTION_14BIT;
const HDC1080_MeasurementResolution temperatureResolution =         HDC1080_RESOLUTION_14BIT;

// OLED screen
const uint8_t pinSCL =                                              20;
const uint8_t pinSDA =                                              19;
const uint8_t nLoadingMessages =                                    5;
const uint16_t timeoutLoadingMessage =                              3000;
const String loadingPageTexts[nLoadingMessages] =                   {"GETTING EEPROM", "CONNECTING WIFI", "SETTING DATABASE", "SETTING SENSOR", "SETTING SCREEN"};
const String messagePageSearchingTexts =                            {"WPS CONNECTION..."};
const String messagePageSuccessfulTexts =                           {"NEW CONNECTION IS:"};
const String messagePageErrorTexts[2] =                             {"NO NEW CONNECTION", "REVEALED!"};
const uint16_t timeoutMessage =                                     3000;

// Button
const uint8_t pinButton =                                           D5;
const int16_t timeLongPress =                                       3000;