

#include <gui/controls_screen/ControlsPresenter.hpp>
#include <gui/controls_screen/ControlsView.hpp>
#include <gui/common/Defines.hpp>


ControlsPresenter::ControlsPresenter(ControlsView& v) : 
    DemoPresenter(v),
    view(v),
    counter(0),
    tickDivieder(0)
{
}

void ControlsPresenter::activate()
{
#ifndef SIMULATOR
#ifdef USE_STM32H747I_DISCO
    model->setScanJunctionTemp(true);
    view.setJunctionTempValue(model->getTempValue());
#else
    model->setScanPotentiometer(true);
    view.setPotentiometerValue(model->getPotentiometerValue());
#endif
#endif
    origBrightnessValue = model->getBrightnessValue();
    view.setBrightnessValue(model->getBrightnessValue());
	model->setCurrentScreen(Defines::CONTROLS_SCREEN);
}

void ControlsPresenter::deactivate()
{
#ifndef SIMULATOR
#ifdef USE_STM32H747I_DISCO
    model->setScanJunctionTemp(false);
#else
    model->setScanPotentiometer(false);
#endif
#endif
    model->setBrightnessValue(origBrightnessValue);
}

#ifdef USE_STM32H747I_DISCO
void ControlsPresenter::newJunctionTempValue(int value)
{
    view.setJunctionTempValue(value);
}
#else
void ControlsPresenter::newPotentiometerValue(int value)
{
    view.setPotentiometerValue(value);
}
#endif

void ControlsPresenter::newBrightnessValue(int value)
{
    model->setBrightnessValue(value);
}

void ControlsPresenter::oneSecond()
{
    counter++;
    switch (counter)
    {
    case 4:
        dragCounter = 560;
        view.screenClick(ClickEvent::PRESSED, dragCounter, 100);
        break;
    case 8:
        view.screenClick(ClickEvent::RELEASED, dragCounter, 100);
        break;
    case 14:
        dragCounter = 20;
        view.setBrightnessValue(dragCounter);
        break;
    case 33 :
        view.screenClick(ClickEvent::PRESSED, 42, 42);
        break;
    case 36:
        view.screenClick(ClickEvent::RELEASED, 42, 42);
        break;
    }
}

void ControlsPresenter::presenterTick()
{
    if (counter > 4 && counter < 8 )
    {
        view.screenDrag(dragCounter, 100, dragCounter - 6, 100);
        dragCounter = dragCounter - 6;
    }
    else if (counter > 18 && counter < 28)
    {
        if (tickDivieder >= 2)
        {
            view.setBrightnessValue(dragCounter);
            dragCounter++;
            tickDivieder = 0;
        }
        else
        {
            tickDivieder++;
        }
    }
}

void ControlsPresenter::screenSaverMinorTick()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreen();
}
