#ifndef DATABASEMANAGEMENT_H
    #define DATABASEMANAGEMENT_H

    #ifndef ARDUINO_H
        #include <Arduino.h>
    #endif

    #ifndef ABSTRACTOBSERVER_H
        #include <AbstractObserver.h>
    #endif

    #ifndef SENSOR_H
        #include <Sensor.h>
    #endif

    class Sensor;

    class DatabaseManagement : private AbstractObserver {
        friend class AbstractSubject;

        public:
            DatabaseManagement(Sensor &sensor);

        private:
            Sensor &sensor;
            
            void update();
    };

#endif