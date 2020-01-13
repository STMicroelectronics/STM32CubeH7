#include <gui/containers/OvenTimerWheelCenterElement.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

OvenTimerWheelCenterElement::OvenTimerWheelCenterElement()
{
    
}

void OvenTimerWheelCenterElement::initialize()
{
    OvenTimerWheelCenterElementBase::initialize();

    textAreaWheelCenterValue.setPosition(0, 6, 128, 45);
    textAreaWheelCenterValue.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    textAreaWheelCenterValue.setLinespacing(0);
    Unicode::snprintf(textAreaWheelCenterValueBuffer, TEXTAREAWHEELCENTERVALUE_SIZE, "%d", 0);
    textAreaWheelCenterValue.setWildcard(textAreaWheelCenterValueBuffer);
    textAreaWheelCenterValue.setTypedText(TypedText(T_OVENWHEELTEXT));

    add(textAreaWheelCenterValue);
}

void OvenTimerWheelCenterElement::updateText(int16_t itemIndex)
{
    Unicode::snprintf(textAreaWheelCenterValueBuffer, TEXTAREAWHEELCENTERVALUE_SIZE, "%d", itemIndex);
    textAreaWheelCenterValue.invalidate();
}

void OvenTimerWheelCenterElement::setAlpha(uint8_t alpha)
{
    textAreaWheelCenterValue.setAlpha(alpha);
}

uint8_t OvenTimerWheelCenterElement::getAlpha()
{
    return textAreaWheelCenterValue.getAlpha();
}
