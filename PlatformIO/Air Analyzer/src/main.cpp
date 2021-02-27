#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <EEPROM.h>

#include <Button.h>
#include <Sensor.h>
#include <Screen.h>
#include <DatabaseManagement.h>

#include "configMain.h"
#include "functionSetupMain.h"

Button button(pinButton, B_PULLUP, timeLongPress);
Sensor sensor(addressSensor, humidityResolution, temperatureResolution);
Screen screen(sensor, pinSCL, pinSDA);
DatabaseManagement database(sensor, databaseIP);

String wifiSSID;
String wifiPassword;
int8_t resultButton = 0;
unsigned long timeStartedEEPROM = 0;
bool errorSavingDatabase = false;

void setup() {
  Serial.begin(baudRate);
  screen.begin();

  /* Checking the version of data on EEPROM, to execute the right update of EEPROM and/or system. */
  uint8_t actualVersionEEPROM = 0;

  EEPROM.begin(sizeEEPROM);
  EEPROM.get(addressVersionEEPROM, actualVersionEEPROM);

  switch (actualVersionEEPROM) {
    /* If this is a first utilization about the system, will be launched the installation. */
    case 0:
      installConfiguration(button, screen);

    /* Else if is a first version, will be launched the upgrade to version 2. */
    case 1:
      upgradeConfigurationToVersionTwo(screen);

      /* 
       * Showing a message of complete.
       * This will have to show after the last update implemented. 
       */
      screen.showMessagePage(messagePageInstallationCompleteMessages);
      delay(timeoutMessage);

    default:
      loadConfiguration(sensor, screen, database, wifiSSID, wifiPassword);
  }
  EEPROM.end();
}

void loop() {
  /* 
   * Checking the status of button and execution the right action:
   *  - "-1" to execute WPS connection;
   *  - "1" to change the room ID.
   */
  resultButton = button.checkPress();
  if (resultButton == -1) {
    char c_wifiSSID[sizeWiFiSSID];
    char c_wifiPassword[sizeWiFiPassword];

    screen.showMessagePage(messagePageSearchingMessage);

    bool result = WiFi.beginWPSConfig(); 
    if (result) {
      if (WiFi.SSID().length() > 0) {
        wifiSSID = WiFi.SSID();
        wifiSSID.toCharArray(c_wifiSSID, sizeWiFiSSID);
        wifiPassword = WiFi.psk();
        wifiPassword.toCharArray(c_wifiPassword, sizeWiFiPassword);

        EEPROM.begin(sizeEEPROM);
        EEPROM.put(addressWiFiSSID, c_wifiSSID);
        EEPROM.put(addressWiFiPassword, c_wifiPassword);
        EEPROM.commit();
        EEPROM.end();

        screen.showMessagePage(messagePageSuccessfulMessage);
        delay(timeoutMessage);
      } else {
        WiFi.begin(wifiSSID, wifiPassword);

        screen.showMessagePage(messagePageErrorMessages);
        delay(timeoutMessage);
      }
    }
  } else if (resultButton == 1) {
    if (database.getRoomID() == MAX_ROOM_ID) {
      database.setRoomID(MIN_ROOM_ID);
    } else {
      database.setRoomID(database.getRoomID() + 1);
    }
    screen.setRoomID(database.getRoomID());
    screen.showMainPage();

    timeStartedEEPROM = millis() + timeSaveEEPROM;
  }

  /* Saving on EEPROM and updating the database only if the time is elapsed. */
  if ((timeStartedEEPROM < millis()) && (timeStartedEEPROM != 0)) {
    timeStartedEEPROM = 0;

    errorSavingDatabase = !database.executeQuery(DM_ROOM);

    EEPROM.begin(sizeEEPROM);
    if (EEPROM.read(addressRoomID) != database.getRoomID()) {
      EEPROM.write(addressRoomID, database.getRoomID());
      EEPROM.commit();
    }
    EEPROM.end();
  }

  /* Updating the status icons on the screen if there is a change. */
  if (screen.getIsErrorUpdate() != database.getIsErrorUpdate()) {
    screen.setIsErrorUpdate(database.getIsErrorUpdate());
    screen.showMainPage();
  }  

  if (screen.getIsConnectedWiFi() != WiFi.isConnected()) {
    screen.setIsConnectedWiFi(WiFi.isConnected());
    screen.showMainPage();
  } 

  /* If there was an error on previous saving of the new room ID into database, there will be a new attempt. */
  if (errorSavingDatabase) {
    errorSavingDatabase = !database.executeQuery(DM_ROOM);
  }       

  sensor.check();  
}