#include <gui/common/DemoPresenter.hpp>
#include <gui/common/Defines.hpp>

DemoPresenter::DemoPresenter(DemoViewInterface& view) :
    viewInterface(view)
{
}

void DemoPresenter::menuOptionSelected()
{
    if (model->getPreviousSelectedMainMenuType() == Defines::CAROUSEL_MENU)
    {
        static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreen();
    }
    else if (model->getPreviousSelectedMainMenuType() == Defines::ANIMATING_BUTTONS_MENU)
    {
        static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuAnimatingButtonsScreen();
    }
}

void DemoPresenter::mcuLoadUpdated(uint8_t mcuLoad)
{
    viewInterface.updateProcessorLoad(mcuLoad);
}

FrontendApplication* DemoPresenter::application()
{
    return static_cast<FrontendApplication*>(Application::getInstance());
}

