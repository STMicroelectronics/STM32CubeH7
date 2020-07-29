#ifndef EXTRACTORHOODSCREEN_PRESENTER_HPP
#define EXTRACTORHOODSCREEN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ExtractorHoodScreenView;

class ExtractorHoodScreenPresenter : public Presenter, public ModelListener
{
public:
    ExtractorHoodScreenPresenter(ExtractorHoodScreenView& v);

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

    virtual ~ExtractorHoodScreenPresenter() {};

    void setShowLight(bool show);
    bool getShowLight();

    void setFanState(bool state);
    bool getFanState();
    void setFanSpeed(uint16_t speed);
    uint16_t getFanSpeed();

    virtual void mcuLoadUpdated(uint8_t mcuLoad);

    virtual void autoDemoTick();
    virtual void autoDemoStart();

    void setAutoDemoState(Model::AutoDemoStates state);
    Model::AutoDemoStates getAutoDemoState();

private:
    ExtractorHoodScreenPresenter();

    ExtractorHoodScreenView& view;
};


#endif // EXTRACTORHOODSCREEN_PRESENTER_HPP
