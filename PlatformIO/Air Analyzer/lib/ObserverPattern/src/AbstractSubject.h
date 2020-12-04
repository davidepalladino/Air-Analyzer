#ifndef ABSTRACTOBSUBJECT_H
    #define ABSTRACTOBSUBJECT_H

    #ifndef ARDUINO_H
        #include <Arduino.h>
    #endif

    #ifndef ABSTRACTOBSERVER_H
        #include <AbstractObserver.h>
    #endif

    class AbstractSubject {
        public:
            virtual void addObserver(AbstractObserver* observer) = 0;
            virtual void removeObserver(AbstractObserver* observer) = 0;
            virtual void notify() = 0;
    };
#endif