USE airanalyzer;

/* Creating a table that will contain the room ID. */
CREATE TABLE Room (
    ID INT PRIMARY KEY,
    Name VARCHAR(25),
    IsActive BOOLEAN NOT NULL,
);

CREATE TABLE Measure (
    DateAndTime DATETIME NOT NULL,
    DayWeek INT NOT NULL,
    Room INT NOT NULL
        REFERENCES Room(ID)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    Temperature DECIMAL(5, 2) NOT NULL,
    Humidity DECIMAL(5,2) NOT NULL,
    PRIMARY KEY(DateAndTime, Room)
);

/* Indexing the table of values to improve the performance. */
ALTER TABLE Measure ADD INDEX Data (DateAndTime);