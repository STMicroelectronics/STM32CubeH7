#include <gui/game2d_screen/Game2DView.hpp>
#include <gui/game2d_screen/Game2DPresenter.hpp>

Game2DPresenter::Game2DPresenter(Game2DView& v)
    : view(v)
{
}

void Game2DPresenter::activate()
{
    HAL::getInstance()->setFrameRateCompensation(true);
}

void Game2DPresenter::deactivate()
{
    HAL::getInstance()->setFrameRateCompensation(false);
}

void Game2DPresenter::gotoMenuSelected()
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

void Game2DPresenter::mcuLoadUpdated(uint8_t newMcuLoad)
{
    view.mcuLoadUpdated(newMcuLoad);
}
