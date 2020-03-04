#include <gui/homeautomation_screen/HomeAutomationView.hpp>
#include <gui/homeautomation_screen/HomeAutomationPresenter.hpp>

HomeAutomationPresenter::HomeAutomationPresenter(HomeAutomationView& v)
    : view(v)
{
}

void HomeAutomationPresenter::activate()
{
}

void HomeAutomationPresenter::deactivate()
{
}

void HomeAutomationPresenter::gotoMenuSelected()
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

void HomeAutomationPresenter::mcuLoadUpdated(uint8_t newMcuLoad)
{
    view.mcuLoadUpdated(newMcuLoad);
}

void HomeAutomationPresenter::screenSaverMinorTick()
{
    view.screenSaverMinorTick();
}
