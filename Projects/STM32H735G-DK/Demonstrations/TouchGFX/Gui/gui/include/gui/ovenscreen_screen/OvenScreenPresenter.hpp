#ifndef OVENSCREEN_PRESENTER_HPP
#define OVENSCREEN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class OvenScreenView;

class OvenScreenPresenter : public Presenter, public ModelListener
{
public:
    OvenScreenPresenter(OvenScreenView& v);

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

    virtual ~OvenScreenPresenter() {};

    void setShowOven(bool show);
    bool getShowOven();
    bool getOvenEntered();

    void setSelectedOvenMode(uint16_t mode);
    uint16_t getSelectedOvenMode();
    void setOvenTimerHour(int16_t hour);
    int16_t getOvenTimerHour();
    void setOvenTimerMinute(int16_t minute);
    int16_t getOvenTimerMinute();
    void setOvenTemperature(uint16_t temp);
    uint16_t getOvenTemperature();

    virtual void mcuLoadUpdated(uint8_t mcuLoad);

    virtual void autoDemoTick();
    virtual void autoDemoStart();

    void setAutoDemoState(Model::AutoDemoStates state);
    Model::AutoDemoStates getAutoDemoState();

private:
    OvenScreenPresenter();

    OvenScreenView& view;

};


#endif // OVENSCREEN_PRESENTER_HPP
