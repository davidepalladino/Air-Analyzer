#ifndef ARDUINO_H
    #include <Arduino.h>
#endif

#ifndef ESP8266WIFI_H
    #include <ESP8266WiFi.h>
#endif

#ifndef EEPROM_H    
    #include <EEPROM.h>
#endif

#ifndef BUTTON_H
    #include <Button.h>
#endif

#ifndef SENSOR_H
    #include <Sensor.h>
#endif

#ifndef SCREEN_H
    #include <Screen.h>
#endif

#ifndef DATABASEMANAGEMENT_H
    #include <DatabaseManagement.h>
#endif

#ifndef CONFIGMAIN_H
    #include "configMain.h"
#endif

void installConfiguration(Button& button, Screen& screen);
void loadConfiguration(Sensor& sensor, Screen& screen, DatabaseManagement& database, String& wifiSSID, String& wifiPassword);
long calulateDelay(long timeStarted, long timeNecessary);

/**
 * @brief This procedure provides the first installation about the system. 
 * @param button Button to provide the user to execute the steps.
 * @param screen Screen to visualize the status of installation with the messages.
 */
void installConfiguration(Button& button, Screen& screen) {
    int8_t resultButton = 0;

    // Room ID
    uint8_t roomID = 0;
    do {
        resultButton = button.checkPress();
        if (resultButton == 1) {
            if (roomID == maxRoomID) {
                roomID = minRoomID;
            } else {
                roomID++;
            }
        }

        screen.setRoomID(roomID);
        screen.showInstallationRoomIDPage(installationRoomIDPageMessages);

        delay(5);                                                               // To prevent "Soft WDT Reset".
    } while (resultButton != -1); 

    // WiFi
    char wifiSSID[sizeWiFiSSID];
    char wifiPassword[sizeWiFiPassword];
    do {
        screen.showInstallationWiFiPage(installationRoomWiFiPageMessages, 1);
        do {
            resultButton = button.checkPress();
            delay(5);                                                           // To prevent "Soft WDT Reset".
        } while (resultButton != 1);

        screen.showInstallationWiFiPage(installationRoomWiFiPageMessages, 2);

        bool result = WiFi.beginWPSConfig(); 
        if (result) {
            if (WiFi.SSID().length() > 0) {
                WiFi.SSID().toCharArray(wifiSSID, sizeWiFiSSID);
                WiFi.psk().toCharArray(wifiPassword, sizeWiFiPassword);

                break;
            } else {
                screen.showInstallationWiFiPage(installationRoomWiFiPageMessages, 3);
                delay(timeoutMessage);
            }
        }

        delay(5);                                                               // To prevent "Soft WDT Reset".
    } while (WiFi.status() != WL_CONNECTED);

    // EEPROM
    EEPROM.begin(sizeEEPROM);
    EEPROM.put(addressVersionEEPROM, versionEEPROM);
    EEPROM.put(addressWiFiSSID, wifiSSID);
    EEPROM.put(addressWiFiPassword, wifiPassword);
    EEPROM.put(addressRoomID, roomID);
    EEPROM.commit();

    screen.showMessagePage(messagePageInstallationCompleteMessages);
    delay(timeoutMessage);
}

/**
 * @brief This procedure provides the configuration about the system. 3
 * @param sensor Sensor to initialize.
 * @param screen Screen to visualize the status of loading with the messages.
 * @param database Database to initialize.
 */
void loadConfiguration(Sensor& sensor, Screen& screen, DatabaseManagement& database, String& wifiSSID, String& wifiPassword) {
    unsigned long timeStartedLoadingMessage;
    float percentageLoadingMessage = 100 / ((sizeof(loadingPageMessages) / sizeof(loadingPageMessages[0])) - 1);
    uint8_t iLoadingMessages = 0;
    
    uint8_t gotRoomID = 0;
    char c_wifiSSID[sizeWiFiSSID];
    char c_wifiPassword[sizeWiFiPassword];
    
    // EEPROM
    timeStartedLoadingMessage = millis();
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * iLoadingMessages++));
    EEPROM.get(addressWiFiSSID, c_wifiSSID);
    EEPROM.get(addressWiFiPassword, c_wifiPassword);
    EEPROM.get(addressRoomID, gotRoomID);
    wifiSSID = c_wifiSSID;
    wifiPassword = c_wifiPassword;
    delay(calulateDelay(timeStartedLoadingMessage, timeoutLoadingMessage));

    // WiFi
    timeStartedLoadingMessage = millis();
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * iLoadingMessages++));
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifiSSID, wifiPassword);
    WiFi.hostname("Air Analyzer-" + String(gotRoomID));
    Serial.print("\nConnection to WiFi..");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.print("\n\tIP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("\tHostname: ");
    Serial.println(WiFi.hostname());
    Serial.print("\tMode: ");
    Serial.println(WiFi.getMode());
    delay(calulateDelay(timeStartedLoadingMessage, timeoutLoadingMessage));

    // Database
    timeStartedLoadingMessage = millis();
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * iLoadingMessages++));
    database.begin();
    database.setRoomID(gotRoomID);
    database.executeQuery(DM_ROOM);
    delay(calulateDelay(timeStartedLoadingMessage, timeoutLoadingMessage));
    
    // Sensor
    timeStartedLoadingMessage = millis();
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * iLoadingMessages++));
    sensor.begin();
    delay(calulateDelay(timeStartedLoadingMessage, timeoutLoadingMessage));

    // Screen
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * iLoadingMessages++));
    screen.setRoomID(gotRoomID);
    screen.setIsConnectedWiFi(true);
    screen.setIsErrorUpdate(false);
}

/**
 * @brief This function provides to calculate the delay based on the past time and the time necessary. Is used for the loading message during the loading page.
 * @param timeStarted Past time, where the timer is started.
 * @param timeNecessary Time necessary, where will be used to calculate the difference.
 * @return "0" if the difference between the past time and the present time is greater than time necessary; else the result of this difference with time necessary.
 */
long calulateDelay(long timeStarted, long timeNecessary) {
    long difference = millis() - timeStarted;
    return (timeNecessary - difference) < 0 ? 0 : (timeNecessary - difference);
}