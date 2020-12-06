#include <DatabaseManagement.h>

DatabaseManagement::DatabaseManagement(Sensor &sensor, bool solarTime, uint8_t totalMinutesUpdate) : sensor(sensor) {
    this->sensor.addObserver(this);

    this->datetime = new DatetimeInterval(solarTime, totalMinutesUpdate);
    
    this->databaseAddress = new IPAddress(databaseIP[0], databaseIP[1], databaseIP[2], databaseIP[3]);
    this->database = new MySQL_Connection((Client*) &client);
}

void DatabaseManagement::begin() {
    this->datetime->begin();
}

char* DatabaseManagement::createQuery() {
    String queryString = "INSERT INTO airanalyzer.`Values` (`When`, Room, Temperature, Humidity) VALUES ('" + 
                    String(this->datetime->getActualYear()) + "-" +
                    String(this->datetime->getActualMonth()) + "-" +
                    String(this->datetime->getActualDay()) + " " +
                    String(this->datetime->getActualHours()) + ":" +
                    String(this->datetime->getActualMinutes()) + ":" +
                    String(this->datetime->getActualSeconds()) + "', " +

                    String(databaseRoomID) + ", " +
                    
                    String(this->sensor.getTemperature())  + ", " +
                    String(this->sensor.getHumidity())  + ")";
    
    uint8_t sizeQuery = queryString.length() + 1;
    char* queryChar = new char[sizeQuery];
    queryString.toCharArray(queryChar, sizeQuery);

    return queryChar;
}

void DatabaseManagement::update() {
    if (this->datetime->checkTime()) {
        if (this->database->connect(*databaseAddress, databasePort, (char*) databaseUsername, (char*) databasePassword)) {
            delay(1000);

            const char* query = createQuery();
            
            MySQL_Cursor *cursorDatabase = new MySQL_Cursor(database);
            cursorDatabase->execute(query);
            this->database->close();

            delete query;
            delete cursorDatabase;
        }
    }
}

