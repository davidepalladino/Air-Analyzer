#ifndef DATABASEMANAGEMENT_H
    #define DATABASEMANAGEMENT_H

    #ifndef ARDUINO_H
        #include <Arduino.h>
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

    #ifndef CREDENTIALSDATABASE_H
        #include "..\..\..\credentialsDatabase.h"
    #endif

    class Sensor;

    class DatabaseManagement : private AbstractObserver {
        friend class AbstractSubject;

        public:
            DatabaseManagement(Sensor &sensor, bool solarTime, uint8_t totalMinutesUpdate);
            void begin();

        private:
            Sensor &sensor;
            DatetimeInterval* datetime;
            WiFiClient client;
            IPAddress* databaseAddress;
            MySQL_Connection* database;
            char* createQuery();
            void update();
    };

#endif