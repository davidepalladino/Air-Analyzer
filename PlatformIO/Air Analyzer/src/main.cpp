#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <EEPROM.h>

#include <Button.h>
#include <Sensor.h>
#include <Screen.h>
#include <DatabaseManagement.h>

#include "configMain.h"

unsigned long calulateDelay(unsigned long timeStarted, unsigned long timeNecessary);

Sensor sensor(addressSensor, humidityResolution, temperatureResolution);
Screen screen(sensor, pinSCL, pinSDA);
DatabaseManagement database(sensor, databaseTimezone, databaseMinutesUpdate);
Button frontButton(pinButton, B_PULLUP, timeLongPress);

unsigned long timeStartedEEPROM = 0;
char wifiSSID[sizeWiFiSSID];
char wifiPassword[sizeWiFiPassword];
bool errorSavingDatabase = false;
int8_t resultButton = 0;

void setup() {
  unsigned long timeStartedLoadingMessage;
  float percentageLoadingMessage = 100 / (nLoadingMessages - 1);
  uint8_t iLoadingMessages = 0;
  uint8_t roomID;

  Serial.begin(baudRate);
  screen.begin();

  // EEPROM
  timeStartedLoadingMessage = millis();
  screen.showLoadingPage(loadingPageTexts[iLoadingMessages], (percentageLoadingMessage * iLoadingMessages++));
  EEPROM.begin(sizeEEPROM);
  EEPROM.get(addressWiFiSSID, wifiSSID);
  EEPROM.get(addressWiFiPassword, wifiPassword);
  EEPROM.get(addressRoomID, roomID);
  EEPROM.end();
  delay(calulateDelay(timeStartedLoadingMessage, timeoutLoadingMessage));

  // WiFi
  timeStartedLoadingMessage = millis();
  screen.showLoadingPage(loadingPageTexts[iLoadingMessages], (percentageLoadingMessage * iLoadingMessages++));
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
  screen.showLoadingPage(loadingPageTexts[iLoadingMessages], (percentageLoadingMessage * iLoadingMessages++));
  database.setRoomID(roomID);
  database.begin();
  delay(calulateDelay(timeStartedLoadingMessage, timeoutLoadingMessage));
  
  // Sensor
  timeStartedLoadingMessage = millis();
  screen.showLoadingPage(loadingPageTexts[iLoadingMessages], (percentageLoadingMessage * iLoadingMessages++));
  sensor.begin();
  delay(calulateDelay(timeStartedLoadingMessage, timeoutLoadingMessage));

  // Screen
  screen.showLoadingPage(loadingPageTexts[iLoadingMessages], (percentageLoadingMessage * iLoadingMessages++));
  screen.setRoomID(roomID);
  screen.setIsConnectedWiFi(true);
  screen.setIsErrorUpdate(false);
}

void loop() {
  /* Checking the status of button and execution the right action:
   *  "-1" to execute WPS connection;
   *  "1" to change the room ID.
   */
  resultButton = frontButton.checkPress();
  if (resultButton == -1) {
    screen.showMessagePage(messagePageSearchingTexts);
    bool result = WiFi.beginWPSConfig(); 
    if (result) {
      if (WiFi.SSID().length() > 0) {
        WiFi.SSID().toCharArray(wifiSSID, sizeWiFiSSID);
        WiFi.psk().toCharArray(wifiPassword, sizeWiFiPassword);

        EEPROM.begin(sizeEEPROM);
        EEPROM.put(addressWiFiSSID, wifiSSID);
        EEPROM.put(addressWiFiPassword, wifiPassword);
        EEPROM.commit();
        EEPROM.end();

        screen.showMessagePage(messagePageSuccessfulTexts, WiFi.SSID());
        delay(timeoutMessage);
      } else {
        WiFi.begin(wifiSSID, wifiPassword);

        screen.showMessagePage(messagePageErrorTexts[0], messagePageErrorTexts[1]);
        delay(timeoutMessage);
      }
    }
  } else if (resultButton == 1) {
    if (database.getRoomID() == maxRoomID) {
      database.setRoomID(minRoomID);
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

/**
 * @brief This function provides to calculate the delay based on the past time and the time necessary. Is used for the loading message during the loading page.
 * @param timeStarted Past time, where the timer is started.
 * @param timeNecessary Time necessary, where will be used to calculate the difference.
 * @return "0" if the difference between the past time and the present time is greater than time necessary; else the result of this difference with time necessary.
 */
unsigned long calulateDelay(unsigned long timeStarted, unsigned long timeNecessary) {
  unsigned long difference = millis() - timeStarted;
  return (timeNecessary - difference) < 0 ? 0 : (timeNecessary - difference);
}