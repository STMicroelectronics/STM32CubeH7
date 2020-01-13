#include <gui/containers/OvenTimerWheelElement.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

OvenTimerWheelElement::OvenTimerWheelElement()
{    
}

void OvenTimerWheelElement::initialize()
{
    OvenTimerWheelElementBase::initialize();   

    textAreaWheelValue.setPosition(0, 9, 128, 39);
    textAreaWheelValue.setColor(touchgfx::Color::getColorFrom24BitRGB(91, 146, 167));
    textAreaWheelValue.setLinespacing(0);
    Unicode::snprintf(textAreaWheelValueBuffer, TEXTAREAWHEELVALUE_SIZE, "%d", 0);
    textAreaWheelValue.setWildcard(textAreaWheelValueBuffer);
    textAreaWheelValue.setTypedText(TypedText(T_OVENWHEELTEXT));

    add(textAreaWheelValue);
}

void OvenTimerWheelElement::updateText(int16_t itemIndex)
{
    Unicode::snprintf(textAreaWheelValueBuffer, TEXTAREAWHEELVALUE_SIZE, "%d", itemIndex);
    textAreaWheelValue.invalidate();
}

void OvenTimerWheelElement::setAlpha(uint8_t alpha)
{
    textAreaWheelValue.setAlpha(alpha);
}

uint8_t OvenTimerWheelElement::getAlpha()
{
    return textAreaWheelValue.getAlpha();
}
