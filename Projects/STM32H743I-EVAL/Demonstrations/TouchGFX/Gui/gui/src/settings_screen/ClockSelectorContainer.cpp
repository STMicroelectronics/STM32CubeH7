#include <gui/settings_screen/ClockSelectorContainer.hpp>
#include <BitmapDatabase.hpp>

ClockSelectorContainer::ClockSelectorContainer() :
    clockSelectedCallback(this, &ClockSelectorContainer::clockSelectedHandler),
    selectedClockChangedCallback(0),
    selectedClockIndex(-1)
{
    for (int i = 0; i < NUMBER_OF_CLOCK_SELECTIONS; i++)
    {
        clockSelectors[i].setXY((60 * (i + 1)) + (i * 232), 38);
        clockSelectors[i].setClickAction(clockSelectedCallback);
        add(clockSelectors[i]);
    }

    clockSelectedIndicator.setBitmap(Bitmap(BITMAP_BLUE_GLOW_ID));
    add(clockSelectedIndicator);
}

ClockSelectorContainer::~ClockSelectorContainer()
{
}

AnalogClock* ClockSelectorContainer::getClock(int index)
{
    if (index < NUMBER_OF_CLOCK_SELECTIONS)
    {
        return &(clockSelectors[index]);
    }
    return 0;
}

void ClockSelectorContainer::clockSelectedHandler(const AnalogClock& c, const ClickEvent& evt)
{
    bool wasPressed = pressed;
    pressed = (evt.getType() == ClickEvent::PRESSED);
    if (wasPressed && evt.getType() == ClickEvent::RELEASED)
    {
        int index = (&c == &clockSelectors[0]) ? 0 : (&c == &clockSelectors[1]) ? 1 : 2;
        setSelectedClock(index);
    }
}

void ClockSelectorContainer::updateTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    for (int i = 0; i < NUMBER_OF_CLOCK_SELECTIONS; i++)
    {
        clockSelectors[i].updateTime(hour, minute, second);
    }
}

void ClockSelectorContainer::initializeTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    for (int i = 0; i < NUMBER_OF_CLOCK_SELECTIONS; i++)
    {
        clockSelectors[i].initializeTime(hour, minute, second);
    }
}

void ClockSelectorContainer::setSelectedClock(int clockIndex)
{
    if (clockIndex < NUMBER_OF_CLOCK_SELECTIONS)
    {
        if (selectedClockIndex != clockIndex)
        {
            selectedClockIndex = clockIndex;
            clockSelectedIndicator.moveTo(clockSelectors[selectedClockIndex].getX() - 20, clockSelectors[selectedClockIndex].getY() - 20);

            if (selectedClockChangedCallback != 0 && selectedClockChangedCallback->isValid())
            {
                selectedClockChangedCallback->execute(*this, selectedClockIndex);
            }
        }

    }
}
