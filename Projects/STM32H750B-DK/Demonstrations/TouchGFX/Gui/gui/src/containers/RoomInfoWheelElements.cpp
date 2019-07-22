#include <gui/containers/RoomInfoWheelElements.hpp>

RoomInfoWheelElements::RoomInfoWheelElements()
{
    
}

void RoomInfoWheelElements::initialize()
{
    RoomInfoWheelElementsBase::initialize();

    roomInfoElement.initialize();
    roomInfoElement.setXY(0, 0);
    add(roomInfoElement);
}

void RoomInfoWheelElements::updateValues(int16_t item)
{
    roomInfoElement.updateValues(item);
}

void RoomInfoWheelElements::setAlpha(uint8_t alpha)
{
    roomInfoElement.setAlpha(alpha);
}

void RoomInfoWheelElements::startFadeAnimation(uint8_t endAlpha, uint16_t duration, uint16_t delay, EasingEquation alphaProgressionEquation)
{
    roomInfoElement.setFadeAnimationDelay(delay);
    roomInfoElement.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
}
