#include "Sensor.h"

Sensor::Sensor(uint8_t pin, uint8_t type) {
    sensor = new DHT(pin, type);
    temperature = 0;
    humidity = 0;
}

void Sensor::begin() { sensor->begin(); }

bool Sensor::check() {
    if(setTemperature(sensor->readTemperature()) || setHumidity(sensor->readHumidity())) {
        notify();
        return true;
    } else {
        return false;
    }
}

void Sensor::addObserver(AbstractObserver* observer) {
    this->observers.push_back(observer);
}

void Sensor::removeObserver(AbstractObserver* observer) {
    this->observers.remove(observer);
}

bool Sensor::setTemperature(float temperature) {
    if (getTemperature() != temperature) {
        this->temperature = temperature;
        return true;
    } else {
        return false;
    }
}

bool Sensor::setHumidity(float humidity) {
    if (getHumidity() != humidity) {
        this->humidity = humidity;
        return true;
    } else {
        return false;
    }
}

float Sensor::getTemperature() { return this->temperature; }

float Sensor::getHumidity() { return this->humidity; }

void Sensor::notify() {
    std::list<AbstractObserver* >::iterator iteratorObservers = this->observers.begin();
    
    while(iteratorObservers != this->observers.end()) {
        (*iteratorObservers)->update();
        iteratorObservers++;
    }
}