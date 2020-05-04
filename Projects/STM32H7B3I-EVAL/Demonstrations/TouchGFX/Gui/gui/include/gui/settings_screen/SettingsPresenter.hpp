#ifndef SETTINGS_PRESENTER_HPP
#define SETTINGS_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SettingsView;

class SettingsPresenter : public Presenter, public ModelListener
{
public:
    SettingsPresenter(SettingsView& v);
    virtual ~SettingsPresenter() {};

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    void clockSelected(int clockIndex);
    void updateTime(uint8_t hour, uint8_t minute, uint8_t second);
    void stateUpdated(int clockIndex, uint8_t hour, uint8_t minute, uint8_t second, uint16_t year, uint8_t month, uint8_t day);

    void gotoMenuSelected();
    void mcuLoadUpdated(uint8_t newMcuLoad);
    void screenSaverMinorTick();

private:
    SettingsPresenter();

    SettingsView& view;
};

#endif // SETTINGS_PRESENTER_HPP
