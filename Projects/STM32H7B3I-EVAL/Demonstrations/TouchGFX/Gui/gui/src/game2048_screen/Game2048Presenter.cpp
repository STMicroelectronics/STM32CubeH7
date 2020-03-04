#include <gui/game2048_screen/Game2048View.hpp>
#include <gui/game2048_screen/Game2048Presenter.hpp>

Game2048Presenter::Game2048Presenter(Game2048View& v)
    : view(v)
{
}

void Game2048Presenter::activate()
{
}

void Game2048Presenter::deactivate()
{
}

void Game2048Presenter::gotoMenuSelected()
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

void Game2048Presenter::mcuLoadUpdated(uint8_t newMcuLoad)
{
    view.mcuLoadUpdated(newMcuLoad);
}

void Game2048Presenter::screenSaverMinorTick()
{
    view.screenSaverMinorTick();
}
