#ifndef HALLWAYSCREEN_PRESENTER_HPP
#define HALLWAYSCREEN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class HallwayScreenView;

class HallwayScreenPresenter : public Presenter, public ModelListener
{
public:
    HallwayScreenPresenter(HallwayScreenView& v);

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

    virtual ~HallwayScreenPresenter() {};

    void setShowIntro(bool show);

    virtual void mcuLoadUpdated(uint8_t mcuLoad);

    virtual void autoDemoTick();
    virtual void autoDemoStart();

    void setAutoDemoState(Model::AutoDemoStates state);
    Model::AutoDemoStates getAutoDemoState();
    void resetAutoDemo();

private:
    HallwayScreenPresenter();

    HallwayScreenView& view;
};


#endif // HALLWAYSCREEN_PRESENTER_HPP
