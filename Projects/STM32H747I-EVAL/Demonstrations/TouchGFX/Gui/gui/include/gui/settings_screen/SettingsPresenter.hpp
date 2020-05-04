

#ifndef SETTINGS_PRESENTER_HPP
#define SETTINGS_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class SettingsView;

class SettingsPresenter : public DemoPresenter
{
public:
    SettingsPresenter(SettingsView& v);
    virtual ~SettingsPresenter() {};

    virtual void activate();
    virtual void deactivate();

    void clockSelected(int clockIndex);

    virtual void updateTime(uint8_t hour, uint8_t minute, uint8_t second);

    void stateUpdated(int clockIndex, uint8_t hour, uint8_t minute, uint8_t second, uint16_t year, uint8_t month, uint8_t day);

    virtual void oneSecond();
    virtual void screenSaverMinorTick();

private:
    SettingsPresenter();
    SettingsView& view;

    int16_t counter;
};

#endif // SETTINGS_PRESENTER_HPP
