#include <gui/controls_screen/ControlsView.hpp>
#include <gui/controls_screen/ControlsPresenter.hpp>

ControlsPresenter::ControlsPresenter(ControlsView& v)
    : view(v)
{
}

void ControlsPresenter::activate()
{
    model->setScanPotentiometer(true);
    origBrightnessValue = model->getBrightnessValue();
    view.setPotentiometerValue(model->getPotentiometerValue());
    view.setBrightnessValue(model->getBrightnessValue());
}

void ControlsPresenter::deactivate()
{
    model->setScanPotentiometer(false);
    model->setBrightnessValue(origBrightnessValue);
}

void ControlsPresenter::gotoMenuSelected()
{
    if (model->getPreviousSelectedMainMenuType() == Defines::CAROUSEL_MENU)
    {
        static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreenNoTransition();
    }
    else if (model->getPreviousSelectedMainMenuType() == Defines::ANIMATING_BUTTONS_MENU)
    {
        static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuAnimatingButtonsScreenNoTransition();
    }
}

void ControlsPresenter::mcuLoadUpdated(uint8_t newMcuLoad)
{
    view.mcuLoadUpdated(newMcuLoad);
}

void ControlsPresenter::screenSaverMinorTick()
{
    view.screenSaverMinorTick();
}

void ControlsPresenter::newPotentiometerValue(int value)
{
    view.setPotentiometerValue(value);
}

void ControlsPresenter::newBrightnessValue(int value)
{
    model->setBrightnessValue(value);
}
