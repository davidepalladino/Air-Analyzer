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
    } dateTime_t;

    class DatetimeInterval {
        public:
            DatetimeInterval(int8_t timeZone, uint8_t totalMinuteUpdate) ;
            void begin(); 
            bool checkTime();
            uint16_t getActualYear();
            uint8_t getActualMonth();
            uint8_t getActualDay();
            uint8_t getActualHour();
            uint8_t getActualMinute();
            uint8_t getActualSecond();

        private:
            WiFiUDP* ntpUDP;
            NTPClient* ntpClient;
            struct tm* dateTimeTemp;
            uint8_t hourUpdate;
            uint8_t minuteUpdate;
            dateTime_t actualDatetime;
            dateTime_t nextDatetime;

            void setHourUpdate(uint8_t hourUpdate);
            void setMinuteUpdate(uint8_t minuteUpdate);
            void setDatetimeRaw(dateTime_t* dateTime, uint32_t raw);
            uint8_t getHourUpdate();
            uint8_t getMinuteUpdate();
            uint16_t getNextYear();
            uint8_t getNextMonth();
            uint8_t getNextDay();
            uint8_t getNextHour();
            uint8_t getNextMinute();
            uint8_t getNextSecond();
            uint8_t getMaxDay(uint8_t year, uint8_t month);
            bool configActualDatetime();
            bool configNextDatetime();
    };
#endif