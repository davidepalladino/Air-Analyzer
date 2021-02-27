 /**
  * @brief This library allows to manage the screen for the Air Analyzer purposes.
  * The class is an observer where updates own status when is notified by the subject class.
  * Copyright (c) 2020 Davide Palladino.
  * All right reserved.
  * 
  * @author Davide Palladino
  * @contact me@davidepalladino.com
  * @website www.davidepalladino.com
  * @version 1.3.0
  * @date 28th February, 2021
  * 
  * This library is free software; you can redistribute it and/or
  *  modify it under the terms of the GNU General Public
  *  License as published by the Free Software Foundation; either
  *  version 3.0 of the License, or (at your option) any later version
  * 
  * This library is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
  *  GNU Lesser General Public License for more details.
  * 
  */

#ifndef SCREEN_H
    #define SCREEN_H

    #ifndef ARDUINO_H
        #include <Arduino.h>
    #endif

    #ifndef ABSTRACTOBSERVER_H
        #include <AbstractObserver.h>
    #endif

    #ifndef U8G2LIB_H
        #include <U8g2lib.h>
    #endif

    #ifndef SENSOR_H
        #include <Sensor.h>
    #endif

    #ifndef CONFIGSCREEN_H
        #include "configScreen.h"
    #endif

    #ifndef ICONSSCREEN_H
        #include "iconsScreen.h"
    #endif

    class Sensor;

    class Screen : private AbstractObserver {
        friend class AbstractSubject;

        public:
            /** 
             * @brief This constructor creates the object setting only the subject class and the pins where is
             *  connected the screen.
             * @param sensor Subject object that will notify.
             * @param pinSCL Pin SCL where is connected the screen.
             * @param pinSDA Pin SDA where i connected the screen.
             */
            Screen(Sensor& sensor, uint8_t pinSCL, uint8_t pinSDA);

            /** 
             * @brief This method initializes the screen object.
             */
            void begin();

            /**
             * @brief This method sets the room ID.
             * @param roomID Number of room.
             */
            void setRoomID(uint8_t roomID);

            /**
             * @brief This method sets the status indicator about the WiFi connection.
             * @param isConnectedWiFi Value "true" if Wifi is connected; else, value "false".
             */
            void setIsConnectedWiFi(bool isConnectedWiFi);

            /**
             * @brief This method gets the status indicator about the WiFi connection.
             * @return Value "true" if Wifi is connected; else, value "false".
             */
            bool getIsConnectedWiFi();

            /**
             * @brief This method sets the status indicator about the error on update.
             * @param isConnectedWiFi Value "true" if there is an error during update; else, value "false".
             */
            void setIsErrorUpdate(bool isErrorUpdate);

            /**
             * @brief This method gets the status indicator about the error on update.
             * @return Value "true" if there is an error during update; else, value "false".
             */
            bool getIsErrorUpdate();

            /**
             * @brief This methods shows an installation page about the room ID.
             * @param messages Array of three messages to visualize.
             */
            void showInstallationRoomIDPage(const String messages[3]);

            /**
             * @brief This methods shows an installation page about the WiFi.
             * @param messages Array of six messages to visualize.
             * @param result Type of rows to visualize: "0" for three rows considering the first, second and third message;
             *  "1" considering the fourth and fifth messages ; "2" considering the sixth message.
             */
            void showInstallationWiFiPage(const String messages[6], uint8_t result);

            /**
             * @brief This methods shows an installation page about the timezone.
             * @param messages Array of three messages to visualize. To the last message will be append the "timezone" value.
             * @param timezone Value selected and that must be show.
             */
            void showInstallationTimezonePage(const String messages[3], int8_t timezone);

            /**
             * @brief This methods shows an configuration page about the update two version.
             * @param messages Array of three messages to visualize. To the last message will be append the "localIP" value.
             * @param localIP Value selected and that must be show.
             */
            void showUpgradeConfigurationToVersionTwoPage(const String messages[2], String localIP);

            /**
             * @brief This method shows a page with an loading bar and a description message.
             * @param message The specific message to visualize.
             * @param percentage Percentage to fill the loading bar.
             */
            void showLoadingPage(String message, float percentage);

            /**
             * @brief This method shows the main page using the sensor object (to visualize the values) the room ID
             *  and the status of connection and update.
             */
            void showMainPage();

            /**
             * @brief This method shows a simple page with a simple row of message at the center.
             * @param message The specific row message to visualize.
             * @warning The row of message shall be of maximum 18 character. Other characters may be lost at right of screen.
             */
            void showMessagePage(String message);

            /**
             * @brief This method shows a simple page with two simple rows of messages at the center.
             * @param messages The two messages to visualize.
             * @warning The single row of message shall be of maximum 18 character. Other characters may be lost at right of screen.
             */
            void showMessagePage(const String messages[2]);
            
        private:
            U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* screen;
            Sensor& sensor;
            uint8_t roomID;
            bool isConnectedWiFi;
            bool isErrorUpdate;

            /** 
             * @brief This method draws the bar and the internal fill considering the percenage.
             * @param percentage Percentage to fill the loading bar.
             */
            void drawBar(float percentage);

            /**
             * @brief This method draws the single message considering the "x" and "y" positions.
             * @param positionX X position.
             * @param positionY Y position.
             * @param message The specific row message to visualize.
             * @warning The row of message shall be of maximum 18 character. Other characters may be lost at right of screen.
             */
            void drawMessage(uint8_t positionX, uint8_t positionY, String message);

            /**
             * @brief This method draws a circle with the room ID inside.
             */
            void drawRoomID();

            /**
             * @brief This method draws the icon of temperature and the read value.
             */
            void drawTemperature();

            /**
             * @brief This method draws the icon of humidity and the read value.
             */
            void drawHumidity();

            /**
             * @brief This method draws the status icon of WiFi.
             */
            void drawWiFiStatus();

            /**
             * @brief This method draws the status icon of error.
             */
            void drawErrorStatus();

            /** 
             * @brief This method update the main screen.
             */
            void update();
    };
#endif