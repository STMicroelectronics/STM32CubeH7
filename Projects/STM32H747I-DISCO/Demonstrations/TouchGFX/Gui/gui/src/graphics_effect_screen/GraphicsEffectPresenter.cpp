#include <gui/graphics_effect_screen/GraphicsEffectView.hpp>
#include <gui/graphics_effect_screen/GraphicsEffectPresenter.hpp>
#include <gui/common/Defines.hpp>

GraphicsEffectPresenter::GraphicsEffectPresenter(GraphicsEffectView& v)
    : DemoPresenter(v), view(v), counter(0), sliderCounter(0), rightSliderMove(false), leftSliderMove(false)
{
}

void GraphicsEffectPresenter::activate()
{
    if (model->getCounterActive())
    {
        view.changeSlider1Value(40);
        view.changeSlider2Value(85);
    }
	model->setCurrentScreen(Defines::GRAPHICS_EFFECT_SCREEN);
}

void GraphicsEffectPresenter::deactivate()
{

}

void GraphicsEffectPresenter::oneSecond()
{
    counter++;
    switch (counter)
    {
    case 4:
        leftSliderMove = true;
        sliderCounter = 40;
        break;
    case 18:
        leftSliderMove = false;
        break;
    case 24:
        rightSliderMove = true;
        sliderCounter = 85;
        break;
    case 28:
        rightSliderMove = false;
        break;
    case 30:
        view.changeSlider2Value(75);
        break;
    case 38:
        view.screenClick(ClickEvent::PRESSED, 48, 42);
        break;
    case 40:
        view.screenClick(ClickEvent::RELEASED, 48, 42);
        break;
    }
}

void GraphicsEffectPresenter::presenterTick()
{
    if (leftSliderMove)
    {
        view.changeSlider1Value(sliderCounter);
        sliderCounter++;
    }
    if (rightSliderMove)
    {
        view.changeSlider2Value(sliderCounter);
        sliderCounter--;
    }
}

void GraphicsEffectPresenter::screenSaverMinorTick()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreen();
}
