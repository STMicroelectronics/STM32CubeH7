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
        HOME_AUTOMATION,
        SETTINGS,
        GRAPHICS_EFFECT,
        NUMBER_OF_DEMO_SCREENS,
        NO_DEMO_SCREEN
    };
};

#endif /* GUI_DEFINES_HPP */
