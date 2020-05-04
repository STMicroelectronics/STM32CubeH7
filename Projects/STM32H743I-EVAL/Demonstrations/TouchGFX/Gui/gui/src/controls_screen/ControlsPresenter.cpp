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
    model->setScanPotentiometer(true);
    origBrightnessValue = model->getBrightnessValue();
    view.setPotentiometerValue(model->getPotentiometerValue());
    view.setBrightnessValue(model->getBrightnessValue());
    model->setCurrentScreen(Defines::CONTROLS_SCREEN);
}

void ControlsPresenter::deactivate()
{
    model->setScanPotentiometer(false);
    model->setBrightnessValue(origBrightnessValue);
}

void ControlsPresenter::newPotentiometerValue(int value)
{
    view.setPotentiometerValue(value);
}

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
        dragCounter = 448;
        view.screenClick(ClickEvent::PRESSED, dragCounter, 100);
        break;
    case 8:
        view.screenClick(ClickEvent::RELEASED, dragCounter, 100);
        break;
    case 14:
        dragCounter = 16;
        view.setBrightnessValue(dragCounter);
        break;
    case 33 :
        view.screenClick(ClickEvent::PRESSED, 34, 42);
        break;
    case 36:
        view.screenClick(ClickEvent::RELEASED, 34, 42);
        break;
    }
}

void ControlsPresenter::presenterTick()
{
    if (counter > 4 && counter < 8 )
    {
        view.screenDrag(dragCounter, 100, dragCounter - 4, 100);
        dragCounter = dragCounter - 4;
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
