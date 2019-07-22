#ifndef OVENWHEELELEMENT_HPP
#define OVENWHEELELEMENT_HPP

#include <gui_generated/containers/OvenWheelElementBase.hpp>

class OvenWheelElement : public OvenWheelElementBase
{
public:
    OvenWheelElement();
    virtual ~OvenWheelElement() {}

    virtual void initialize();

    void zoomImage(bool out);
    void setImage(int16_t item);
    void startFade(uint8_t endAlpha, uint16_t duration, uint16_t delay = 0, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);

protected:

};

#endif // OVENWHEELELEMENT_HPP
