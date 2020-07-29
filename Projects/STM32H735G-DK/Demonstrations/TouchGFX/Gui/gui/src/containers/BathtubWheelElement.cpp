#include <gui/containers/BathtubWheelElement.hpp>
#include <gui/common/Defines.hpp>

BathtubWheelElement::BathtubWheelElement()
{

}

void BathtubWheelElement::initialize()
{
    BathtubWheelElementBase::initialize();
}

void BathtubWheelElement::setTemp(uint16_t temp, bool isCelius)
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

void BathtubWheelElement::setJetspeed(uint16_t speed)
{
    Unicode::snprintf(elementValueBuffer, ELEMENTVALUE_SIZE, "%d", speed + 1);
}
