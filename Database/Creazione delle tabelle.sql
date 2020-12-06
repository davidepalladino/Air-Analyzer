USE airanalyzer;

CREATE TABLE Room (
    ID INT PRIMARY KEY,
    Nome VARCHAR(25) NOT NULL
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

ALTER TABLE `Values` ADD INDEX Data (`When`);

INSERT INTO Room (ID, Nome) VALUES (5, 'Cameretta');