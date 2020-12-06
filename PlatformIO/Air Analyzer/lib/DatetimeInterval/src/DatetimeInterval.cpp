#include "DatetimeInterval.h"

DatetimeInterval::DatetimeInterval(int8_t timezone, uint8_t totalMinuteUpdate) {
    this->ntpClient = new NTPClient(*new WiFiUDP(), (timezone * 3600));

    setUpdateHour(totalMinuteUpdate / 60);
    setUpdateMinute(totalMinuteUpdate - (getUpdateHour() * 60));
}

void DatetimeInterval::begin() {
    this->ntpClient->begin();

    if (WiFi.status() == WL_CONNECTED) {
        this->ntpClient->update();
        
        setDatetimeRaw(&this->actualDatetime, this->ntpClient->getEpochTime());
        setDatetimeRaw(&this->nextDatetime, this->ntpClient->getEpochTime());
    } else {
        WiFi.reconnect();
    }
}

bool DatetimeInterval::checkTime() {
    bool result = false;    
    
    if (configActualDatetime()) {
        if (getActualHour() > getNextHour()) {
            result = true;
        } else if ((getActualHour() == getNextHour()) && (getActualMinute() > getNextMinute())) {
            result = true;
        } else if ((getActualHour() == getNextHour()) && (getActualMinute() == getNextMinute()) && (getActualSecond() > getNextSecond())) {
            result = true;
        }
    }

    if (result) {
        configNextDatetime();
    }
    
    return result;
}

uint16_t DatetimeInterval::getActualYear() { return this->actualDatetime.year; }

uint8_t DatetimeInterval::getActualMonth() { return this->actualDatetime.month; }

uint8_t DatetimeInterval::getActualDay() { return this->actualDatetime.day; }

uint8_t DatetimeInterval::getActualHour() { return this->actualDatetime.hour; }

uint8_t DatetimeInterval::getActualMinute() { return this->actualDatetime.minute; }

uint8_t DatetimeInterval::getActualSecond() { return this->actualDatetime.second; }

void DatetimeInterval::setUpdateHour(uint8_t updateHour) { this->updateHour = updateHour; }

void DatetimeInterval::setUpdateMinute(uint8_t updateMinute) { this->updateMinute = updateMinute; }

void DatetimeInterval::setDatetimeRaw(datetime_t* dateTime, uint32_t raw) {
    time_t rawTime = raw;
    this->tempDatetime = localtime(&rawTime);

    dateTime->year = this->tempDatetime->tm_year + 1900;
    dateTime->month = this->tempDatetime->tm_mon + 1;
    dateTime->day = this->tempDatetime->tm_mday;
    dateTime->hour = this->tempDatetime->tm_hour;
    dateTime->minute = this->tempDatetime->tm_min;
    dateTime->second = this->tempDatetime->tm_sec;
}

uint8_t DatetimeInterval::getUpdateHour() { return this->updateHour; }

uint8_t DatetimeInterval::getUpdateMinute() { return this->updateMinute; }

uint16_t DatetimeInterval::getNextYear() { return this->nextDatetime.year; }

uint8_t DatetimeInterval::getNextMonth() { return this->nextDatetime.month; }

uint8_t DatetimeInterval::getNextDay() { return this->nextDatetime.day; }

uint8_t DatetimeInterval::getNextHour() { return this->nextDatetime.hour; }

uint8_t DatetimeInterval::getNextMinute() { return this->nextDatetime.minute; }

uint8_t DatetimeInterval::getNextSecond() { return this->nextDatetime.second; }

uint8_t DatetimeInterval::getMaxDay(uint8_t year, uint8_t month) {
	int maxDay = 0;

	switch (month) {
	    case 4:
	    case 6:
	    case 9:
	    case 11:
	    	maxDay = 30;
	       	break;

	    case 2:
	        maxDay = 28;

	        if (year % 4 == 0)
	        {
	        	maxDay++;
	        }
	        break;

	     default:
	    	 maxDay = 31;
	         break;
	}

	return maxDay;
}

bool DatetimeInterval::configActualDatetime() {
    if (WiFi.status() == WL_CONNECTED) {
        this->ntpClient->update();
        setDatetimeRaw(&this->actualDatetime, this->ntpClient->getEpochTime());

        return true;
    } else {
        WiFi.reconnect();

        return false;
    }
}

bool DatetimeInterval::configNextDatetime() {
    if (configActualDatetime()) {
        uint16_t year = getActualYear();
        uint8_t month = getActualMonth();
        uint8_t day = getActualDay();
        uint8_t hour = getActualHour();
        uint8_t minute = getActualMinute();
        uint8_t seconds = getActualSecond();

        minute += getUpdateMinute();
        if (minute > 59) {
            minute -= 60;
            hour += 1;
        }

        hour += getUpdateHour();
        if (hour > 23) {
            hour -= 23;
            day += 1;
        }

        uint8_t maxDay = getMaxDay(year, month);
        if (day > maxDay) {
            day -= maxDay;
            month += 1;
        }

        if (month > 12) {
            month -= 12;
            year += 1;
        }

        this->nextDatetime.year = year;
        this->nextDatetime.month = month;
        this->nextDatetime.day = day;
        this->nextDatetime.hour = hour;
        this->nextDatetime.minute = minute;
        this->nextDatetime.second = seconds;

        return true;
    } else {
        return false;
    }
}