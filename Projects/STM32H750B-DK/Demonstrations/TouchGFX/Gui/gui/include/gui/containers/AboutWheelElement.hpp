#ifndef ABOUTWHEELELEMENT_HPP
#define ABOUTWHEELELEMENT_HPP

#include <gui_generated/containers/AboutWheelElementBase.hpp>

class AboutWheelElement : public AboutWheelElementBase
{
public:
    AboutWheelElement();
    virtual ~AboutWheelElement() {}

    virtual void initialize();

    void setText(int16_t item);
    void setLargeText();
    void setSmallText();

protected:
};

#endif // ABOUTWHEELELEMENT_HPP
