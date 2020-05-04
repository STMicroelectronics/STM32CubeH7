#include <gui/settings_screen/TimeSettingContainer.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

TimeSettingContainer::TimeSettingContainer() :
    ampmSelectedCallback(this, &TimeSettingContainer::ampmSelectedHandler),
    digitSelectedCallback(this, &TimeSettingContainer::digitSelectedHandler),
    timeSettingUpdatedCallback(this, &TimeSettingContainer::timeSettingUpdatedHandler)
{
    clock.setXY(42, 38);
    clock.setVolumeChangedCallback(timeSettingUpdatedCallback);
    add(clock);

    int circleSpace = 30;
    digitalClockHourCircle.setBitmap(Bitmap(BITMAP_CLOCK_SETTINGS_CIRCLE_ID));
    digitalClockHourCircle.setXY(298, 159 - digitalClockHourCircle.getHeight() / 2);
    add(digitalClockHourCircle);
    digitalClockHourTextBuffer[0] = 0;
    digitalClockHourText.setTypedText(TypedText(T_SETTINGS_CLOCK_DIGIT));
    digitalClockHourText.setWildcard(digitalClockHourTextBuffer);
    digitalClockHourText.setColor(Color::getColorFrom24BitRGB(0x0, 0x0, 0x34));
    digitalClockHourText.setPosition(digitalClockHourCircle.getX(), 128, digitalClockHourCircle.getWidth(), 52);
    digitalClockHourText.setClickAction(digitSelectedCallback);
    add(digitalClockHourText);

    digitalClockMinuteCircle.setBitmap(Bitmap(BITMAP_CLOCK_SETTINGS_CIRCLE_ID));
    digitalClockMinuteCircle.setXY(digitalClockHourCircle.getRect().right() + circleSpace, digitalClockHourCircle.getY());
    add(digitalClockMinuteCircle);
    digitalClockMinuteTextBuffer[0] = 0;
    digitalClockMinuteText.setTypedText(TypedText(T_SETTINGS_CLOCK_DIGIT));
    digitalClockMinuteText.setWildcard(digitalClockMinuteTextBuffer);
    digitalClockMinuteText.setColor(Color::getColorFrom24BitRGB(0x0, 0x0, 0x34));
    digitalClockMinuteText.setPosition(digitalClockMinuteCircle.getX(), digitalClockHourText.getY(), digitalClockMinuteCircle.getWidth(), digitalClockHourText.getHeight());
    digitalClockMinuteText.setClickAction(digitSelectedCallback);
    add(digitalClockMinuteText);

    digitalClockSecondCircle.setBitmap(Bitmap(BITMAP_CLOCK_SETTINGS_CIRCLE_ID));
    digitalClockSecondCircle.setXY(digitalClockMinuteCircle.getRect().right() + circleSpace, digitalClockHourCircle.getY());
    add(digitalClockSecondCircle);
    digitalClockSecondTextBuffer[0] = 0;
    digitalClockSecondText.setTypedText(TypedText(T_SETTINGS_CLOCK_DIGIT));
    digitalClockSecondText.setWildcard(digitalClockSecondTextBuffer);
    digitalClockSecondText.setColor(Color::getColorFrom24BitRGB(0x0, 0x0, 0x0));
    digitalClockSecondText.setPosition(digitalClockSecondCircle.getX(), digitalClockHourText.getY(), digitalClockSecondCircle.getWidth(), digitalClockHourText.getHeight());
    digitalClockSecondText.setClickAction(digitSelectedCallback);
    add(digitalClockSecondText);

    for (int i = 0; i < 2; i++)
    {
        colon[i].setBitmap(Bitmap(BITMAP_CLOCK_SETTINGS_COLON_ID));
        colon[i].setXY(digitalClockHourCircle.getX() + (digitalClockHourCircle.getWidth() * (i + 1)) + (circleSpace * i) + 10, digitalClockHourCircle.getY() + 26);
        add(colon[i]);
    }

    ampm.setBitmap(Bitmap(BITMAP_CLOCK_SETTINGS_AM_ID));
    ampm.setXY(digitalClockHourCircle.getX() + 102, 30);
    ampm.setClickAction(ampmSelectedCallback);
    add(ampm);

    setActive(false);
}

TimeSettingContainer::~TimeSettingContainer()
{
}

void TimeSettingContainer::updateTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    if (!isActive)
    {
        clock.updateTime(hour, minute, second);
        setDigitalClock(hour, minute, second);
    }
}

void TimeSettingContainer::initializeTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    if (!isActive)
    {
        clock.initializeTime(hour % 12, minute, second);
        setDigitalClock(hour, minute, second);
    }
}

void TimeSettingContainer::getTime(uint8_t& h, uint8_t& m, uint8_t& s)
{
    h = hour + (is_pm ? 12 : 0);
    m = minute;
    s = second;
}

void TimeSettingContainer::setActive(bool active)
{
    isActive = active;

    if (isActive)
    {
        clock.useHourHandMinuteCorrection(false);
        clock.useMinuteHandSecondCorrection(false);
        clock.useAnimation(0);
        setUpdateMode(AnalogClockWithSetTimeFunctionality::HOUR_UPDATE);
    }
}

void TimeSettingContainer::ampmSelectedHandler(const Image& img, const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        if (img.getBitmap() == BITMAP_CLOCK_SETTINGS_AM_ID)
        {
            ampm.setBitmap(Bitmap(BITMAP_CLOCK_SETTINGS_PM_ID));
            is_pm = true;
        }
        else
        {
            ampm.setBitmap(Bitmap(BITMAP_CLOCK_SETTINGS_AM_ID));
            is_pm = false;
        }
        ampm.invalidate();
    }
}

void TimeSettingContainer::digitSelectedHandler(const TextAreaWithOneWildcard& text, const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        if (&text == &digitalClockHourText)
        {
            setUpdateMode(AnalogClockWithSetTimeFunctionality::HOUR_UPDATE);
        }
        else if (&text == &digitalClockMinuteText)
        {
            setUpdateMode(AnalogClockWithSetTimeFunctionality::MINUTE_UPDATE);
        }
        else if (&text == &digitalClockSecondText)
        {
            setUpdateMode(AnalogClockWithSetTimeFunctionality::SECOND_UPDATE);
        }
    }
}

void TimeSettingContainer::timeSettingUpdatedHandler(AnalogClockWithSetTimeFunctionality::UpdateMode updateMode, uint8_t newValue)
{
    switch (updateMode)
    {
    case AnalogClockWithSetTimeFunctionality::HOUR_UPDATE:
        Unicode::snprintf(digitalClockHourTextBuffer, 3, "%02d", (newValue + 11) % 12 + 1);
        hour = newValue % 12;
        digitalClockHourText.invalidate();
        break;
    case AnalogClockWithSetTimeFunctionality::MINUTE_UPDATE:
        Unicode::snprintf(digitalClockMinuteTextBuffer, 3, "%02d", newValue);
        minute = newValue;
        digitalClockMinuteText.invalidate();
        break;
    case AnalogClockWithSetTimeFunctionality::SECOND_UPDATE:
        Unicode::snprintf(digitalClockSecondTextBuffer, 3, "%02d", newValue);
        second = newValue;
        digitalClockSecondText.invalidate();
        break;
    default:
        break;
    }
}

void TimeSettingContainer::setDigitalClock(uint8_t hour, uint8_t minute, uint8_t second)
{
    if (hour > 0 && hour <= 12)
    {
        ampm.setBitmap(Bitmap(BITMAP_CLOCK_SETTINGS_AM_ID));
        is_pm = false;
    }
    else
    {
        ampm.setBitmap(Bitmap(BITMAP_CLOCK_SETTINGS_PM_ID));
        is_pm = true;
    }
    ampm.invalidate();
    timeSettingUpdatedHandler(AnalogClockWithSetTimeFunctionality::HOUR_UPDATE, hour % 12);
    timeSettingUpdatedHandler(AnalogClockWithSetTimeFunctionality::MINUTE_UPDATE, minute);
    timeSettingUpdatedHandler(AnalogClockWithSetTimeFunctionality::SECOND_UPDATE, second);
}

void TimeSettingContainer::setUpdateMode(AnalogClockWithSetTimeFunctionality::UpdateMode updateMode)
{
    digitalClockHourText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    digitalClockMinuteText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    digitalClockSecondText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));

    digitalClockHourCircle.setBitmap(Bitmap(BITMAP_CLOCK_SETTINGS_CIRCLE_ID));
    digitalClockMinuteCircle.setBitmap(Bitmap(BITMAP_CLOCK_SETTINGS_CIRCLE_ID));
    digitalClockSecondCircle.setBitmap(Bitmap(BITMAP_CLOCK_SETTINGS_CIRCLE_ID));

    clock.setUpdateMode(updateMode);

    switch (updateMode)
    {
    case AnalogClockWithSetTimeFunctionality::HOUR_UPDATE:
        digitalClockHourCircle.setBitmap(Bitmap(BITMAP_CLOCK_SETTINGS_CIRCLESELECTED_ID));
        digitalClockHourText.setColor(Color::getColorFrom24BitRGB(0x0, 0xAE, 0xFF));
        break;
    case AnalogClockWithSetTimeFunctionality::MINUTE_UPDATE:
        digitalClockMinuteCircle.setBitmap(Bitmap(BITMAP_CLOCK_SETTINGS_CIRCLESELECTED_ID));
        digitalClockMinuteText.setColor(Color::getColorFrom24BitRGB(0x0, 0xAE, 0xFF));
        break;
    case AnalogClockWithSetTimeFunctionality::SECOND_UPDATE:
        digitalClockSecondCircle.setBitmap(Bitmap(BITMAP_CLOCK_SETTINGS_CIRCLESELECTED_ID));
        digitalClockSecondText.setColor(Color::getColorFrom24BitRGB(0x0, 0xAE, 0xFF));
        break;
    default:
        break;
    }

    digitalClockHourCircle.invalidate();
    digitalClockMinuteCircle.invalidate();
    digitalClockSecondCircle.invalidate();
}

