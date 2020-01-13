#ifndef OVENTIMERWHEELCENTERELEMENT_HPP
#define OVENTIMERWHEELCENTERELEMENT_HPP

#include <gui_generated/containers/OvenTimerWheelCenterElementBase.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

class OvenTimerWheelCenterElement : public OvenTimerWheelCenterElementBase
{
public:
    OvenTimerWheelCenterElement();
    virtual ~OvenTimerWheelCenterElement() {}

    virtual void initialize();
    void updateText(int16_t itemIndex);
    void setAlpha(uint8_t alpha);
    uint8_t getAlpha();

protected:
    TextAreaWithOneWildcard textAreaWheelCenterValue;

    static const uint16_t TEXTAREAWHEELCENTERVALUE_SIZE = 3;
    touchgfx::Unicode::UnicodeChar textAreaWheelCenterValueBuffer[TEXTAREAWHEELCENTERVALUE_SIZE];
};

#endif // OVENTIMERWHEELCENTERELEMENT_HPP
