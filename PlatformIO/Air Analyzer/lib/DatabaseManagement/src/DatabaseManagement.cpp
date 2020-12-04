#include <DatabaseManagement.h>

DatabaseManagement::DatabaseManagement(Sensor &sensor) : sensor(sensor) {
    this->sensor.addObserver(this);
}

void DatabaseManagement::update() {

}