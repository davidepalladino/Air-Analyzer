#include "DatetimeInterval.h"

DatetimeInterval::DatetimeInterval(int8_t timezone, uint8_t totalMinuteUpdate) {
    ntpClient = new NTPClient(*new WiFiUDP(), (timezone * 3600));

    if (totalMinuteUpdate > 240) {
        totalMinuteUpdate = 240;
    }

    updateHour = totalMinuteUpdate / 60;
    updateMinute = totalMinuteUpdate - (updateHour * 60);

    endTimeoutNTP = 0;
}

void DatetimeInterval::begin() {
    ntpClient->begin();

    if (ntpClient->update()) {
        translateDatetime(&actualDatetime, ntpClient->getEpochTime());
        translateDatetime(&nextDatetime, ntpClient->getEpochTime());

        endTimeoutNTP = millis() + TIMEOUT_CHECK_NTP;

        Serial.println("\033[1;92m[NTP UPDATED]\033[0m");
    } else {
        Serial.println("\033[1;91m[NTP ERROR]\033[0m");
    }
}

bool DatetimeInterval::checkTime() {
    bool result = false;    
    
    if (configActualDatetime()) {
        if (getActualYear() > nextDatetime.year) {
            result = true;
        } else if (getActualYear() == nextDatetime.year) {
            if (getActualMonth() > nextDatetime.month) {
                result = true;
            } else if (getActualMonth() == nextDatetime.month) {
                if (getActualDay() > nextDatetime.day) {
                    result = true;
                } else if (getActualDay() == nextDatetime.day) {
                    if (getActualHour() > nextDatetime.hour) {
                        result = true;
                    } else if (getActualHour() == nextDatetime.hour) {
                        if (getActualMinute() > nextDatetime.minute) {
                            result = true;
                        } else if (getActualMinute() == nextDatetime.minute) {
                            if (getActualSecond() > nextDatetime.second) {
                                result = true;
                            }
                        }
                    }
                }
            }
        }
    }
    
    return result;
}

uint16_t DatetimeInterval::getActualYear() { return actualDatetime.year; }

uint8_t DatetimeInterval::getActualMonth() { return actualDatetime.month; }

uint8_t DatetimeInterval::getActualDay() { return actualDatetime.day; }

uint8_t DatetimeInterval::getActualDayWeek() { return actualDatetime.wday; }

uint8_t DatetimeInterval::getActualHour() { return actualDatetime.hour; }

uint8_t DatetimeInterval::getActualMinute() { return actualDatetime.minute; }

uint8_t DatetimeInterval::getActualSecond() { return actualDatetime.second; }

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
    tempDatetime = localtime(&rawTime);

    dateTime->year = tempDatetime->tm_year + 1900;
    dateTime->month = tempDatetime->tm_mon + 1;
    dateTime->day = tempDatetime->tm_mday;
    dateTime->wday = tempDatetime->tm_wday;
    dateTime->hour = tempDatetime->tm_hour;
    dateTime->minute = tempDatetime->tm_min;
    dateTime->second = tempDatetime->tm_sec;
}

bool DatetimeInterval::configActualDatetime() {
    if (WiFi.status() == WL_CONNECTED) {
        /* 
         * There is a case where "timeout" will go to overflow and the result of "millis()" not. 
         * In this case the sensor will be read every time until the result of "millis()" will go to overflow, too. 
         */
        if ((long) (endTimeoutNTP - millis()) <= 0) {
            if (ntpClient->update()) {
                endTimeoutNTP = millis() + TIMEOUT_CHECK_NTP;
                translateDatetime(&actualDatetime, ntpClient->getEpochTime());

                //Serial.println("\033[1;92m[NTP UPDATED]\033[0m");
                return true;
            } else {
                Serial.println("\033[1;91m[NTP ERROR]\033[0m");
                return false;
            }
        } else {
            //Serial.println("\033[1;93m[NTP CHECK TOO FAST]\033[0m");
            return false;
        }
    } else {
        Serial.println("\033[1;91m[WIFI ERROR FROM DatetimeInterval]\033[0m");
        return false;
    }
}

void DatetimeInterval::configNextDatetime() {
    uint16_t year = getActualYear();
    uint8_t month = getActualMonth();
    uint8_t day = getActualDay();
    uint8_t wday = getActualDayWeek();
    uint8_t hour = getActualHour();
    uint8_t minute = getActualMinute();
    uint8_t seconds = getActualSecond();

    minute += updateMinute;
    if (minute > 59) {
        minute -= 60;
        hour += 1;
    }

    hour += updateHour;
    if (hour > 23) {
        hour -= 24;
        day += 1;
        wday += 1;
    }

    if (wday > 6) {
        wday = 0;
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

    nextDatetime.year = year;
    nextDatetime.month = month;
    nextDatetime.day = day;
    nextDatetime.wday = wday;
    nextDatetime.hour = hour;
    nextDatetime.minute = minute;
    nextDatetime.second = seconds;

    Serial.println("\033[1;92m[TIME]\033[0m");
    Serial.print("\t\033[1;97mACTUAL: "); 
    switch (getActualDayWeek()) {
        case 0:
            Serial.print("SUN - ");
            break;
        case 1:
            Serial.print("MON - ");
            break;
        case 2:
            Serial.print("TUE - ");
            break;
        case 3:
            Serial.print("WED - ");
            break;
        case 4:
            Serial.print("THU - ");
            break;
        case 5:
            Serial.print("FRI - ");
            break;
        case 6:
            Serial.print("SAT - ");
            break;
    }
    Serial.print(getActualYear()); Serial.print("-"); Serial.print(getActualMonth()); Serial.print("-"); Serial.print(getActualDay()); Serial.print(" "); 
        Serial.print(getActualHour()); Serial.print(":"); Serial.print(getActualMinute()); Serial.print(":"); Serial.print(getActualSecond()); Serial.println("\033[0m");
    Serial.print("\t\033[1;97mNEXT:   ");
    switch (nextDatetime.wday) {
        case 0:
            Serial.print("SUN - ");
            break;
        case 1:
            Serial.print("MON - ");
            break;
        case 2:
            Serial.print("TUE - ");
            break;
        case 3:
            Serial.print("WED - ");
            break;
        case 4:
            Serial.print("THU - ");
            break;
        case 5:
            Serial.print("FRI - ");
            break;
        case 6:
            Serial.print("SAT - ");
            break;
    }
    Serial.print(nextDatetime.year); Serial.print("-"); Serial.print(nextDatetime.month); Serial.print("-"); Serial.print(nextDatetime.day); Serial.print(" ");
        Serial.print(nextDatetime.hour); Serial.print(":"); Serial.print(nextDatetime.minute); Serial.print(":"); Serial.print(nextDatetime.second);  Serial.println("\033[0m");
    Serial.println("\033[1;92m---------------------------------------------------\033[0m");
}