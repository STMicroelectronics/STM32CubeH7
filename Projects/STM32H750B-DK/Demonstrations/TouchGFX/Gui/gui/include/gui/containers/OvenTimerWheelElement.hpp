#ifndef OVENTIMERWHEELELEMENT_HPP
#define OVENTIMERWHEELELEMENT_HPP

#include <gui_generated/containers/OvenTimerWheelElementBase.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

class OvenTimerWheelElement : public OvenTimerWheelElementBase
{
public:
    OvenTimerWheelElement();
    virtual ~OvenTimerWheelElement() {}

    virtual void initialize();
    void updateText(int16_t itemIndex);
    void setAlpha(uint8_t alpha);
    uint8_t getAlpha();

protected:
    /*
     * Member Declarations
     */
    TextAreaWithOneWildcard textAreaWheelValue;

    /*
     * Wildcard Buffers
     */
    static const uint16_t TEXTAREAWHEELVALUE_SIZE = 3;
    touchgfx::Unicode::UnicodeChar textAreaWheelValueBuffer[TEXTAREAWHEELVALUE_SIZE];
};

#endif // OVENTIMERWHEELELEMENT_HPP
