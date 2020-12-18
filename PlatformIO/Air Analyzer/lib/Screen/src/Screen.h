 /**
  * @brief This library allows to manage the screen for the Air Analyzer purposes.
  * The class is an observer where updates own status when is notified by the subject class.
  * Copyright (c) 2020 Davide Palladino.
  * All right reserved.
  * 
  * @author Davide Palladino
  * @contact me@davidepalladino.com
  * @website www.davidepalladino.com
  * @version 1.0.0
  * @date 16th December, 2020
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
            Screen(Sensor &sensor, uint8_t pinSCL, uint8_t pinSDA);

            /** 
             * @brief This method initializes the screen object.
             */
            void begin();

        private:
            U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* screen;
            Sensor &sensor;

            /** 
             * @brief This method draws the icons of temperature and humidity.
             */
            void drawIcons();

            /** 
             * @brief This method draws the values of temperature and humidity.
             */
            void drawValues();

            /** 
             * @brief This method draws the units of temperature and humidity.
             */
            void drawUnits();

            /** 
             * @brief This method shows the actual values, using the "draw" methods.
             */
            void update();
    };
#endif