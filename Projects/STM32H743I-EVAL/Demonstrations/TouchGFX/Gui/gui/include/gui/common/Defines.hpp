#ifndef GUI_DEFINES_HPP
#define GUI_DEFINES_HPP

#include <touchgfx/hal/HAL.hpp>

class Defines
{
public:
    enum MainMenuType
    {
        ANIMATING_BUTTONS_MENU = 0,
        CAROUSEL_MENU,
        NO_MENU
    };

    enum DemoID
    {
        GAME2048 = 0,
        KNIGHT_GAME,
        CONTROLS,
        AUDIO_PLAYER,
        HOME_AUTOMATION,
        SETTINGS,
        VIDEO_PLAYER,
        BUMP_MAP,
        NUMBER_OF_DEMO_SCREENS,
        NO_DEMO_SCREEN
    };

	enum Screens
	{
		AUDIO_PLAYER_SCREEN,
		CONTROLS_SCREEN,
		KNIGHT_GAME_SCREEN,
		GAME2048_SCREEN,
		BUMP_MAP_SCREEN,
		HOME_AUTOMATION_SCREEN,
		MAIN_MENU_SCREEN,
		SETTINGS_SCREEN,
		VIDEO_PLAYER_SCREEN
	};
};


#endif /* GUI_DEFINES_HPP */
