#include <DatabaseManagement.h>

DatabaseManagement::DatabaseManagement(Sensor& sensor, const uint8_t databaseIP[4]) : sensor(sensor) {
    this->sensor.addObserver(this);

    databaseAddress = new IPAddress(databaseIP[0], databaseIP[1], databaseIP[2], databaseIP[3]);
    database = new MySQL_Connection((Client*) new WiFiClient());

    roomID = 0;

    isErrorUpdate = false;
    endTimeoutUpdateDabataseError = 0;
}

void DatabaseManagement::begin(int8_t timezone, uint8_t totalMinutesUpdate) {
    if (totalMinutesUpdate > 240) {
        totalMinutesUpdate = 240;
    }

    datetime = new DatetimeInterval(timezone, totalMinutesUpdate);
    datetime->begin();
    //executeQuery(DM_ROOM);
}

void DatabaseManagement::setRoomID(uint8_t roomID) { this->roomID = roomID; }

uint8_t DatabaseManagement::getRoomID() { return roomID; }

void DatabaseManagement::setUserID(uint32_t userID) { this->userID = userID; }

bool DatabaseManagement::getIsErrorUpdate() { return isErrorUpdate; }

char* DatabaseManagement::createQueryInsertRoom() {
    String queryString = "UPDATE airanalyzer_" + String(userID) + ".Room SET IsActive = 1 WHERE ID = " + 
                    String(roomID);

    uint8_t sizeQuery = queryString.length() + 1;
    char* queryChar = new char[sizeQuery];
    queryString.toCharArray(queryChar, sizeQuery);

    return queryChar;
}

char* DatabaseManagement::createQueryInsertValues() {
    delay(1000);

    String queryString = "INSERT INTO airanalyzer_" + String(userID) + ".Measure (DateAndTime, DayWeek, Room, Temperature, Humidity) VALUES ('" + 
                    String(datetime->getActualYear()) + "-" +
                    String(datetime->getActualMonth()) + "-" +
                    String(datetime->getActualDay()) + " " +
                    String(datetime->getActualHour()) + ":" +
                    String(datetime->getActualMinute()) + ":" +
                    String(datetime->getActualSecond()) + "', " +

                    String(datetime->getActualDayWeek()) + ", " +

                    String(roomID) + ", " +
                    
                    String(sensor.getTemperature())  + ", " +
                    String(sensor.getHumidity())  + ")";
    
    uint8_t sizeQuery = queryString.length() + 1;
    char* queryChar = new char[sizeQuery];
    queryString.toCharArray(queryChar, sizeQuery);

    return queryChar;
}

bool DatabaseManagement::executeQuery(queryType_t queryType) {
    if ((long) (endTimeoutUpdateDabataseError - millis()) <= 0) {
        if (WiFi.status() == WL_CONNECTED) {
            if (database->connect(*databaseAddress, databasePort, (char*) databaseUsername, (char*) databasePassword)) {
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
                database->close();

                delete query;
                delete cursorDatabase;

                isErrorUpdate = false;
                endTimeoutUpdateDabataseError = 0;
                return true;
            } else {
                Serial.println("\033[1;91m[DATABASE ERROR]\033[0m");

                isErrorUpdate = true;
                endTimeoutUpdateDabataseError = millis() + TIMEOUT_UPDATE_DATABASE_ERROR;
                return false;
            }
        } else if (WiFi.status() == WL_DISCONNECTED) {
            Serial.println("\033[1;91m[WIFI ERROR FROM DatabaseManagement]\033[0m");

            isErrorUpdate = true;
            return false;
        } else {
            isErrorUpdate = true;
            return false;
        }
    } else {
        isErrorUpdate = true;
        return false;
    }
}

void DatabaseManagement::update() {
    if ((datetime->checkTime() || endTimeoutUpdateDabataseError > 0) && executeQuery(DM_VALUES)) {            
        Serial.println("\033[1;92m------------------ [TRANSACTION] ------------------\033[0m");
        Serial.println("\033[1;92m[VALUES]\033[0m");
        Serial.print("\t\033[1;97mTEMPERATURE:   "); Serial.print(sensor.getTemperature());  Serial.println("\033[0m");
        Serial.print("\t\033[1;97mHUMIDITY:      "); Serial.print(sensor.getHumidity()); Serial.println("\033[0m");

        datetime->configNextDatetime();
    }
}