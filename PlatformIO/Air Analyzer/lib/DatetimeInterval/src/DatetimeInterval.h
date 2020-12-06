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
            DatetimeInterval(int8_t timezone, uint8_t totalMinuteUpdate);
            void begin(); 
            bool checkTime();
            uint16_t getActualYear();
            uint8_t getActualMonth();
            uint8_t getActualDay();
            uint8_t getActualHour();
            uint8_t getActualMinute();
            uint8_t getActualSecond();

        private:
            NTPClient* ntpClient;
            uint8_t updateHour;
            uint8_t updateMinute;
            struct tm* tempDatetime;
            datetime_t actualDatetime;
            datetime_t nextDatetime;

            void setUpdateHour(uint8_t updateHour);
            void setUpdateMinute(uint8_t updateMinute);
            uint8_t getUpdateHour();
            uint8_t getUpdateMinute();
            uint16_t getNextYear();
            uint8_t getNextMonth();
            uint8_t getNextDay();
            uint8_t getNextHour();
            uint8_t getNextMinute();
            uint8_t getNextSecond();
            uint8_t getMaxDay(uint8_t year, uint8_t month);
            void setDatetimeRaw(datetime_t* datetime, uint32_t raw);
            bool configActualDatetime();
            bool configNextDatetime();
    };
#endif