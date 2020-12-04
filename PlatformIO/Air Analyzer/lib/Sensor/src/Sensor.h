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

            Sensor(uint8_t pin, uint8_t type);
            void begin();
            bool check();

        private:
            std::list<AbstractObserver* > observers;
            DHT* sensor;
            float temperature;
            float humidity;

            bool setTemperature(float temperature);
            bool setHumidity(float humidity);
            float getTemperature();
            float getHumidity();
            void addObserver(AbstractObserver* observer) override;
            void removeObserver(AbstractObserver* observer) override;
            void notify() override;
    };
#endif