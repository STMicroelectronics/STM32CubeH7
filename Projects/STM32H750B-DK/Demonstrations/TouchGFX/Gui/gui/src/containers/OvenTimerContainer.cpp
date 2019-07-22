#include <gui/containers/OvenTimerContainer.hpp>

OvenTimerContainer::OvenTimerContainer()
{
}

void OvenTimerContainer::initialize()
{
    OvenTimerContainerBase::initialize();
}

void OvenTimerContainer::setAlpha(uint8_t alpha)
{
    backgroundTimer.setAlpha(alpha);
    imageHourSelectedItem.setAlpha(alpha);
    imageMinuteSelectedItem.setAlpha(alpha);
    imageGradientHour.setAlpha(alpha);
    imageGradientMinutes.setAlpha(alpha);

    for (int cnt = 0; cnt < scrollWheelHoursListItems.getNumberOfDrawables(); cnt++)
    {
        scrollWheelHoursListItems[cnt].setAlpha(alpha);
    }
    for (int cnt = 0; cnt < scrollWheelHoursSelectedListItems.getNumberOfDrawables(); cnt++)
    {
        scrollWheelHoursSelectedListItems[cnt].setAlpha(alpha);
    }

    for (int cnt = 0; cnt < scrollWheelMinutesListItems.getNumberOfDrawables(); cnt++)
    {
        scrollWheelMinutesListItems[cnt].setAlpha(alpha);
    }
    for (int cnt = 0; cnt < scrollWheelMinutesSelectedListItems.getNumberOfDrawables(); cnt++)
    {
        scrollWheelMinutesSelectedListItems[cnt].setAlpha(alpha);
    }
}

uint8_t OvenTimerContainer::getAlpha()
{
    return backgroundTimer.getAlpha();
}

void OvenTimerContainer::setWheelsValue(int16_t hour, int16_t minute)
{
    scrollWheelHours.animateToItem(hour + 5, 0);
    scrollWheelHours.animateToItem(hour, 0);
    scrollWheelMinutes.animateToItem(minute, 0);
}

int16_t OvenTimerContainer::getSelectedHour()
{
    return scrollWheelHours.getSelectedItem();
}

int16_t OvenTimerContainer::getSelectedMinute()
{
    return scrollWheelMinutes.getSelectedItem();
}

void OvenTimerContainer::animateHourWheelToItem(int16_t item, int16_t steps)
{
    scrollWheelHours.animateToItem(item, steps);
}

void OvenTimerContainer::animateMinuteWheelToItem(int16_t item, int16_t steps)
{
    scrollWheelMinutes.animateToItem(item, steps);
}

void OvenTimerContainer::scrollWheelHoursUpdateItem(OvenTimerWheelElement& item, int16_t itemIndex)
{
    item.updateText(itemIndex);
}

void OvenTimerContainer::scrollWheelHoursUpdateCenterItem(OvenTimerWheelCenterElement& item, int16_t itemIndex)
{
    item.updateText(itemIndex);
}

void OvenTimerContainer::scrollWheelMinutesUpdateItem(OvenTimerWheelElement& item, int16_t itemIndex)
{
    item.updateText(itemIndex);
}

void OvenTimerContainer::scrollWheelMinutesUpdateCenterItem(OvenTimerWheelCenterElement& item, int16_t itemIndex)
{
    item.updateText(itemIndex);
}
