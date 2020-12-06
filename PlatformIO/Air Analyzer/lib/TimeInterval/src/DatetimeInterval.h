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

    class DatetimeInterval {
        public:
            DatetimeInterval(bool solarTime, uint8_t totalMinutesUpdate);
            uint16_t getActualYear();
            uint8_t getActualMonth();
            uint8_t getActualDay();
            uint8_t getActualMinutes();
            uint8_t getActualSeconds();
            uint8_t getActualHours();
            void begin(); 
            bool checkTime();

        private:
            WiFiUDP* ntpUDP;
            NTPClient* ntpClient;
            struct tm* datetimeActual;
            uint8_t hoursUpdate;
            uint8_t minutesUpdate;
            uint16_t actualYear;
            uint8_t actualMonth;
            uint8_t actualDay;
            uint8_t actualHours;
            uint8_t actualMinutes;
            uint8_t actualSeconds;
            uint8_t nextHours;
            uint8_t nextMinutes;
            uint8_t nextSeconds;

            void setHoursUpdate(uint8_t hoursUpdate);
            void setMinutesUpdate(uint8_t minutesUpdate);
            void setActualYear(uint32_t actualYearRaw);
            void setActualMonth(uint32_t actualMonthRaw);
            void setActualDay(uint32_t actualDayRaw);
            void setActualHours(uint8_t actualHours);
            void setActualMinutes(uint8_t actualMinutes);
            void setActualSeconds(uint8_t actualSeconds);
            void setNextHours(uint8_t nextHours);
            void setNextMinutes(uint8_t nextMinutes);
            void setNextSeconds(uint8_t nextSeconds);
            uint8_t getHoursUpdate();
            uint8_t getMinutesUpdate();
            uint8_t getNextSeconds();
            uint8_t getNextHours();
            uint8_t getNextMinutes();
            bool configActualDatetime();
            bool configNextTime();
    };
#endif