 /**
  * @brief This library allows to manage an inteval of time between the actual (get from an NTP server) and 
  * the next set. 
  * Copyright (c) 2020 Davide Palladino. 
  * All right reserved.
  * 
  * @author Davide Palladino
  * @contact me@davidepalladino.com
  * @website www.davidepalladino.com
  * @version 1.0.1
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

#ifndef DATETIMEINTERVAL_H
    #define DATETIMEINTERVAL_H

    #ifndef ARDUINO_H
        #include <Arduino.h>
    #endif

    #ifndef ESP8266WIFI_H
        #include <ESP8266WiFi.h>
    #endif

    #ifndef WIFIUDP_H
        #include <WiFiUdp.h>
    #endif

    #ifndef NTPCLIENT_H
        #include <NTPClient.h>
    #endif

    #ifndef TIME_H
        #include <time.h>
    #endif

    #ifndef CONFIGWIFI_H
        #include "..\..\..\configWiFi.h"
    #endif

    typedef struct {
        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
    } datetime_t;

    class DatetimeInterval {
        public:
            /** 
             * @brief This constructor creates the object setting the timezone and the minutes for every update.
             * @param timezone Timezone is expressed in hour.
             * @param totalMinuteUpdate Total minutes for every update.
             * @warning The max minutes will be 240. Every value greater than this will be considered as such.
             */
            DatetimeInterval(int8_t timezone, uint8_t totalMinuteUpdate);

            /** 
             * @brief This method initializes the NTP object.
             */
            void begin(); 

            /** 
             * @brief This method initializes the NTP object.
             * @return Boolean value "true" to indicate if there is a variation; else "false" value.
             */
            bool checkTime();

            /** 
             * @brief This method gets the actual and last year read with th NTP object.
             * @return Actual and last year read.
             */
            uint16_t getActualYear();

            /** 
             * @brief This method gets the actual and last month read with th NTP object.
             * @return Actual and last month read.
             */
            uint8_t getActualMonth();

            /** 
             * @brief This method gets the actual and last day read with th NTP object.
             * @return Actual and last day read.
             */
            uint8_t getActualDay();

            /** 
             * @brief This method gets the actual and last hour read with th NTP object.
             * @return Actual and last hour read.
             */
            uint8_t getActualHour();

            /** 
             * @brief This method gets the actual and last minute read with th NTP object.
             * @return Actual and last minute read.
             */
            uint8_t getActualMinute();

            /** 
             * @brief This method gets the actual and last second read with th NTP object.
             * @return Actual and last second read.
             */
            uint8_t getActualSecond();

        private:
            NTPClient* ntpClient;
            uint8_t updateHour;
            uint8_t updateMinute;
            struct tm* tempDatetime;
            datetime_t actualDatetime;
            datetime_t nextDatetime;

            /** 
             * @brief This method translates a raw format to datetime_t format.
             * @param datetime Variable where will be transfered the converted datetime.
             * @param raw Raw format of datetime.
             */
            void translateDatetime(datetime_t* datetime, uint32_t raw);

            /** 
             * @brief This method gets the max day considering the year and the month.
             * @param datetime Year to consider.
             * @param raw Month to consider.
             */
            uint8_t getMaxDay(uint8_t year, uint8_t month);

            /** 
             * @brief This method configs the actual datetime with the NTP object.
             * @return Boolean value "false" to indicate if there is an error with the connection; else "false" value.
             */
            bool configActualDatetime();

            /** 
             * @brief This method configs the next datetime, based on the "updateHour" and "updateMinute".
             */
            void configNextDatetime();
    };
#endif