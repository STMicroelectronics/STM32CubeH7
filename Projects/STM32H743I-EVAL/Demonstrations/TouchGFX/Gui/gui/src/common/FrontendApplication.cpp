#include <new>
#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/transitions/NoTransition.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <gui/main_menu_screen/MainMenuCarouselView.hpp>
#include <gui/main_menu_screen/MainMenuAnimatingButtonsView.hpp>
#include <gui/main_menu_screen/MainMenuPresenter.hpp>
#include <gui/game2048_screen/Game2048View.hpp>
#include <gui/game2048_screen/Game2048Presenter.hpp>
#include <gui/controls_screen/ControlsView.hpp>
#include <gui/controls_screen/ControlsPresenter.hpp>
#include <gui/audio_player_screen/AudioPlayerPresenter.hpp>
#include <gui/audio_player_screen/AudioPlayerView.hpp>
#include <gui/home_automation_screen/HomeAutomationPresenter.hpp>
#include <gui/home_automation_screen/HomeAutomationView.hpp>
#include <gui/settings_screen/SettingsPresenter.hpp>
#include <gui/settings_screen/SettingsView.hpp>
#include <gui/video_player_screen/VideoPlayerPresenter.hpp>
#include <gui/video_player_screen/VideoPlayerView.hpp>
#include <gui/bump_map_screen/BumpMapPresenter.hpp>
#include <gui/bump_map_screen/BumpMapView.hpp>
#include <gui/gameKnight_screen/GameKnightPresenter.hpp>
#include <gui/gameKnight_screen/GameKnightView.hpp>

using namespace touchgfx;

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap) :
      touchgfx::MVPApplication(),
      transitionCallback(),
      frontendHeap(heap),
      model(m),
      screenSaverTick(0)
{
}

void FrontendApplication::handleTickEvent()
{
    model.tick();
    MVPApplication::handleTickEvent();

    // Screen saver functionality:
    if (uint32_t(screenSaverTick) >= model.GetautoDemoTicks())
	{
        if (!model.getCounterActive() && model.getCurrentScreen() == Defines::MAIN_MENU_SCREEN)
        {
            model.setCounterActive(true);
            model.screenSaverMinorTick();
        }
        screenSaverTick = 0;
    }
    else
    {
        screenSaverTick++;
    }
}

void FrontendApplication::handleClickEvent(const ClickEvent& evt)
{
    MVPApplication::handleClickEvent(evt);

    // A click event has been registered so reset last click time
    resetScreenSaver();
}

void FrontendApplication::handleDragEvent(const DragEvent& evt)
{
    MVPApplication::handleDragEvent(evt);

    // A drag event has been registered so reset last click time
    resetScreenSaver();
}

void FrontendApplication::resetScreenSaver()
{
    screenSaverTick = 0;
    model.setCounterActive(false);
}

void FrontendApplication::gotoMainMenuCarouselScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoMainMenuCarouselScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoMainMenuCarouselScreenImpl()
{
    makeTransition< MainMenuCarouselView, MainMenuPresenter, NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoMainMenuAnimatingButtonsScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoMainMenuAnimatingButtonsScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoMainMenuAnimatingButtonsScreenImpl()
{
    makeTransition< MainMenuAnimatingButtonsView, MainMenuPresenter, NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoGame2048Screen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoGame2048ScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoGame2048ScreenImpl()
{
    makeTransition< Game2048View, Game2048Presenter, NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoGameKnightScreen()
{
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::gotoGameKnightScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoGameKnightScreenImpl()
{
    makeTransition< GameKnightView, GameKnightPresenter, NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoControlsScreen()
{
    transitionCallback = touchgfx::Callback< FrontendApplication >(this, &FrontendApplication::gotoControlsScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoControlsScreenImpl()
{
    makeTransition< ControlsView, ControlsPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoAudioPlayerScreen()
{
    transitionCallback = touchgfx::Callback< FrontendApplication >(this, &FrontendApplication::gotoAudioPlayerScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoAudioPlayerScreenImpl()
{
    makeTransition< AudioPlayerView, AudioPlayerPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoHomeAutomationScreen()
{
    transitionCallback = touchgfx::Callback< FrontendApplication >(this, &FrontendApplication::gotoHomeAutomationScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoHomeAutomationScreenImpl()
{
    makeTransition< HomeAutomationView, HomeAutomationPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoSettingsScreen()
{
    transitionCallback = touchgfx::Callback< FrontendApplication >(this, &FrontendApplication::gotoSettingsScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoSettingsScreenImpl()
{
    makeTransition< SettingsView, SettingsPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoVideoPlayerScreen()
{
    transitionCallback = touchgfx::Callback< FrontendApplication >(this, &FrontendApplication::gotoVideoPlayerScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoVideoPlayerScreenImpl()
{
    makeTransition< VideoPlayerView, VideoPlayerPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoBumpMapScreen()
{
    transitionCallback = touchgfx::Callback< FrontendApplication >(this, &FrontendApplication::gotoBumpMapScreenImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoBumpMapScreenImpl()
{
    makeTransition< BumpMapView, BumpMapPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}
