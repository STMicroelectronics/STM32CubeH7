#ifndef HOMEUIWHEELELEMENT_HPP
#define HOMEUIWHEELELEMENT_HPP

#include <gui_generated/containers/HomeUIWheelElementBase.hpp>
#include <gui/containers/HomeUIWheelLargeElements.hpp>
#include <gui/containers/HomeUIWheelSmallElements.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

class HomeUIWheelElement : public HomeUIWheelElementBase
{
public:
    HomeUIWheelElement();
    virtual ~HomeUIWheelElement() {}

    virtual void initialize();

    void setCenterItem(bool set);

    void setRoomInfo(int16_t item);
    void setContent(bool devices);
    void fadeWidgets(uint8_t endAlpha, uint16_t duration, uint16_t delay = 0, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);
    void setAlpha(uint8_t alpha);

protected:
    int16_t currentItem;
    bool showDevices;

    FadeAnimator<HomeUIWheelLargeElements> largeElements;
    FadeAnimator<HomeUIWheelSmallElements> smallElements;
};

#endif // HOMEUIWHEELELEMENT_HPP
