#include <gui/containers/OvenSplashScreen.hpp>

OvenSplashScreen::OvenSplashScreen()
{

}

void OvenSplashScreen::initialize()
{
    OvenSplashScreenBase::initialize();
}

void OvenSplashScreen::setAlpha(uint8_t alpha)
{
    background.setAlpha(alpha);
    textAreaHour.setAlpha(alpha);
    textAreaMinute.setAlpha(alpha);
    textAreaDivider.setAlpha(alpha);
    textAreaAmPm.setAlpha(alpha);
    powerButton.setAlpha(alpha);
}

uint8_t OvenSplashScreen::getAlpha()
{
    return background.getAlpha();
}

void OvenSplashScreen::screenIsPressed()
{
    if (screenPressed && screenPressed->isValid())
    {
        screenPressed->execute();
    }
}

void OvenSplashScreen::setScreenPressedAction(GenericCallback<>& callback)
{
    screenPressed = &callback;
}
