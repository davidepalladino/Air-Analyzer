#include "DatetimeInterval.h"

DatetimeInterval::DatetimeInterval(bool solarTime, uint8_t totalMinutesUpdate) {
    this->ntpUDP = new WiFiUDP();

    if (solarTime) {
        this->ntpClient = new NTPClient(*this->ntpUDP, 3600);
    } else {
        this->ntpClient = new NTPClient(*this->ntpUDP);
    }

    setHoursUpdate(totalMinutesUpdate / 60);
    setMinutesUpdate(totalMinutesUpdate - (getHoursUpdate() * 60));
}

void DatetimeInterval::begin() {
    this->ntpClient->begin();

    if (WiFi.status() == WL_CONNECTED) {
        this->ntpClient->update();

        setActualYear(this->ntpClient->getEpochTime());
        setActualMonth(this->ntpClient->getEpochTime());
        setActualDay(this->ntpClient->getEpochTime());
        setActualHours(this->ntpClient->getHours());
        setActualMinutes(this->ntpClient->getMinutes());
        setActualSeconds(this->ntpClient->getSeconds());

        setNextHours(this->ntpClient->getHours());
        setNextMinutes(this->ntpClient->getMinutes());
        setNextSeconds(this->ntpClient->getSeconds());
    } else {
        WiFi.reconnect();
    }
}

bool DatetimeInterval::checkTime() {
    bool result = false;

    if (WiFi.status() == WL_CONNECTED) {
        this->ntpClient->update();

        if (this->ntpClient->getHours() > getNextHours()) {
            result = true;
        } else if ((this->ntpClient->getHours() == getNextHours()) && (this->ntpClient->getMinutes() > getNextMinutes())) {
            result = true;
        } else if ((this->ntpClient->getHours() == getNextHours()) && (this->ntpClient->getMinutes() == getNextMinutes()) && (this->ntpClient->getSeconds() > getNextSeconds())) {
            result = true;
        }
    } else {
        WiFi.reconnect();
    }

    if (result) {
        configNextTime();
    }
    
    return result;
}

void DatetimeInterval::setHoursUpdate(uint8_t hoursUpdate) { this->hoursUpdate = hoursUpdate; }

void DatetimeInterval::setMinutesUpdate(uint8_t minutesUpdate) { this->minutesUpdate = minutesUpdate; }

void DatetimeInterval::setActualYear(uint32_t actualYearRaw) {
    time_t rawTime = actualYearRaw;
    datetimeActual = localtime(&rawTime);
    this->actualYear = (uint16_t) datetimeActual->tm_year + 1900;
}

void DatetimeInterval::setActualMonth(uint32_t actualMonthRaw) {
    time_t rawTime = actualMonthRaw;
    datetimeActual = localtime(&rawTime);
    this->actualMonth = (uint8_t) datetimeActual->tm_mon + 1;
}

void DatetimeInterval::setActualDay(uint32_t actualDayRaw) {
    time_t rawTime = ntpClient->getEpochTime();
    datetimeActual = localtime(&rawTime);
    this->actualDay = (uint8_t) datetimeActual->tm_mday;
}

void DatetimeInterval::setActualHours(uint8_t actualHours) { this->actualHours = actualHours; }

void DatetimeInterval::setActualMinutes(uint8_t actualMinutes) { this->actualMinutes = actualMinutes; }

void DatetimeInterval::setActualSeconds(uint8_t actualSeconds) { this->actualSeconds = actualSeconds; }

void DatetimeInterval::setNextHours(uint8_t nextHour) { this->nextHours = nextHour; }

void DatetimeInterval::setNextMinutes(uint8_t nextMinutes) { this->nextMinutes = nextMinutes; }

void DatetimeInterval::setNextSeconds(uint8_t nextSeconds) { this->nextSeconds = nextSeconds; }

uint16_t DatetimeInterval::getActualYear() { return this->actualYear; }

uint8_t DatetimeInterval::getActualMonth() { return this->actualMonth; }

uint8_t DatetimeInterval::getActualDay() { return this->actualDay; }

uint8_t DatetimeInterval::getHoursUpdate() { return this->hoursUpdate; }

uint8_t DatetimeInterval::getMinutesUpdate() { return this->minutesUpdate; }

uint8_t DatetimeInterval::getActualHours() { return this->actualHours; }

uint8_t DatetimeInterval::getActualMinutes() { return this->actualMinutes; }

uint8_t DatetimeInterval::getActualSeconds() { return this->actualSeconds; }

uint8_t DatetimeInterval::getNextHours() { return this->nextHours; }

uint8_t DatetimeInterval::getNextMinutes() { return this->nextMinutes; }

uint8_t DatetimeInterval::getNextSeconds() { return this->nextSeconds; }

bool DatetimeInterval::configActualDatetime() {
    if (WiFi.status() == WL_CONNECTED) {
        this->ntpClient->update();

        setActualYear(this->ntpClient->getEpochTime());
        setActualMonth(this->ntpClient->getEpochTime());
        setActualDay(this->ntpClient->getEpochTime());
        setActualHours(this->ntpClient->getHours());
        setActualMinutes(this->ntpClient->getMinutes());
        setActualSeconds(this->ntpClient->getSeconds());

        return true;
    } else {
        WiFi.reconnect();

        return false;
    }
}

bool DatetimeInterval::configNextTime() {
    if (configActualDatetime()) {
        uint8_t hours = getActualHours();
        uint8_t minutes = getActualMinutes();
        uint8_t seconds = getActualSeconds();

        hours += getHoursUpdate();
        if (hours > 23) {
            hours -= 23;
        }

        minutes += getMinutesUpdate();
        if (minutes > 59) {
            minutes -= 60;
            hours += 1;
        }

        setNextHours(hours);
        setNextMinutes(minutes);
        setNextSeconds(seconds);

        return true;
    } else {
        return false;
    }
}
