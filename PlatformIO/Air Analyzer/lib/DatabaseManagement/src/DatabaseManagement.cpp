#include <DatabaseManagement.h>

DatabaseManagement::DatabaseManagement(Sensor &sensor, int8_t timezone, uint8_t totalMinutesUpdate) : sensor(sensor) {
    this->sensor.addObserver(this);

    if (totalMinutesUpdate > 240) {
        totalMinutesUpdate = 240;
    }

    this->datetime = new DatetimeInterval(timezone, totalMinutesUpdate);
    
    this->databaseAddress = new IPAddress(databaseIP[0], databaseIP[1], databaseIP[2], databaseIP[3]);
    this->database = new MySQL_Connection((Client*) new WiFiClient());

    this->errorConnection = false;
}

void DatabaseManagement::begin() {
    this->datetime->begin();

    while (WiFi.status() != WL_CONNECTED) {
        WiFi.reconnect();
    }
    
    if (this->database->connect(*databaseAddress, databasePort, (char*) databaseUsername, (char*) databasePassword)) {
        const char* query = createQueryInsertRoom();
        
        MySQL_Cursor *cursorDatabase = new MySQL_Cursor(database);
        cursorDatabase->execute(query);
        this->database->close();

        delete query;
        delete cursorDatabase;
    }
}

char* DatabaseManagement::createQueryInsertRoom() {
    String queryString = "INSERT INTO airanalyzer.Room (ID, Nome) VALUES (" + 
                    String(databaseRoomID) + ", '" +
                    databaseRoomName  + "')";

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

                    String(databaseRoomID) + ", " +
                    
                    String(this->sensor.getTemperature())  + ", " +
                    String(this->sensor.getHumidity())  + ")";
    
    uint8_t sizeQuery = queryString.length() + 1;
    char* queryChar = new char[sizeQuery];
    queryString.toCharArray(queryChar, sizeQuery);

    return queryChar;
}

void DatabaseManagement::update() {
    if (WiFi.status() == WL_CONNECTED) {
        if (this->datetime->checkTime() || errorConnection) {            
            if (this->database->connect(*databaseAddress, databasePort, (char*) databaseUsername, (char*) databasePassword)) {
                Serial.println("\033[1;92m[VALUES]\033[0m");
                Serial.print("\t\033[1;97mTEMPERATURE:   "); Serial.print(sensor.getTemperature());  Serial.println("\033[0m");
                Serial.print("\t\033[1;97mHUMIDITY:      "); Serial.print(sensor.getHumidity()); Serial.println("\033[0m");

                const char* query = createQueryInsertValues();
                MySQL_Cursor *cursorDatabase = new MySQL_Cursor(database);
                cursorDatabase->execute(query);
                this->database->close();

                delete query;
                delete cursorDatabase;

                Serial.println("\033[1;92m---------------------------------------------------\033[0m");

                this->errorConnection = false;
            } else {
                this->errorConnection = true;
            }
        }
    } else {
        WiFi.reconnect();
    }
}