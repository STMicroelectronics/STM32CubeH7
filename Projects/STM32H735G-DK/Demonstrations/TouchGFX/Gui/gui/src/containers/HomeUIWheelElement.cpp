#include <gui/containers/HomeUIWheelElement.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include "BitmapDatabase.hpp"

HomeUIWheelElement::HomeUIWheelElement() :
    showDevices(false)
{    
}

void HomeUIWheelElement::initialize()
{
    HomeUIWheelElementBase::initialize();
    
    largeElements.initialize();
    largeElements.setXY(0, 0);
    largeElements.setAlpha(0);
    largeElements.setVisible(false);

    smallElements.initialize();
    smallElements.setXY(0, 0);
    smallElements.setAlpha(0);

    add(largeElements);
    add(smallElements);
}

void HomeUIWheelElement::setCenterItem(bool set)
{
    if (set)
    {
        largeElements.setVisible(true);
        smallElements.setVisible(false);
    }
    else
    {
        largeElements.setVisible(false);
        smallElements.setVisible(true);
    }
    HomeUIWheelElement::invalidate();
}

void HomeUIWheelElement::setRoomInfo(int16_t item)
{
    currentItem = item;

    largeElements.setRoomInfo(item);
    smallElements.setRoomInfo(item);

    HomeUIWheelElement::invalidate();
}

void HomeUIWheelElement::setContent(bool devices)
{
    showDevices = devices;

    largeElements.setContent(devices);
    smallElements.setContent(devices);

    setRoomInfo(currentItem);
}

void HomeUIWheelElement::fadeWidgets(uint8_t endAlpha, uint16_t duration, uint16_t delay, EasingEquation alphaProgressionEquation)
{
    largeElements.setFadeAnimationDelay(delay);
    largeElements.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);

    smallElements.setFadeAnimationDelay(delay);
    smallElements.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
}

void HomeUIWheelElement::setAlpha(uint8_t alpha)
{
    largeElements.setAlpha(alpha);
    smallElements.setAlpha(alpha);
}
