#ifndef BATHTUBWHEELELEMENT_HPP
#define BATHTUBWHEELELEMENT_HPP

#include <gui_generated/containers/BathtubWheelElementBase.hpp>

class BathtubWheelElement : public BathtubWheelElementBase
{
public:
    BathtubWheelElement();
    virtual ~BathtubWheelElement() {}

    virtual void initialize();

    void setTemp(uint16_t temp, bool isCelcius);
    void setJetspeed(uint16_t speed);

protected:
};

#endif // BATHTUBWHEELELEMENT_HPP
