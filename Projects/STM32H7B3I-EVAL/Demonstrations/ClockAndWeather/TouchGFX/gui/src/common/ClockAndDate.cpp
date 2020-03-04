#include "gui/common/ClockAndDate.hpp"


uint16_t getWeekday(uint16_t year, uint16_t month, uint16_t day)
{
    uint16_t adjustment, mm, yy;

    adjustment = (14 - month) / 12;
    mm = month + 12 * adjustment - 2;
    yy = year - adjustment;
    return (day + (13 * mm - 1) / 5 +
        yy + yy / 4 - yy / 100 + yy / 400) % 7;
}

void getWeekdayName(int weekdayValue, Unicode::UnicodeChar *buffer, uint16_t bufferSize)
{
    switch (weekdayValue)
    {
    case 0:
        Unicode::strncpy(buffer, "SUN", bufferSize);
        break;
    case 1:
        Unicode::strncpy(buffer, "MON", bufferSize);
        break;
    case 2:
        Unicode::strncpy(buffer, "TUE", bufferSize);
        break;
    case 3:
        Unicode::strncpy(buffer, "WED", bufferSize);
        break;
    case 4:
        Unicode::strncpy(buffer, "THU", bufferSize);
        break;
    case 5:
        Unicode::strncpy(buffer, "FRI", bufferSize);
        break;
    case 6:
        Unicode::strncpy(buffer, "SAT", bufferSize);
        break;
    }
}

void getMonthName(int monthValue, Unicode::UnicodeChar *buffer, uint16_t bufferSize)
{
    switch (monthValue)
    {
    case 1:
        Unicode::strncpy(buffer, "JAN", bufferSize);
        break;
    case 2:
        Unicode::strncpy(buffer, "FEB", bufferSize);
        break;
    case 3:
        Unicode::strncpy(buffer, "MAR", bufferSize);
        break;
    case 4:
        Unicode::strncpy(buffer, "APR", bufferSize);
        break;
    case 5:
        Unicode::strncpy(buffer, "MAY", bufferSize);
        break;
    case 6:
        Unicode::strncpy(buffer, "JUN", bufferSize);
        break;
    case 7:
        Unicode::strncpy(buffer, "JUL", bufferSize);
        break;
    case 8:
        Unicode::strncpy(buffer, "AUG", bufferSize);
        break;
    case 9:
        Unicode::strncpy(buffer, "SEP", bufferSize);
        break;
    case 10:
        Unicode::strncpy(buffer, "OCT", bufferSize);
        break;
    case 11:
        Unicode::strncpy(buffer, "NOV", bufferSize);
        break;
    case 12:
        Unicode::strncpy(buffer, "DEC", bufferSize);
        break;
    }
}


void getClock(uint16_t hour, uint16_t minute, int32_t timezone, bool isMetric, Unicode::UnicodeChar *buffer, uint16_t bufferSize)
{
    uint16_t timezoneCompentation = 0;
    int minuteBuffer;
    int hourBuffer;

    if (((timezone / 60) % 60) == 0)
    {
        minuteBuffer = minute;
    }
    else
    {
        if ((minute + 30) > 60)
        {
            minuteBuffer = (minute + 30) - 60;
            timezoneCompentation = 1;
        }
        else
        {
            minuteBuffer = (minute + 30);
        }
    }

    int hourWithTimezone = hour + (timezone / 60) / 60 + timezoneCompentation;

    if (hourWithTimezone < 24 && hourWithTimezone >= 0)
    {
        hourBuffer = hourWithTimezone;
    }
    else if (hourWithTimezone >= 24)
    {
        hourBuffer = hourWithTimezone - 24;
    }
    else
    {
        hourBuffer = hourWithTimezone + 24;
    }

    if (isMetric)
    {
        Unicode::snprintf(buffer, bufferSize, "%02d:%02d", hourBuffer, minuteBuffer);
    }   
    else
    {
        if (hourBuffer < 12)
        {
            if (hourBuffer == 0)
            {
                Unicode::snprintf(buffer, bufferSize, "%02d:%02d AM", 12, minuteBuffer);
            }
            else
            {
              Unicode::snprintf(buffer, bufferSize, "%02d:%02d AM", hourBuffer, minuteBuffer);
            }
        }
        else
        {
            if (hourBuffer == 12)
            {
                Unicode::snprintf(buffer, bufferSize, "%02d:%02d PM", 12, minuteBuffer);
            }
            else
            {
              Unicode::snprintf(buffer, bufferSize, "%02d:%02d PM", hourBuffer - 12, minuteBuffer);
            }
        }        
    }
}
