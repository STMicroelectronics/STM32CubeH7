#include <gui/game2D_screen/Game2DPresenter.hpp>
#include <gui/game2D_screen/Game2DView.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <gui/common/Defines.hpp>


Game2DPresenter::Game2DPresenter(Game2DView& v) :
    DemoPresenter(v),
    view(v),
    counter(0)
{
}

Game2DPresenter::~Game2DPresenter()
{
}

void Game2DPresenter::activate()
{
    HAL::getInstance()->setFrameRateCompensation(true);
	model->setCurrentScreen(Defines::GAME2D_SCREEN);
}

void Game2DPresenter::deactivate()
{
    HAL::getInstance()->setFrameRateCompensation(false);
}

void Game2DPresenter::oneSecond()
{
    counter++;
    switch (counter)
    {
    case 4:
        view.screenClick(ClickEvent::PRESSED, 400, 200);
        break;
    case 6:
        view.screenClick(ClickEvent::RELEASED, 400, 200);
        break;
    case 7:
        view.screenClick(ClickEvent::PRESSED, 400, 200);
        break;
    case 9:
        view.screenClick(ClickEvent::RELEASED, 400, 200);
        break;
    case 11:
        view.screenClick(ClickEvent::PRESSED, 400, 200);
        break;
    case 14:
        view.screenClick(ClickEvent::RELEASED, 400, 200);
        break;
    case 18:
        view.screenClick(ClickEvent::PRESSED, 200, 444);
        break;
    case 20:
        view.screenClick(ClickEvent::RELEASED, 200, 444);
        break;
    case 22:
        view.screenClick(ClickEvent::PRESSED, 400, 200);
        break;
    case 24:
        view.screenClick(ClickEvent::RELEASED, 400, 200);
        break;
    case 25:
        view.screenClick(ClickEvent::PRESSED, 400, 200);
        break;
    case 27:
        view.screenClick(ClickEvent::RELEASED, 400, 200);
        break;
    case 29:
        view.screenClick(ClickEvent::PRESSED, 400, 200);
        break;
    case 31:
        view.screenClick(ClickEvent::RELEASED, 400, 200);
        break;
    case 34:
        view.screenClick(ClickEvent::PRESSED, 200, 444);
        break;
    case 36:
        view.screenClick(ClickEvent::RELEASED, 200, 444);
        break;
    case 38:
        view.screenClick(ClickEvent::PRESSED, 400, 200);
        break;
    case 40:
        view.screenClick(ClickEvent::RELEASED, 400, 200);
        break;
    case 43:
        view.screenClick(ClickEvent::PRESSED, 400, 200);
        break;
    case 46:
        view.screenClick(ClickEvent::RELEASED, 400, 200);
        break;
    case 50:
        view.screenClick(ClickEvent::PRESSED, 40, 40);
        break;
    case 52:
        view.screenClick(ClickEvent::RELEASED, 40, 40);
        break;
    }
}

void Game2DPresenter::screenSaverMinorTick()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreen();
}
