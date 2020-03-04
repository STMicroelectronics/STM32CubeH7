#ifndef HOMEAUTOMATION_PRESENTER_HPP
#define HOMEAUTOMATION_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class HomeAutomationView;

class HomeAutomationPresenter : public Presenter, public ModelListener
{
public:
    HomeAutomationPresenter(HomeAutomationView& v);
    virtual ~HomeAutomationPresenter() {};

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

    void gotoMenuSelected();
    void mcuLoadUpdated(uint8_t newMcuLoad);
    void screenSaverMinorTick();

private:
    HomeAutomationPresenter();

    HomeAutomationView& view;
};

#endif // HOMEAUTOMATION_PRESENTER_HPP
