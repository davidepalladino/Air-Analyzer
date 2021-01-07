#include "DatetimeInterval.h"

DatetimeInterval::DatetimeInterval(int8_t timezone, uint8_t totalMinuteUpdate) {
    this->ntpClient = new NTPClient(*new WiFiUDP(), (timezone * 3600));

    if (totalMinuteUpdate > 240) {
        totalMinuteUpdate = 240;
    }

    this->updateHour = totalMinuteUpdate / 60;
    this->updateMinute = totalMinuteUpdate - (this->updateHour * 60);
}

void DatetimeInterval::begin() {
    this->ntpClient->begin();

    if (WiFi.status() == WL_CONNECTED) {
        if (this->ntpClient->update()) {
            translateDatetime(&this->actualDatetime, this->ntpClient->getEpochTime());
            translateDatetime(&this->nextDatetime, this->ntpClient->getEpochTime());

        } else {
            Serial.println("\033[1;91m[ERROR NTP]\033[0m");
        }
    } else {
        Serial.println("\033[1;91m[ERROR WiFi]\033[0m");
        WiFi.reconnect();
    }
}

bool DatetimeInterval::checkTime() {
    bool result = false;    
    
    if (configActualDatetime()) {
        if (getActualYear() > this->nextDatetime.year) {
            result = true;
        } else if (getActualYear() == this->nextDatetime.year) {
            if (getActualMonth() > this->nextDatetime.month) {
                result = true;
            } else if (getActualMonth() == this->nextDatetime.month) {
                if (getActualDay() > this->nextDatetime.day) {
                    result = true;
                } else if (getActualDay() == this->nextDatetime.day) {
                    if (getActualHour() > this->nextDatetime.hour) {
                        result = true;
                    } else if (getActualHour() == this->nextDatetime.hour) {
                        if (getActualMinute() > this->nextDatetime.minute) {
                            result = true;
                        } else if (getActualMinute() == this->nextDatetime.minute) {
                            if (getActualSecond() > this->nextDatetime.second) {
                                result = true;
                            }
                        }
                    }
                }
            }
        }
    }

    if (result) {
        configNextDatetime();
        Serial.println("\033[1;92m------------------ [TRANSACTION] ------------------\033[0m");
        Serial.println("\033[1;92m[TIME]\033[0m");
        Serial.print("\t\033[1;97mACTUAL: "); Serial.print(getActualYear()); Serial.print("-"); Serial.print(getActualMonth()); Serial.print("-"); Serial.print(getActualDay()); Serial.print(" "); 
            Serial.print(getActualHour()); Serial.print(":"); Serial.print(getActualMinute()); Serial.print(":"); Serial.print(getActualSecond()); Serial.println("\033[0m");
        Serial.print("\t\033[1;97mNEXT:   "); Serial.print(this->nextDatetime.year); Serial.print("-"); Serial.print(this->nextDatetime.month); Serial.print("-"); Serial.print(this->nextDatetime.day); Serial.print(" ");
            Serial.print(this->nextDatetime.hour); Serial.print(":"); Serial.print(this->nextDatetime.minute); Serial.print(":"); Serial.print(this->nextDatetime.second);  Serial.println("\033[0m");
    }
    
    return result;
}

uint16_t DatetimeInterval::getActualYear() { return this->actualDatetime.year; }

uint8_t DatetimeInterval::getActualMonth() { return this->actualDatetime.month; }

uint8_t DatetimeInterval::getActualDay() { return this->actualDatetime.day; }

uint8_t DatetimeInterval::getActualHour() { return this->actualDatetime.hour; }

uint8_t DatetimeInterval::getActualMinute() { return this->actualDatetime.minute; }

uint8_t DatetimeInterval::getActualSecond() { return this->actualDatetime.second; }

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

void DatetimeInterval::translateDatetime(datetime_t* dateTime, uint32_t raw) {
    time_t rawTime = raw;
    this->tempDatetime = localtime(&rawTime);

    dateTime->year = this->tempDatetime->tm_year + 1900;
    dateTime->month = this->tempDatetime->tm_mon + 1;
    dateTime->day = this->tempDatetime->tm_mday;
    dateTime->hour = this->tempDatetime->tm_hour;
    dateTime->minute = this->tempDatetime->tm_min;
    dateTime->second = this->tempDatetime->tm_sec;
}

bool DatetimeInterval::configActualDatetime() {
    if (WiFi.status() == WL_CONNECTED) {
        if (this->ntpClient->update()) {
            translateDatetime(&this->actualDatetime, this->ntpClient->getEpochTime());
            return true;
        } else {
            Serial.println("\033[1;91m[ERROR NTP]\033[0m");
            return false;
        }
    } else {
        Serial.println("\033[1;91m[ERROR WiFi]\033[0m");
        WiFi.reconnect();
        return false;
    }
}

void DatetimeInterval::configNextDatetime() {
    uint16_t year = getActualYear();
    uint8_t month = getActualMonth();
    uint8_t day = getActualDay();
    uint8_t hour = getActualHour();
    uint8_t minute = getActualMinute();
    uint8_t seconds = getActualSecond();

    minute += this->updateMinute;
    if (minute > 59) {
        minute -= 60;
        hour += 1;
    }

    hour += this->updateHour;
    if (hour > 23) {
        hour -= 24;
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
}