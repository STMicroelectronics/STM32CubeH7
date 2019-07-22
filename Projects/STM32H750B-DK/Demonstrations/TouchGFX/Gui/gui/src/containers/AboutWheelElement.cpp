#include <gui/containers/AboutWheelElement.hpp>
#include <texts/TextKeysAndLanguages.hpp>

AboutWheelElement::AboutWheelElement()
{

}

void AboutWheelElement::initialize()
{
    AboutWheelElementBase::initialize();
}

void AboutWheelElement::setText(int16_t item)
{
    switch (item)
    {
    case 0:
        largeText.setTypedText(TypedText(T_ABOUTLARGESTRUCTURE));
        smallText.setTypedText(TypedText(T_ABOUTSMALLSTRUCTURE));
        break;

    case 1:
        largeText.setTypedText(TypedText(T_ABOUTLARGESKINS));
        smallText.setTypedText(TypedText(T_ABOUTSMALLSKINS));
        break;

    case 2:
        largeText.setTypedText(TypedText(T_ABOUTLARGEWIDGETS));
        smallText.setTypedText(TypedText(T_ABOUTSMALLWIDGETS));
        break;    

    case 3:
        largeText.setTypedText(TypedText(T_ABOUTLARGEEXAMPLES));
        smallText.setTypedText(TypedText(T_ABOUTSMALLEXAMPLES));
        break;

    case 4:
        largeText.setTypedText(TypedText(T_ABOUTLARGECUSTOMCONTAINER));
        smallText.setTypedText(TypedText(T_ABOUTSMALLCUSTOMCONTAINER));
        break;

    case 5:
        largeText.setTypedText(TypedText(T_ABOUTLARGETEXTHANDLING));
        smallText.setTypedText(TypedText(T_ABOUTSMALLTEXTHANDLING));
        break;

    case 6:
        largeText.setTypedText(TypedText(T_ABOUTLARGECODEGENERATION));
        smallText.setTypedText(TypedText(T_ABOUTSMALLCODEGENERATION));
        break;

    case 7:
        largeText.setTypedText(TypedText(T_ABOUTLARGEINTERACTIONS));
        smallText.setTypedText(TypedText(T_ABOUTSMALLINTERACTIONS));
        break;    

    case 8:
        largeText.setTypedText(TypedText(T_ABOUTLARGERUN));
        smallText.setTypedText(TypedText(T_ABOUTSMALLRUN));
        break;
    }
}

void AboutWheelElement::setLargeText()
{
    largeText.setVisible(true);
    smallText.setVisible(false);
    largeText.invalidate();
    smallText.invalidate();
}

void AboutWheelElement::setSmallText()
{
    largeText.setVisible(false);
    smallText.setVisible(true);
    largeText.invalidate();
    smallText.invalidate();
}
