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
