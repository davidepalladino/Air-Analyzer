 /**
  * @brief This library allows to manage the database for the Air Analyzer purposes.
  * The class is an observer where updates own status when is notified by the subject class.
  * Copyright (c) 2020 Davide Palladino. 
  * All right reserved.
  * 
  * @author Davide Palladino
  * @contact me@davidepalladino.com
  * @website www.davidepalladino.com
  * @version 1.0.0
  * @date 18th December, 2020
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

#ifndef DATABASEMANAGEMENT_H
    #define DATABASEMANAGEMENT_H

    #ifndef ARDUINO_H
        #include <Arduino.h>
    #endif

    #ifndef ESP8266WIFI_H
        #include <ESP8266WiFi.h>
    #endif

    #ifndef MYSQL_CONNECTION_H
        #include <MySQL_Connection.h>
    #endif

    #ifndef MYSQL_CURSOR_H
        #include <MySQL_Cursor.h>
    #endif

    #ifndef ABSTRACTOBSERVER_H
        #include <AbstractObserver.h>
    #endif

    #ifndef TIMEINTERVAL_H
        #include <DatetimeInterval.h>
    #endif

    #ifndef SENSOR_H
        #include <Sensor.h>
    #endif

    #ifndef CONFIGWIFI_H
        #include "..\..\..\configWiFi.h"
    #endif

    #ifndef CONFIGDATABASE_H
        #include "..\..\..\configDatabase.h"
    #endif

    class Sensor;

    class DatabaseManagement : private AbstractObserver {
        friend class AbstractSubject;

        public:
            /** 
             * @brief This constructor creates the object setting only the subjec class, the timezone and the minutes for every loading.
             * @param sensor Subject object that will notify.
             * @param timezone Timezone is expressed in hour.
             * @param totalMinuteUpdate Total minutes for every update.
             */
            DatabaseManagement(Sensor &sensor, int8_t timezone, uint8_t totalMinutesUpdate);

            /** 
             * @brief This method initializes the NTP object.
             */
            void begin();

        private:
            Sensor &sensor;
            DatetimeInterval* datetime;
            IPAddress* databaseAddress;
            MySQL_Connection* database;
            bool errorConnection;

            /**
             * @brief This method creates the query to insert the room of this sensor into database.
             * @return An array of characters.
             * @warning Use "free" function after the utilization, to prevent overflow of memory heap.
             */
            char* createQueryInsertRoom();

            /**
             * @brief This method creates the query to insert the actual values read into database.
             * @return An array of characters.
             * @warning Use "free" function after the utilization, to prevent overflow of memory heap.
             */
            char* createQueryInsertValues();

             /** 
             * @brief This method insert the acntual values into database.
             */
            void update();
    };
#endif