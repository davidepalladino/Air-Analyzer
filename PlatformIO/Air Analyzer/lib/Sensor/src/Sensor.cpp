#include "Sensor.h"

Sensor::Sensor(uint8_t pin, uint8_t type) {
    sensorDHT = new DHT(pin, type);
    sensorHDC = NULL;

    temperature = 0;
    humidity = 0;

    endTimeoutHDC = 0;
}

Sensor::Sensor(uint8_t address, HDC1080_MeasurementResolution humidityResolution, HDC1080_MeasurementResolution temperatureResolution) {
    sensorDHT = NULL;
    sensorHDC = new ClosedCube_HDC1080();

    this->address = address;
    this->humidityResolution = humidityResolution;
    this->temperatureResolution = temperatureResolution;

    temperature = 0;
    humidity = 0;
}

void Sensor::begin() { 
    if (sensorDHT != NULL) {
        sensorDHT->begin();
    } else if (sensorHDC != NULL) {
        sensorHDC->begin(address);
        sensorHDC->setResolution(humidityResolution, temperatureResolution);
    }
}

bool Sensor::check() {
    bool changed = false;

    if (sensorDHT != NULL) {
        if (checkTemperature(sensorDHT->readTemperature()) || checkHumidity(sensorDHT->readHumidity())) {
            changed = true;
        } else {
            changed = false;
        }
    } else if (sensorHDC != NULL) {
        /* 
         * There is a case where "timeout" will go to overflow and the result of "millis()" not. 
         * In this case the sensor will be read every time until the result of "millis()" will go to overflow, too. 
         */
        if ((long) (endTimeoutHDC - millis()) <= 0) {
            endTimeoutHDC = millis() + TIMEOUT_READ_HDC;

            if (checkTemperature(sensorHDC->readTemperature()) || checkHumidity(sensorHDC->readHumidity())) {
                changed = true;
            } else {
                changed = false;
            }

        } else {
            changed = false;
        }
    }

    if (changed) {
        if ((getTemperature() >= 1) && (getHumidity() >= 1) && (getTemperature() <= 124) && (getHumidity() <= 99)) {
            notify();
            return true;
        } else {
            Serial.println("\033[1;91m[SENSOR ERROR]\033[0m");
            return false;
        }
    } else {
        return false;
    }
}

double Sensor::getTemperature() { return temperature; }

double Sensor::getHumidity() { return humidity; }

bool Sensor::checkTemperature(double temperature) {
    if (getTemperature() != temperature) {
        this->temperature = temperature;
        return true;
    } else {
        return false;
    }
}

bool Sensor::checkHumidity(double humidity) {
    if (getHumidity() != humidity) {
        this->humidity = humidity;
        return true;
    } else {
        return false;
    }
}

void Sensor::addObserver(AbstractObserver* observer) { observers.push_back(observer); }

void Sensor::removeObserver(AbstractObserver* observer) { observers.remove(observer); }

void Sensor::notify() {
    std::list<AbstractObserver* >::iterator iteratorObservers = observers.begin();
    
    while (iteratorObservers != observers.end()) {
        (*iteratorObservers)->update();
        iteratorObservers++;
    }
}