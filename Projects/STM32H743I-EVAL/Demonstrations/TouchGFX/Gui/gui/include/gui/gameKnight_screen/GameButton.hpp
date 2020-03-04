#ifndef GAME_BUTTON_HPP
#define GAME_BUTTON_HPP

#include <touchgfx/widgets/Button.hpp>

using namespace touchgfx;

// Small change to standard button. The GameButton fires 
// on pressed and not release.
class GameButton : public Button
{
public:
    void handleClickEvent(const ClickEvent& event)
    {
        bool wasPressed = pressed;
        pressed = (event.getType() == ClickEvent::PRESSED);
        if ((pressed && !wasPressed) || (!pressed && wasPressed))
        {
            // Pressed state changed, so invalidate
            invalidate();
        }
        if ((event.getType() == ClickEvent::PRESSED) && action)
        {
            // This is a click. Fire callback.
            if (action->isValid())
            {
                action->execute(*this);
            }
        }
    }
};

#endif
