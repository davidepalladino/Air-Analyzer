#ifndef SCREEN_H
    #define SCREEN_H

    #ifndef ARDUINO_H
        #include <Arduino.h>
    #endif

    #ifndef ABSTRACTOBSERVER_H
        #include <AbstractObserver.h>
    #endif

    #ifndef U8G2LIB_H
        #include <U8g2lib.h>
    #endif

    #ifndef SENSOR_H
        #include <Sensor.h>
    #endif

    #ifndef CONFIGSCREEN_H
        #include "configScreen.h"
    #endif

    #ifndef ICONSSCREEN_H
        #include "iconsScreen.h"
    #endif

    class Sensor;

    class Screen : private AbstractObserver {
        friend class AbstractSubject;

        public:
            Screen(Sensor &sensor, uint8_t pinSCL, uint8_t pinSDA);
            void begin();

        private:
            U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C* screen;
            Sensor &sensor;

            void drawIcons();
            void drawValues();
            void drawUnits();
            void update();
    };

#endif