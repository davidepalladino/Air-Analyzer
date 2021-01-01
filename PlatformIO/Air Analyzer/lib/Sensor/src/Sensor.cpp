#include "Sensor.h"

Sensor::Sensor(uint8_t pin, uint8_t type) {
    sensor = new DHT(pin, type);
    temperature = 0;
    humidity = 0;
}

void Sensor::begin() { sensor->begin(); }

bool Sensor::check() {
    if ((checkTemperature(sensor->readTemperature()) || checkHumidity(sensor->readHumidity())) && ((getTemperature() > 0) && (getHumidity() > 0))) {
        notify();
        return true;
    } else {
        if (getTemperature() < 1 || getHumidity() < 1) {
            Serial.println("\033[1;91m[ERROR VALUE READ]\033[0m");
        }
        return false;
    }
}

float Sensor::getTemperature() { return this->temperature; }

float Sensor::getHumidity() { return this->humidity; }

bool Sensor::checkTemperature(float temperature) {
    if (getTemperature() != temperature) {
        this->temperature = temperature;
        return true;
    } else {
        return false;
    }
}

bool Sensor::checkHumidity(float humidity) {
    if (getHumidity() != humidity) {
        this->humidity = humidity;
        return true;
    } else {
        return false;
    }
}

void Sensor::addObserver(AbstractObserver* observer) { this->observers.push_back(observer); }

void Sensor::removeObserver(AbstractObserver* observer) { this->observers.remove(observer); }

void Sensor::notify() {
    std::list<AbstractObserver* >::iterator iteratorObservers = this->observers.begin();
    
    while(iteratorObservers != this->observers.end()) {
        (*iteratorObservers)->update();
        iteratorObservers++;
    }
}