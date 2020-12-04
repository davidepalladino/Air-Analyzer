#ifndef ABSTRACTOBSERVER_H
    #define ABSTRACTOBSERVER_H

    #ifndef ARDUINO_H
        #include <Arduino.h>
    #endif

    class AbstractObserver {
        public:
            virtual void update() = 0;
    };
#endif