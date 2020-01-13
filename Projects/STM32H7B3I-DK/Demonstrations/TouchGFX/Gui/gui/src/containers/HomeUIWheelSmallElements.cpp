#include <gui/containers/HomeUIWheelSmallElements.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>

#include <gui/common/Defines.hpp>

HomeUIWheelSmallElements::HomeUIWheelSmallElements() :
    showDevices(false)
{

}

void HomeUIWheelSmallElements::initialize()
{
    HomeUIWheelSmallElementsBase::initialize();
}

void HomeUIWheelSmallElements::updateCurrentItem(int16_t item)
{
    currentItem = item;
}

void HomeUIWheelSmallElements::setRoomInfo(int16_t item)
{
    currentItem = item;

    if (showDevices)
    {
        switch (item)
        {
        case 0:
            textAreaRoom.setTypedText(TypedText(T_HOMEUIWHEELKITCHEN));
            textAreaBottomline.setTypedText(TypedText(T_HOMEUIWHEELDEVICES));
            Unicode::snprintf(textAreaDevicesBuffer, TEXTAREADEVICES_SIZE, "%d", 4);
            imageSlider.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_DEVICE_SMALL_ID));
            textAreaRoom.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaDegree.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaDevices.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaBottomline.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            break;

        case 1:
            textAreaRoom.setTypedText(TypedText(T_HOMEUIWHEELBATHROOM));
            textAreaBottomline.setTypedText(TypedText(T_HOMEUIWHEELDEVICES));
            Unicode::snprintf(textAreaDevicesBuffer, TEXTAREADEVICES_SIZE, "%d", 3);
            imageSlider.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_DEVICE_SMALL_ID));
            textAreaRoom.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaDegree.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaDevices.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaBottomline.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            break;

        case 2:
            textAreaRoom.setTypedText(TypedText(T_HOMEUIWHEELBASEMENT));
            textAreaBottomline.setTypedText(TypedText(T_HOMEUIWHEELDEVICE));
            Unicode::snprintf(textAreaDevicesBuffer, TEXTAREADEVICES_SIZE, "%d", 1);
            imageSlider.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_OFF_SMALL_ID));
            textAreaRoom.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDegree.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDevices.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaBottomline.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            break;

        case 3:
            textAreaRoom.setTypedText(TypedText(T_HOMEUIWHEELLIVINGROOM));
            textAreaBottomline.setTypedText(TypedText(T_HOMEUIWHEELDEVICES));
            Unicode::snprintf(textAreaDevicesBuffer, TEXTAREADEVICES_SIZE, "%d", 2);
            imageSlider.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_OFF_SMALL_ID));
            textAreaRoom.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDegree.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDevices.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaBottomline.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            break;

        case 4:
            textAreaRoom.setTypedText(TypedText(T_HOMEUIWHEELBEDROOM));
            textAreaBottomline.setTypedText(TypedText(T_HOMEUIWHEELDEVICES));
            Unicode::snprintf(textAreaDevicesBuffer, TEXTAREADEVICES_SIZE, "%d", 2);
            imageSlider.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_OFF_SMALL_ID));
            textAreaRoom.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDegree.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDevices.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaBottomline.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            break;
        }
    }
    else
    {
        switch (item)
        {
        case 0:
            textAreaRoom.setTypedText(TypedText(T_HOMEUIWHEELKITCHEN));
            textAreaBottomline.setTypedText(TypedText(T_HOMEUIWHEELIDLE));
            Unicode::snprintf(textAreaDevicesBuffer, TEXTAREADEVICES_SIZE, "%d", 70);
            imageSlider.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_TEMP_SMALL_ID));
            textAreaRoom.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaDegree.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaDevices.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaBottomline.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            break;

        case 1:
            textAreaRoom.setTypedText(TypedText(T_HOMEUIWHEELBATHROOM));
            textAreaBottomline.setTypedText(TypedText(T_HOMEUIWHEELHEATINGUP));
            Unicode::snprintf(textAreaDevicesBuffer, TEXTAREADEVICES_SIZE, "%d", 73);
            imageSlider.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_TEMP_SMALL_ID));
            textAreaRoom.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaDegree.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaDevices.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            textAreaBottomline.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_FOCUS_TEXT_R, HOME_UI_FOCUS_TEXT_G, HOME_UI_FOCUS_TEXT_B));
            break;

        case 2:
            textAreaRoom.setTypedText(TypedText(T_HOMEUIWHEELBASEMENT));
            textAreaBottomline.setTypedText(TypedText(T_HOMEUIWHEELIDLE));
            Unicode::snprintf(textAreaDevicesBuffer, TEXTAREADEVICES_SIZE, "%d", 60);
            imageSlider.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_OFF_SMALL_ID));
            textAreaRoom.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDegree.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDevices.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaBottomline.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            break;

        case 3:
            textAreaRoom.setTypedText(TypedText(T_HOMEUIWHEELLIVINGROOM));
            textAreaBottomline.setTypedText(TypedText(T_HOMEUIWHEELIDLE));
            Unicode::snprintf(textAreaDevicesBuffer, TEXTAREADEVICES_SIZE, "%d", 64);
            imageSlider.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_OFF_SMALL_ID));
            textAreaRoom.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDegree.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDevices.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaBottomline.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            break;

        case 4:
            textAreaRoom.setTypedText(TypedText(T_HOMEUIWHEELBEDROOM));
            textAreaBottomline.setTypedText(TypedText(T_HOMEUIWHEELIDLE));
            Unicode::snprintf(textAreaDevicesBuffer, TEXTAREADEVICES_SIZE, "%d", 68);
            imageSlider.setBitmap(Bitmap(BITMAP_MAIN_SLIDER_OFF_SMALL_ID));
            textAreaRoom.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDegree.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaDevices.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            textAreaBottomline.setColor(touchgfx::Color::getColorFrom24BitRGB(HOME_UI_UNFOCUS_TEXT_R, HOME_UI_UNFOCUS_TEXT_G, HOME_UI_UNFOCUS_TEXT_B));
            break;
        }
    }
}

void HomeUIWheelSmallElements::setContent(bool devices)
{
    showDevices = devices;
    if (showDevices)
    {
        textAreaDegree.setVisible(false);
        textAreaDevices.setXY(38, 59);
        textAreaBottomline.setXY(24, 114);
    }
    else
    {
        textAreaDegree.setVisible(true);
        textAreaDevices.setXY(38, 64);
        textAreaBottomline.setXY(24, 110);
    }

    setRoomInfo(currentItem);
}

void HomeUIWheelSmallElements::setAlpha(uint8_t alpha)
{
    textAreaDegree.setAlpha(alpha);
    textAreaDevices.setAlpha(alpha);
    textAreaBottomline.setAlpha(alpha);
    imageSlider.setAlpha(alpha);
    textAreaRoom.setAlpha(alpha);
}

uint8_t HomeUIWheelSmallElements::getAlpha()
{
    return imageSlider.getAlpha();
}
