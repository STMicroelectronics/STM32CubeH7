#ifndef GRAPHICS_EFFECT_PRESENTER_HPP
#define GRAPHICS_EFFECT_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class GraphicsEffectView;

class GraphicsEffectPresenter : public DemoPresenter
{
public:
    GraphicsEffectPresenter(GraphicsEffectView& v);

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

    virtual ~GraphicsEffectPresenter() {};

    virtual void oneSecond();
    virtual void presenterTick();
    virtual void screenSaverMinorTick();
private:
    GraphicsEffectPresenter();

    GraphicsEffectView& view;

    int16_t counter;
    uint16_t sliderCounter;
    uint16_t tickCounter;
    bool rightSliderMove;
    bool leftSliderMove;
};


#endif // GRAPHICS_EFFECT_PRESENTER_HPP
