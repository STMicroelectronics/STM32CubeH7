#include <gui/gameKnight_screen/GameKnightPresenter.hpp>
#include <gui/gameKnight_screen/GameKnightView.hpp>
#include <gui/model/Model.hpp>

GameKnightPresenter::GameKnightPresenter(GameKnightView& v) : 
    DemoPresenter(v), 
    view(v),
    counter(0)
{
}

void GameKnightPresenter::activate()
{
    setChromArtStatus(false);
    HAL::getInstance()->setFrameRateCompensation(true);
    model->setCurrentScreen(Defines::KNIGHT_GAME_SCREEN);
}

void GameKnightPresenter::deactivate()
{
    setChromArtStatus(false);
    HAL::getInstance()->setFrameRateCompensation(false);
}

void GameKnightPresenter::mcuLoadUpdated(uint8_t mcuLoad)
{
    view.mcuLoadUpdated(mcuLoad);
}

void GameKnightPresenter::setChromArtStatus(bool chromArtStatus)
{
#if defined(ST) && !defined(SIMULATOR)
    disableChromArt = chromArtStatus;
#endif
}

void GameKnightPresenter::oneSecond()
{
    counter++;
    switch (counter)
    {
    case 3:
        view.screenClick(ClickEvent::PRESSED, 405, 406);
        break;
    case 4:
        view.screenClick(ClickEvent::RELEASED, 405, 406);
        break;
    case 10:
        view.screenClick(ClickEvent::PRESSED, 62, 420);
        break;
    case 11:
        view.screenClick(ClickEvent::RELEASED, 62, 420);
        break;
    case 20:
        view.screenClick(ClickEvent::PRESSED, 62, 430);
        break;
    case 21:
        view.screenClick(ClickEvent::RELEASED, 62, 430);
        break;
    case 30:
        view.screenClick(ClickEvent::PRESSED, 62, 430);
        break;
    case 31:
        view.screenClick(ClickEvent::RELEASED, 62, 430);
        break;
    case 35:
        view.screenClick(ClickEvent::PRESSED, 62, 430);
        break;
    case 36:
        view.screenClick(ClickEvent::RELEASED, 62, 430);
        break;
    case 41:
        view.screenClick(ClickEvent::PRESSED, 62, 420);
        break;
    case 42:
        view.screenClick(ClickEvent::RELEASED, 62, 420);
        break;
    case 50:
        view.screenClick(ClickEvent::PRESSED, 580, 40);
        break;
    case 51:
        view.screenClick(ClickEvent::RELEASED, 580, 40);
        break;
    case 60:
        view.screenClick(ClickEvent::PRESSED, 580, 40);
        break;
    case 61:
        view.screenClick(ClickEvent::RELEASED, 580, 40);
        break;
    case 100:
        view.screenClick(ClickEvent::PRESSED, 234, 406);
        break;
    case 101:
        view.screenClick(ClickEvent::RELEASED, 234, 406);
        break;
    }

    // Ensure Game exit
    if(counter > 110)
    {
        static bool pressed = true;
        view.screenClick(pressed ? ClickEvent::PRESSED : ClickEvent::RELEASED, 234, 406);
        pressed = !pressed;
    }
}

void GameKnightPresenter::screenSaverMinorTick()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreen();
}
