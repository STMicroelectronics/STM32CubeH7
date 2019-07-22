#include <gui/containers/OvenWheelElement.hpp>
#include "BitmapDatabase.hpp"

OvenWheelElement::OvenWheelElement()
{

}

void OvenWheelElement::initialize()
{
    OvenWheelElementBase::initialize();
}

void OvenWheelElement::zoomImage(bool out)
{
    if (out)
    {
        imageSelectedMode.invalidate();
        imageSelectedMode.setVisible(true);
        imageMode.setVisible(false);
    }
    else
    {
        imageSelectedMode.invalidate();
        imageSelectedMode.setVisible(false);
        imageMode.setVisible(true);
    }
}

void OvenWheelElement::setImage(int16_t item)
{
    switch (item)
    {
    case 0:
        imageMode.setBitmap(Bitmap(BITMAP_ICON_LIGHT_ID));
        imageSelectedMode.setBitmap(Bitmap(BITMAP_ICON_LIGHT_ACTIVE_ID));
        break;
    case 1:
        imageMode.setBitmap(Bitmap(BITMAP_ICON_TOP_ID));
        imageSelectedMode.setBitmap(Bitmap(BITMAP_ICON_TOP_ACTIVE_ID));
        break;
    case 2:
        imageMode.setBitmap(Bitmap(BITMAP_ICON_BUTTOM_ID));
        imageSelectedMode.setBitmap(Bitmap(BITMAP_ICON_BUTTOM_ACTIVE_ID));
        break;
    case 3:
        imageMode.setBitmap(Bitmap(BITMAP_ICON_HOTAIR_ID));
        imageSelectedMode.setBitmap(Bitmap(BITMAP_ICON_HOTAIR_ACTIVE_ID));
        break;
    case 4:
        imageMode.setBitmap(Bitmap(BITMAP_ICON_GRILL_ID));
        imageSelectedMode.setBitmap(Bitmap(BITMAP_ICON_GRILL_ACTIVE_ID));
        break;
    case 5:
        imageMode.setBitmap(Bitmap(BITMAP_ICON_HOTAIR_GRILL_ID));
        imageSelectedMode.setBitmap(Bitmap(BITMAP_ICON_HOTAIR_GRILL_ACTIVE_ID));
        break;
    case 6:
        imageMode.setBitmap(Bitmap(BITMAP_ICON_CLEAN_ID));
        imageSelectedMode.setBitmap(Bitmap(BITMAP_ICON_CLEAN_ACTIVE_ID));
        break;
    }
}

void OvenWheelElement::startFade(uint8_t endAlpha, uint16_t duration, uint16_t delay, EasingEquation alphaProgressionEquation)
{
    imageMode.setFadeAnimationDelay(delay);
    imageSelectedMode.setFadeAnimationDelay(delay);

    imageMode.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    imageSelectedMode.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
}
