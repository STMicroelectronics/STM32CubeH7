
#include <gui/game2048_screen/Game2048Presenter.hpp>
#include <gui/game2048_screen/Game2048View.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <gui/common/Defines.hpp>

Game2048Presenter::Game2048Presenter(Game2048View& v) :
    DemoPresenter(v),
    view(v),
    counter(0)
{
}

Game2048Presenter::~Game2048Presenter()
{
}

void Game2048Presenter::activate()
{
    model->setCurrentScreen(Defines::GAME2048_SCREEN);
}

void Game2048Presenter::deactivate()
{

}

void Game2048Presenter::screenSaverMinorTick()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreen();
}

void Game2048Presenter::oneSecond()
{
    counter++;
    switch (counter)
    {
    case 4:
        view.screenClick(ClickEvent::PRESSED, 520, 220);
        break;
    case 7:
        view.screenClick(ClickEvent::RELEASED, 520, 220);
        break;
    case 60:
        view.screenClick(ClickEvent::PRESSED, 606, 440);
        break;
    case 64:
        view.screenClick(ClickEvent::RELEASED, 606, 440);
        break;
    }
}
