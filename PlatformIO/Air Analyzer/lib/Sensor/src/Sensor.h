 /**
  * @brief This library allows to manage the sensor for the Air Analyzer purposes.
  * The class is an subject that could have some observers, that will be notified if 
  * there will be an update, with the "check" method.
  * Copyright (c) 2020 Davide Palladino. 
  * All right reserved.
  * 
  * @author Davide Palladino
  * @contact me@davidepalladino.com
  * @website www.davidepalladino.com
  * @version 1.0.1
  * @date 13th December, 2020
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

#ifndef SENSOR_H
    #define SENSOR_H

    #ifndef ARDUINO_H
        #include <Arduino.h>
    #endif

    #ifndef DHT_H
        #include <DHT.h>
    #endif

    #ifndef DHT_U_H
        #include <DHT_U.h>
    #endif

    #ifndef ABSTRACTSUBJECT_H
        #include <AbstractSubject.h>
    #endif

    #ifndef ABSTRACTOBSERVER_H
        #include <AbstractObserver.h>
    #endif

    #include <list>

    class Sensor : private AbstractSubject {
        public:
            friend class Screen;
            friend class DatabaseManagement;

            /** 
             * @brief This constructor creates the object setting only the pin and the type of sensor.
             * @param pin Pin where is connected the sensor.
             * @param type Type between "DHT11", "DHT12", "DHT21", "DHT22" and "AM2301".
             */
            Sensor(uint8_t pin, uint8_t type);

            /** 
             * @brief This method initializes the sensor object.
             */
            void begin();

            /** 
             * @brief This method checks if the is some variation of temperature or humidity and if there will be, the observers will be notified.
             * @return Boolean value "true" to indicate if there is a variation; else "false" value.
             */
            bool check();

            /** 
             * @brief This method gets the last value read about the temperature.
             * @return Last value of temperature read.
             */
            float getTemperature();

            /** 
             * @brief This method gets the last value read about the humidity.
             * @return Last value of humidity read.
             */
            float getHumidity();

        private:
            std::list<AbstractObserver* > observers;
            DHT* sensor;
            float temperature;
            float humidity;

            /** 
             * @brief This method checks if the is some variation of temperature between the object and the parameter.
             * @param temperature Value to compare.
             * @return Boolean value "true" to indicate if there is a variation; else "false" value.
             */
            bool checkTemperature(float temperature);

            /** 
             * @brief This method checks if the is some variation of humidity between the object and the parameter.
             * @param humidity Value to compare.
             * @return Boolean value "true" to indicate if there is a variation; else "false" value.
             */
            bool checkHumidity(float humidity);

            /** 
             * @brief This method adds an observer.
             * @param observer Object to add.
             */
            void addObserver(AbstractObserver* observer) override;

            /** 
             * @brief This method remove an observer.
             * @param observer Object to remove.
             */
            void removeObserver(AbstractObserver* observer) override;

            /** 
             * @brief This method notifies all observers.
             */
            void notify() override;
    };
#endif