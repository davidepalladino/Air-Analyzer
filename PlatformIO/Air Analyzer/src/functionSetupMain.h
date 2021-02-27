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

#define MIN_ROOM_ID 1
#define MAX_ROOM_ID 9
#define MIN_TIMEZONE -12
#define MAX_TIMEZONE 14

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
    uint8_t roomID = MIN_ROOM_ID;
    do {
        resultButton = button.checkPress();
        if (resultButton == 1) {
            if (roomID == MAX_ROOM_ID) {
                roomID = MIN_ROOM_ID;
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


    // Timezone
    resultButton = 0;
    int8_t timezone = MIN_TIMEZONE;
    do {        
        resultButton = button.checkPress();
        if (resultButton == 1) {
            if (timezone == MAX_TIMEZONE) {
                timezone = MIN_TIMEZONE;
            } else {
                timezone++;
            }
        }
        screen.showInstallationTimezonePage(installationTimezonePageMessages, timezone); 

        delay(5);                                                               // To prevent "Soft WDT Reset".
    } while (resultButton != -1);

    // EEPROM
    EEPROM.put(addressVersionEEPROM, versionEEPROM);
    EEPROM.put(addressWiFiSSID, wifiSSID);
    EEPROM.put(addressWiFiPassword, wifiPassword);
    EEPROM.put(addressRoomID, roomID);
    EEPROM.put(addressTimezone, timezone);
    EEPROM.commit();
}

/**
 * @brief This procedure provides the upgrade to version 2 about the system. 
 * @param screen Screen to visualize the status of upgrade with the messages.
 */
void upgradeConfigurationToVersionTwo(Screen& screen) {
    bool completed = false;
    uint8_t requestCode = 0;
    uint32_t userID = 0;
    uint8_t roomID = 0;

    Serial.print("\nConnection to WiFi..");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.print("\n\tIP address: ");
    Serial.println(WiFi.localIP());

    screen.showUpgradeConfigurationToVersionTwoPage(upgradeConfigurationToVersionTwoMessages, WiFi.localIP().toString());

    WiFiServer server(serverPort);
    server.begin();

    while (!completed) {
        WiFiClient client = server.available();

        if (client) {
            Serial.print("Connected IP: "); Serial.println(client.remoteIP());

            while (client.connected()) {
                /* Waiting the request code from the application. */
                while (client.available() != 0) {
                    requestCode = client.read();

                    Serial.print("\tRequest code: ");
                    Serial.println(requestCode);

                    break;
                }
                
                delay(5);                                                       // To prevent "Soft WDT Reset".

                switch (requestCode) {
                    // User ID from application
                    case 1:
                        /* Informing the application that the request is been accept. */
                        client.write((int) requestCode);
                        requestCode = 0;

                        /* Waiting and getting the user ID from the application. */
                        while (client.available() == 0);
                        userID = client.readString().toInt();

                        /* Storing the user ID on EEPROM. */
                        EEPROM.put(addressUserID, userID);
                        EEPROM.commit();

                        Serial.print("\tUser ID: "); Serial.println(userID);
                        
                        break;

                    // Room ID to application
                    case 2:
                        requestCode = 0;

                        /* Getting room ID and sending to the application. */
                        client.flush();
                        EEPROM.get(addressRoomID, roomID);
                        client.write((String(roomID) + '\n').c_str());

                        /* Beacuse the configuration is complete, setup the EEPROM with the new informations. */
                        EEPROM.put(addressVersionEEPROM, versionEEPROM);
                        EEPROM.put(addressUserID, userID);
                        EEPROM.commit();

                        completed = true;

                        break;
                }
            }

            client.stop();
            Serial.println("Disconnected");
        }
    }
}

/**
 * @brief This procedure provides the configuration about the system.
 * @param sensor Sensor to initialize.
 * @param screen Screen to visualize the status of loading with the messages.
 * @param database Database to initialize.
 */
void loadConfiguration(Sensor& sensor, Screen& screen, DatabaseManagement& database, String& wifiSSID, String& wifiPassword) {
    unsigned long timeStartedLoadingMessage;
    float percentageLoadingMessage = 100 / ((sizeof(loadingPageMessages) / sizeof(loadingPageMessages[0])) - 1);
    uint8_t iLoadingMessages = 0;
    
    int8_t timezone = MIN_TIMEZONE;
    uint8_t roomID = MIN_ROOM_ID;
    uint32_t userID = 0;
    char c_wifiSSID[sizeWiFiSSID];
    char c_wifiPassword[sizeWiFiPassword];
    
    // EEPROM
    timeStartedLoadingMessage = millis();
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * iLoadingMessages++));
    EEPROM.get(addressWiFiSSID, c_wifiSSID);
    EEPROM.get(addressWiFiPassword, c_wifiPassword);
    EEPROM.get(addressTimezone, timezone);
    EEPROM.get(addressRoomID, roomID);
    EEPROM.get(addressUserID, userID);
    wifiSSID = c_wifiSSID;
    wifiPassword = c_wifiPassword;
    delay(calulateDelay(timeStartedLoadingMessage, timeoutLoadingMessage));

    // WiFi
    timeStartedLoadingMessage = millis();
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * iLoadingMessages++));
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifiSSID, wifiPassword);
    WiFi.hostname("Air Analyzer-" + String(roomID));
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
    database.setRoomID(roomID);
    database.setUserID(userID);
    database.begin(timezone, databaseMinutesUpdate);
    database.executeQuery(DM_ROOM);
    delay(calulateDelay(timeStartedLoadingMessage, timeoutLoadingMessage));
    
    // Sensor
    timeStartedLoadingMessage = millis();
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * iLoadingMessages++));
    sensor.begin();
    delay(calulateDelay(timeStartedLoadingMessage, timeoutLoadingMessage));

    // Screen
    screen.showLoadingPage(loadingPageMessages[iLoadingMessages], (percentageLoadingMessage * iLoadingMessages++));
    screen.setRoomID(roomID);
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