#ifndef CONTROLS_PRESENTER_HPP
#define CONTROLS_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ControlsView;

class ControlsPresenter : public Presenter, public ModelListener
{
public:
    ControlsPresenter(ControlsView& v);

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

    virtual ~ControlsPresenter() {};

    void gotoMenuSelected();
    void mcuLoadUpdated(uint8_t newMcuLoad);
    void screenSaverMinorTick();

    void newPotentiometerValue(int value);
    void newBrightnessValue(int value);

private:
    ControlsPresenter();

    ControlsView& view;

    uint8_t origBrightnessValue;
};

#endif // CONTROLS_PRESENTER_HPP
