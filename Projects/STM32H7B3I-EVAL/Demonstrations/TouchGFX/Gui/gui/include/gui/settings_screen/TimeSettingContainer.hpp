#ifndef TIME_SETTING_CONTAINER
#define TIME_SETTING_CONTAINER

#include <touchgfx/containers/Container.hpp>
#include <gui/settings_screen/AnalogClockWithSetTimeFunctionality.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/mixins/ClickListener.hpp>

using namespace touchgfx;

class TimeSettingContainer : public Container
{
public:
    TimeSettingContainer();
    virtual ~TimeSettingContainer();

    AnalogClock* getClock()
    {
        return &clock;
    }
    void setActive(bool active);

    void updateTime(uint8_t hour, uint8_t minute, uint8_t second);
    void initializeTime(uint8_t hour, uint8_t minute, uint8_t second);
    void getTime(uint8_t& hour, uint8_t& minute, uint8_t& second);

private:
    bool isActive;

    AnalogClockWithSetTimeFunctionality clock;

    ClickListener<TextAreaWithOneWildcard> digitalClockHourText;
    Unicode::UnicodeChar digitalClockHourTextBuffer[3];
    Image digitalClockHourCircle;
    ClickListener<TextAreaWithOneWildcard> digitalClockMinuteText;
    Unicode::UnicodeChar digitalClockMinuteTextBuffer[3];
    Image digitalClockMinuteCircle;
    ClickListener<TextAreaWithOneWildcard> digitalClockSecondText;
    Unicode::UnicodeChar digitalClockSecondTextBuffer[3];
    Image digitalClockSecondCircle;

    Image colon[2];
    ClickListener<Image> ampm;

    Callback<TimeSettingContainer, const Image&, const ClickEvent&> ampmSelectedCallback;
    void ampmSelectedHandler(const Image& c, const ClickEvent& evt);

    Callback<TimeSettingContainer, const TextAreaWithOneWildcard&, const ClickEvent&> digitSelectedCallback;
    void digitSelectedHandler(const TextAreaWithOneWildcard& c, const ClickEvent& evt);

    Callback<TimeSettingContainer, AnalogClockWithSetTimeFunctionality::UpdateMode, uint8_t> timeSettingUpdatedCallback;
    void timeSettingUpdatedHandler(AnalogClockWithSetTimeFunctionality::UpdateMode updateMode, uint8_t newValue);

    void setDigitalClock(uint8_t hour, uint8_t minute, uint8_t second);
    void setUpdateMode(AnalogClockWithSetTimeFunctionality::UpdateMode updateMode);
    uint8_t hour, minute, second;
    bool is_pm;
};

#endif /* TIME_SETTING_CONTAINER */
