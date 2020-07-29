#include <gui/containers/RoomInfo.hpp>
#include <texts/TextKeysAndLanguages.hpp>

RoomInfo::RoomInfo()
{

}

void RoomInfo::initialize()
{
    RoomInfoBase::initialize();
}

void RoomInfo::updateValues(int16_t item)
{
    switch (item)
    {
    case 0:
        Unicode::snprintf(textAreaDevicesLargeBuffer, TEXTAREADEVICESLARGE_SIZE, "%d", 70);
        Unicode::snprintf(textAreaTemperatureBuffer, TEXTAREATEMPERATURE_SIZE, "%d", 70);
        Unicode::snprintf(textAreaHumidtyBuffer, TEXTAREAHUMIDTY_SIZE, "%d", 45);
        textAreaBottomlineLarge.setTypedText(TypedText(T_HOMEUIWHEELIDLE));
        break;

    case 1:
        Unicode::snprintf(textAreaDevicesLargeBuffer, TEXTAREADEVICESLARGE_SIZE, "%d", 73);
        Unicode::snprintf(textAreaTemperatureBuffer, TEXTAREATEMPERATURE_SIZE, "%d", 64);
        Unicode::snprintf(textAreaHumidtyBuffer, TEXTAREAHUMIDTY_SIZE, "%d", 48);
        textAreaBottomlineLarge.setTypedText(TypedText(T_HOMEUIWHEELHEATINGUP));
        break;
    }

    textAreaDevicesLarge.invalidate();
    textAreaTemperature.invalidate();
    textAreaHumidty.invalidate();
    textAreaBottomlineLarge.invalidate();
}

void RoomInfo::setAlpha(uint8_t alpha)
{
    textAreaScheduleTime.setAlpha(alpha);
    textAreaScheduleDays.setAlpha(alpha);
    textAreaSchedule.setAlpha(alpha);
    textAreaHumidty.setAlpha(alpha);
    textAreaTemperature.setAlpha(alpha);
    imageSchedule.setAlpha(alpha);
    textAreaAirHum.setAlpha(alpha);
    imageAirHumidty.setAlpha(alpha);
    swipeBG.setAlpha(alpha);
    textAreaDevicesLarge.setAlpha(alpha);
    imageSliderLarge.setAlpha(alpha);
    textAreaRoomLarge.setAlpha(alpha);
    textAreaDevicesLarge.setAlpha(alpha);
    textAreaBottomlineLarge.setAlpha(alpha);
    textAreaDegreeLarge.setAlpha(alpha);
}

uint8_t RoomInfo::getAlpha()
{
    return swipeBG.getAlpha();
}
