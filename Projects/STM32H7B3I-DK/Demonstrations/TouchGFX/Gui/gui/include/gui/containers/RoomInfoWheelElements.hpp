#ifndef ROOMINFOWHEELELEMENTS_HPP
#define ROOMINFOWHEELELEMENTS_HPP

#include <gui_generated/containers/RoomInfoWheelElementsBase.hpp>
#include <gui/containers/RoomInfo.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

class RoomInfoWheelElements : public RoomInfoWheelElementsBase
{
public:
    RoomInfoWheelElements();
    virtual ~RoomInfoWheelElements() {}

    virtual void initialize();

    void updateValues(int16_t item);
    void setAlpha(uint8_t alpha);

    void startFadeAnimation(uint8_t endAlpha, uint16_t duration, uint16_t delay = 0, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);

protected:
    FadeAnimator<RoomInfo> roomInfoElement;
};

#endif // ROOMINFOWHEELELEMENTS_HPP
