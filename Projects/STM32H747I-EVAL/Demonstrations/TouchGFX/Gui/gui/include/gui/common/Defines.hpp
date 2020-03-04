

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
        GAME2D,
        CONTROLS,
        AUDIO_PLAYER,
        HOME_AUTOMATION,
        SETTINGS,
        VIDEO_PLAYER,
        GRAPHICS_EFFECT,
        NUMBER_OF_DEMO_SCREENS,
        NO_DEMO_SCREEN
    };

	enum Screens
	{
		AUDIO_PLAYER_SCREEN,
		CONTROLS_SCREEN,
		GAME2D_SCREEN,
		GAME2048_SCREEN,
		GRAPHICS_EFFECT_SCREEN,
		HOME_AUTOMATION_SCREEN,
		MAIN_MENU_SCREEN,
		SETTINGS_SCREEN,
		VIDEO_PLAYER_SCREEN
	};
};


#endif /* GUI_DEFINES_HPP */
