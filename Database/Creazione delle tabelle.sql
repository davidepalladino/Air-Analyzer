/* Creating a table that will contain the rooms. */
CREATE TABLE Room (
    ID INT PRIMARY KEY,
    Name VARCHAR(25),
    IsActive BOOLEAN NOT NULL
);

/* Creating the rooms. */
INSERT INTO Room (ID, IsActive) VALUES (1, 0);
INSERT INTO Room (ID, IsActive) VALUES (2, 0);
INSERT INTO Room (ID, IsActive) VALUES (3, 0);
INSERT INTO Room (ID, IsActive) VALUES (4, 0);
INSERT INTO Room (ID, IsActive) VALUES (5, 0);
INSERT INTO Room (ID, IsActive) VALUES (6, 0);
INSERT INTO Room (ID, IsActive) VALUES (7, 0);
INSERT INTO Room (ID, IsActive) VALUES (8, 0);
INSERT INTO Room (ID, IsActive) VALUES (9, 0);

/* Creating a table thant will contain the measures. */
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

/* Indexing the table of measures to improve the performance. */
ALTER TABLE Measure ADD INDEX Data (DateAndTime);