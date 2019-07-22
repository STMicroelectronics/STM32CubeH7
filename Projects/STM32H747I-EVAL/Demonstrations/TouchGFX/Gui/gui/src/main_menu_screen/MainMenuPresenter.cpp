#include <gui/main_menu_screen/MainMenuPresenter.hpp>
#include <gui/main_menu_screen/MainMenuView.hpp>
#include <gui/common/Defines.hpp>

#ifdef STM32H747xx
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_rcc.h"
#endif

MainMenuPresenter::MainMenuPresenter(MainMenuView& v) :
    DemoPresenter(v),
    view(v),
    moveCounter(0),
    autoMove(false)
{
}

void MainMenuPresenter::activate()
{
    if (model->getCounterActive())
    {
        autoMove = true;
    }
	model->setCurrentScreen(Defines::MAIN_MENU_SCREEN);
}

void MainMenuPresenter::deactivate()
{

}

void MainMenuPresenter::setSelectedDemoScreen(Defines::DemoID elementIndex)
{
    model->setSelectedDemoScreen(elementIndex);
}

void MainMenuPresenter::setPreviousSelectedMenuType(Defines::MainMenuType menuType)
{
    model->setPreviousSelectedMainMenuType(menuType);
}

Defines::DemoID MainMenuPresenter::getSelectedDemoScreen()
{
    return model->getSelectedDemoScreen();
}

Defines::MainMenuType MainMenuPresenter::getPreviousSelectedMenuType()
{
    return model->getPreviousSelectedMainMenuType();
}

void MainMenuPresenter::screenSaverMinorTick()
{
    autoMove = true;
    model->setSelectedDemoScreen(Defines::AUDIO_PLAYER);
    model->setAutoDemoDone(false);
    static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreen();    
}

void MainMenuPresenter::gotoSTMenu()
{
#ifndef SIMULATOR
  NVIC_SystemReset();
#endif
}

void MainMenuPresenter::oneSecond()
{
    if (autoMove)
    {
        switch (model->getSelectedDemoScreen())
        {
        case Defines::AUDIO_PLAYER:
            moveCounter++;
            switch (moveCounter)
            {
            case 3:
                view.caruselMove(true);
                break;
            case 6:
                view.caruselMove(true);
                break;
            case 9:
                view.caruselSelectElement();
                autoMove = false;
                break;
            }
            break;
        case Defines::HOME_AUTOMATION:
            moveCounter++;
            switch (moveCounter)
            {
            case 3:
                view.caruselMove(false);
                break;
            case 6:
                view.caruselSelectElement();
                autoMove = false;
                break;
            }
            break;
        case Defines::GAME2D:
            moveCounter++;
            switch (moveCounter)
            {
            case 3:
                view.caruselMove(true);
                break;
            case 6:
                view.caruselMove(true);
                break;
            case 9:
                view.caruselSelectElement();
                autoMove = false;
                break;
            }
            break;
        case Defines::SETTINGS:
            moveCounter++;
            switch (moveCounter)
            {
            case 3:
                view.caruselMove(true);
                break;
            case 6:
                view.caruselSelectElement();
                autoMove = false;
                break;
            }
            break;
        case Defines::VIDEO_PLAYER:
            moveCounter++;
            switch (moveCounter)
            {
            case 3:
                view.caruselMove(true);
                break;
            case 6:
                view.caruselMove(true);
                break;
            case 9:
                view.caruselMove(true);
                break;
            case 12:
                view.caruselSelectElement();
                autoMove = false;
                break;
            }
            break;
        case Defines::CONTROLS:
            moveCounter++;
            switch (moveCounter)
            {
            case 3:
                view.caruselMove(false);
                break;
            case 6:
                view.caruselMove(false);
                break;
            case 9:
                view.caruselSelectElement();
                autoMove = false;
                break;
            }
            break;
        case Defines::GRAPHICS_EFFECT:
            moveCounter++;
            switch (moveCounter)
            {
            case 3:
                view.caruselMove(true);
                break;
            case 6:
                view.caruselSelectElement();
                autoMove = false;
                break;
            }
            break;
        case Defines::GAME2048:
            moveCounter++;
            switch (moveCounter)
            {
            case 3:
                view.caruselMove(true);
                break;
            case 6:
                view.caruselMove(true);
                break;
            case 9:
                view.caruselSelectElement();
                autoMove = false;
                break;
            }
            break;
        case Defines::NO_DEMO_SCREEN:
        case Defines::NUMBER_OF_DEMO_SCREENS:
            break;
        }
    }
}
     
