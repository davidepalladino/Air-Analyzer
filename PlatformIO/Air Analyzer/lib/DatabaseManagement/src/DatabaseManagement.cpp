#include <DatabaseManagement.h>

DatabaseManagement::DatabaseManagement(Sensor &sensor, int8_t timezone, uint8_t totalMinutesUpdate) : sensor(sensor) {
    this->sensor.addObserver(this);

    if (totalMinutesUpdate > 240) {
        totalMinutesUpdate = 240;
    }

    this->datetime = new DatetimeInterval(timezone, totalMinutesUpdate);
    
    this->databaseAddress = new IPAddress(databaseIP[0], databaseIP[1], databaseIP[2], databaseIP[3]);
    this->database = new MySQL_Connection((Client*) new WiFiClient());

    this->roomID = 0;

    this->isErrorUpdate = false;
}

void DatabaseManagement::begin() {
    this->datetime->begin();
    //executeQuery(DM_ROOM);
}

void DatabaseManagement::setRoomID(uint8_t roomID) { this->roomID = roomID; }

uint8_t DatabaseManagement::getRoomID() { return this->roomID; }

bool DatabaseManagement::getIsErrorUpdate() { return this->isErrorUpdate; }

char* DatabaseManagement::createQueryInsertRoom() {
    String queryString = "INSERT INTO airanalyzer.Room (ID) VALUES (" + 
                    String(this->roomID) + ")";

    uint8_t sizeQuery = queryString.length() + 1;
    char* queryChar = new char[sizeQuery];
    queryString.toCharArray(queryChar, sizeQuery);

    return queryChar;
}

char* DatabaseManagement::createQueryInsertValues() {
    delay(1000);

    String queryString = "INSERT INTO airanalyzer.`Values` (`When`, Room, Temperature, Humidity) VALUES ('" + 
                    String(this->datetime->getActualYear()) + "-" +
                    String(this->datetime->getActualMonth()) + "-" +
                    String(this->datetime->getActualDay()) + " " +
                    String(this->datetime->getActualHour()) + ":" +
                    String(this->datetime->getActualMinute()) + ":" +
                    String(this->datetime->getActualSecond()) + "', " +

                    String(roomID) + ", " +
                    
                    String(this->sensor.getTemperature())  + ", " +
                    String(this->sensor.getHumidity())  + ")";
    
    uint8_t sizeQuery = queryString.length() + 1;
    char* queryChar = new char[sizeQuery];
    queryString.toCharArray(queryChar, sizeQuery);

    return queryChar;
}

bool DatabaseManagement::executeQuery(queryType_t queryType) {
    if (WiFi.status() == WL_CONNECTED) {
        if (this->database->connect(*databaseAddress, databasePort, (char*) databaseUsername, (char*) databasePassword)) {
            char* query;
            switch (queryType) {
                case DM_ROOM:
                    query = createQueryInsertRoom();
                    break;
                case DM_VALUES:
                    query = createQueryInsertValues();
                    break;
            }
            
            MySQL_Cursor *cursorDatabase = new MySQL_Cursor(database);
            cursorDatabase->execute(query);
            this->database->close();

            delete query;
            delete cursorDatabase;

            this->isErrorUpdate = false;
            return true;
        } else {
            Serial.println("\033[1;91m[ERROR DATABASE]\033[0m");

            this->isErrorUpdate = true;
            return false;
        }
    } else if (WiFi.status() == WL_DISCONNECTED) {
        Serial.println("\033[1;91m[ERROR WIFI FROM DatabaseManagement]\033[0m");

        this->isErrorUpdate = true;
        return false;
    } else {
        this->isErrorUpdate = true;
        return false;
    }
}

void DatabaseManagement::update() {
    if ((this->datetime->checkTime() || isErrorUpdate) && executeQuery(DM_VALUES)) {            
        Serial.println("\033[1;92m------------------ [TRANSACTION] ------------------\033[0m");
        Serial.println("\033[1;92m[VALUES]\033[0m");
        Serial.print("\t\033[1;97mTEMPERATURE:   "); Serial.print(sensor.getTemperature());  Serial.println("\033[0m");
        Serial.print("\t\033[1;97mHUMIDITY:      "); Serial.print(sensor.getHumidity()); Serial.println("\033[0m");

        datetime->configNextDatetime();
    }
}