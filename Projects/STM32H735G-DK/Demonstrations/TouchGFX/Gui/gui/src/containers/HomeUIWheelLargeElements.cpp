#include <gui/containers/HomeUIWheelLargeElements.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>

#include <gui/common/Defines.hpp>

HomeUIWheelLargeElements::HomeUIWheelLargeElements() :
    showDevices(false)
{

}

void HomeUIWheelLargeElements::initialize()
{
    HomeUIWheelLargeElementsBase::initialize();
}

void HomeUIWheelLargeElements::updateCurrentItem(int16_t item)
{
    currentItem = item;
}

void HomeUIWheelLargeElements::setRoomInfo(int16_t item)
{
    currentItem = item;

    if (showDevices)
    {
        switch (item)
        {
        case 0:
            textAreaRoomLarge.setTypedText(TypedText(T_HOMEUIWHEELKITCHENSELECTED));
            textAreaBottomlineLarge.setTypedText(TypedText(T_HOMEUIWHEELDEVICESCENTERITEM));
            Unicode::snprintf(textAreaDevicesLargeBuffer, TEXTAREADEVICESLARGE_SIZE, "%d", 4);
            imageSliderLarge.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_DEVICE_ID));
            textAreaRoomLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaDegreeLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaDevicesLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaBottomlineLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            break;

        case 1:
            textAreaRoomLarge.setTypedText(TypedText(T_HOMEUIWHEELBATHROOMSELECTED));
            textAreaBottomlineLarge.setTypedText(TypedText(T_HOMEUIWHEELDEVICESCENTERITEM));
            Unicode::snprintf(textAreaDevicesLargeBuffer, TEXTAREADEVICESLARGE_SIZE, "%d", 3);
            imageSliderLarge.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_DEVICE_ID));
            textAreaRoomLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaDegreeLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaDevicesLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaBottomlineLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            break;

        case 2:
            textAreaRoomLarge.setTypedText(TypedText(T_HOMEUIWHEELBASEMENTSELECTED));
            textAreaBottomlineLarge.setTypedText(TypedText(T_HOMEUIWHEELDEVICECENTERITEM));
            Unicode::snprintf(textAreaDevicesLargeBuffer, TEXTAREADEVICESLARGE_SIZE, "%d", 1);
            imageSliderLarge.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_OFF_ID));
            textAreaRoomLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDegreeLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDevicesLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaBottomlineLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            break;

        case 3:
            textAreaRoomLarge.setTypedText(TypedText(T_HOMEUIWHEELLIVINGROOMSELECTED));
            textAreaBottomlineLarge.setTypedText(TypedText(T_HOMEUIWHEELDEVICESCENTERITEM));
            Unicode::snprintf(textAreaDevicesLargeBuffer, TEXTAREADEVICESLARGE_SIZE, "%d", 2);
            imageSliderLarge.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_OFF_ID));
            textAreaRoomLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDegreeLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDevicesLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaBottomlineLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            break;

        case 4:
            textAreaRoomLarge.setTypedText(TypedText(T_HOMEUIWHEELBEDROOMSELECTED));
            textAreaBottomlineLarge.setTypedText(TypedText(T_HOMEUIWHEELDEVICESCENTERITEM));
            Unicode::snprintf(textAreaDevicesLargeBuffer, TEXTAREADEVICESLARGE_SIZE, "%d", 2);
            imageSliderLarge.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_OFF_ID));
            textAreaRoomLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDegreeLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDevicesLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaBottomlineLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            break;
        }
    }
    else
    {
        switch (item)
        {
        case 0:
            textAreaRoomLarge.setTypedText(TypedText(T_HOMEUIWHEELKITCHENSELECTED));
            textAreaBottomlineLarge.setTypedText(TypedText(T_HOMEUIWHEELIDLECENTER));
            Unicode::snprintf(textAreaDevicesLargeBuffer, TEXTAREADEVICESLARGE_SIZE, "%d", 70);
            imageSliderLarge.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_TEMP_ID));
            textAreaRoomLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaDegreeLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaDevicesLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaBottomlineLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            break;

        case 1:
            textAreaRoomLarge.setTypedText(TypedText(T_HOMEUIWHEELBATHROOMSELECTED));
            textAreaBottomlineLarge.setTypedText(TypedText(T_HOMEUIWHEELHEATINGUPCENTER));
            Unicode::snprintf(textAreaDevicesLargeBuffer, TEXTAREADEVICESLARGE_SIZE, "%d", 73);
            imageSliderLarge.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_TEMP_ID));
            textAreaRoomLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaDegreeLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaDevicesLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaBottomlineLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            break;

        case 2:
            textAreaRoomLarge.setTypedText(TypedText(T_HOMEUIWHEELBASEMENTSELECTED));
            textAreaBottomlineLarge.setTypedText(TypedText(T_HOMEUIWHEELIDLECENTER));
            Unicode::snprintf(textAreaDevicesLargeBuffer, TEXTAREADEVICESLARGE_SIZE, "%d", 60);
            imageSliderLarge.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_OFF_ID));
            textAreaRoomLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDegreeLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDevicesLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaBottomlineLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            break;

        case 3:
            textAreaRoomLarge.setTypedText(TypedText(T_HOMEUIWHEELLIVINGROOMSELECTED));
            textAreaBottomlineLarge.setTypedText(TypedText(T_HOMEUIWHEELIDLECENTER));
            Unicode::snprintf(textAreaDevicesLargeBuffer, TEXTAREADEVICESLARGE_SIZE, "%d", 64);
            imageSliderLarge.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_OFF_ID));
            textAreaRoomLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDegreeLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDevicesLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaBottomlineLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            break;

        case 4:
            textAreaRoomLarge.setTypedText(TypedText(T_HOMEUIWHEELBEDROOMSELECTED));
            textAreaBottomlineLarge.setTypedText(TypedText(T_HOMEUIWHEELIDLECENTER));
            Unicode::snprintf(textAreaDevicesLargeBuffer, TEXTAREADEVICESLARGE_SIZE, "%d", 68);
            imageSliderLarge.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_OFF_ID));
            textAreaRoomLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDegreeLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDevicesLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaBottomlineLarge.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            break;
        }
    }
}

void HomeUIWheelLargeElements::setContent(bool devices)
{
    showDevices = devices;
    if (showDevices)
    {
        textAreaDegreeLarge.setVisible(false);
        textAreaDevicesLarge.setXY(26, 44);
        textAreaBottomlineLarge.setXY(24, 112);
    }
    else
    {
        textAreaDegreeLarge.setVisible(true);
        textAreaDevicesLarge.setXY(26, 55);
        textAreaBottomlineLarge.setXY(24, 120);
    }

    setRoomInfo(currentItem);
}

void HomeUIWheelLargeElements::setAlpha(uint8_t alpha)
{
    textAreaDegreeLarge.setAlpha(alpha);
    textAreaDevicesLarge.setAlpha(alpha);
    textAreaBottomlineLarge.setAlpha(alpha);
    imageSliderLarge.setAlpha(alpha);
    textAreaRoomLarge.setAlpha(alpha);
}

uint8_t HomeUIWheelLargeElements::getAlpha()
{
    return imageSliderLarge.getAlpha();
}
