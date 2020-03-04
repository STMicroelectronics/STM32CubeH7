#ifndef CONTROLS_PRESENTER_HPP
#define CONTROLS_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class ControlsView;

class ControlsPresenter : public DemoPresenter
{
public:
    ControlsPresenter(ControlsView& v);

    virtual void activate();
    virtual void deactivate();

    virtual ~ControlsPresenter() {};

    virtual void oneSecond();
    virtual void presenterTick();
    virtual void screenSaverMinorTick();
    void newPotentiometerValue(int value);
    void newBrightnessValue(int value);

private:
    ControlsPresenter();
    ControlsView& view;
    uint8_t origBrightnessValue;
    uint16_t counter;
    uint16_t dragCounter;
    uint16_t tickDivieder;
    bool drag;
};

#endif // CONTROLS_PRESENTER_HPP
