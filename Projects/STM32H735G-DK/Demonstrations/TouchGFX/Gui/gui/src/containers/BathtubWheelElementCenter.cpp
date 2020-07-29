#include <gui/containers/BathtubWheelElementCenter.hpp>
#include <gui/common/Defines.hpp>

BathtubWheelElementCenter::BathtubWheelElementCenter()
{

}

void BathtubWheelElementCenter::initialize()
{
    BathtubWheelElementCenterBase::initialize();
}

void BathtubWheelElementCenter::setTemp(uint16_t temp, bool isCelius)
{
    if (isCelius)
    {
        Unicode::snprintf(elementValueBuffer, ELEMENTVALUE_SIZE, "%d", TEMPERATURE_C_MIN + temp);
    }
    else
    {
        Unicode::snprintf(elementValueBuffer, ELEMENTVALUE_SIZE, "%d", TEMPERATURE_F_MIN + temp);
    }
}

void BathtubWheelElementCenter::setJetspeed(uint16_t speed)
{
    Unicode::snprintf(elementValueBuffer, ELEMENTVALUE_SIZE, "%d", speed + 1);
}
