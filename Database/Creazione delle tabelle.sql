USE airanalyzer;

/* Creating the two table to store the room and the respective values. */
CREATE TABLE Room (
    ID INT PRIMARY KEY,
    Nome VARCHAR(25)
);

CREATE TABLE `Values` (
        `When` DATETIME NOT NULL,
        Room INT NOT NULL
            REFERENCES Room(ID)
            ON DELETE CASCADE
            ON UPDATE CASCADE,
        Temperature DECIMAL(5, 2) NOT NULL,
        Humidity DECIMAL(5,2) NOT NULL,
        PRIMARY KEY(`When`, Room)
);

/* Indexing the table of values to improve the performance. */
ALTER TABLE `Values` ADD INDEX Data (`When`);