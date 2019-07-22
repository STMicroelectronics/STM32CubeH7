

#include <gui/settings_screen/DateSelectorContainer.hpp>


DateSelectorContainer::DateSelectorContainer()
{
    datePicker.setXY(100, 56);
    add(datePicker);
}

DateSelectorContainer::~DateSelectorContainer()
{
}

void DateSelectorContainer::setDate(uint16_t year, uint8_t month, uint8_t day)
{
    datePicker.setDay(day, 16, EasingEquations::backEaseOut);
    datePicker.setMonth(month, 16, EasingEquations::backEaseOut);
    datePicker.setYear(year, 16, EasingEquations::backEaseOut);
}

void DateSelectorContainer::setYear(uint16_t year, uint16_t duration)
{
    datePicker.setYear(year, duration, EasingEquations::backEaseOut);
}
void DateSelectorContainer::setMonth(uint8_t month, uint16_t duration)
{
    datePicker.setMonth(month, duration, EasingEquations::backEaseOut);
}

void DateSelectorContainer::setDay(uint8_t day, uint16_t duration)
{
    datePicker.setDay(day, duration, EasingEquations::backEaseOut);
}

uint16_t DateSelectorContainer::getYear()
{
    return datePicker.getYear();
}

uint8_t DateSelectorContainer::getMonth()
{
    return datePicker.getMonth();
}

uint8_t DateSelectorContainer::getDay()
{
    return datePicker.getDay();
}
