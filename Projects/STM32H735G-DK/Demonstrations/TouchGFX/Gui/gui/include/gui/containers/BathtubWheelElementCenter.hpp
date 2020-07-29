#ifndef BATHTUBWHEELELEMENTCENTER_HPP
#define BATHTUBWHEELELEMENTCENTER_HPP

#include <gui_generated/containers/BathtubWheelElementCenterBase.hpp>

class BathtubWheelElementCenter : public BathtubWheelElementCenterBase
{
public:
    BathtubWheelElementCenter();
    virtual ~BathtubWheelElementCenter() {}

    virtual void initialize();
    void setTemp(uint16_t temp, bool isCelcius);
    void setJetspeed(uint16_t speed);

protected:
};

#endif // BATHTUBWHEELELEMENTCENTER_HPP
