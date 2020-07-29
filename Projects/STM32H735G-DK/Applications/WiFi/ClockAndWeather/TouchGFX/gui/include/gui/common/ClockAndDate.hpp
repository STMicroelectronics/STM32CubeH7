#ifndef CLOCKANDDATE_HPP
#define CLOCKANDDATE_HPP

#include <gui/common/FrontendApplication.hpp>

uint16_t getWeekday(uint16_t year, uint16_t month, uint16_t day);
void getWeekdayName(int weekdayValue, Unicode::UnicodeChar *buffer, uint16_t bufferSize);
void getMonthName(int monthValue, Unicode::UnicodeChar *buffer, uint16_t bufferSize);
void getClock(uint16_t hour, uint16_t minute, int32_t timezone, bool isMetric, Unicode::UnicodeChar *buffer, uint16_t bufferSize);

#endif // CLOCKANDDATE_HPP
