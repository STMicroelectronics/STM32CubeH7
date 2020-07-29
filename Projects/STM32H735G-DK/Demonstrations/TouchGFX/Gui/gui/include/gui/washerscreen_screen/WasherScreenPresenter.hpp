#ifndef WASHERSCREEN_PRESENTER_HPP
#define WASHERSCREEN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class WasherScreenView;

class WasherScreenPresenter : public Presenter, public ModelListener
{
public:
    WasherScreenPresenter(WasherScreenView& v);

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

    virtual ~WasherScreenPresenter() {};
    virtual void mcuLoadUpdated(uint8_t mcuLoad);

    virtual void autoDemoTick();
    virtual void autoDemoStart();

    void setAutoDemoState(Model::AutoDemoStates state);
    Model::AutoDemoStates getAutoDemoState();

private:
    WasherScreenPresenter();

    WasherScreenView& view;
};


#endif // WASHERSCREEN_PRESENTER_HPP
