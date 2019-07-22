#include <gui/containers/DeviceInfo.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

DeviceInfo::DeviceInfo() :
    fanLight(false),
    recipeOnline(false),
    ovenState(false),
    fanState(false),
    fanSpeed(0),
    bathtubOn(false),
    tempCelcius(true),
    jetOn(false),
    lightOn(false),
    tempValue(45),
    jetLevel(1),
    selectedOvenMode(0),
    ovenTimerHour(1),
    ovenTimerMinute(30),
    ovenTemperature(200),
    currentProgram(0),
    washingTime(160)
{

}

void DeviceInfo::initialize()
{
    DeviceInfoBase::initialize();
}

void DeviceInfo::updateDeviceInfo(int16_t item, bool kitchen)
{
    if (kitchen)
    {
        switch (item)
        {
        case 0:
            imageDevice.setBitmap(Bitmap(BITMAP_OVEN_BIG_ID));
            textAreaDeviceName.setTypedText(TypedText(T_HOMEUIDEVICENAMEOVEN));
            break;

        case 1:
            imageDevice.setBitmap(Bitmap(BITMAP_HOME_TEMP_BIG_ID));
            textAreaDeviceName.setTypedText(TypedText(T_HOMEUIDEVICENAMECONTROL));
            break;

        case 2:
            imageDevice.setBitmap(Bitmap(BITMAP_EXTRACTOR_HOOD_BIG_ID));
            textAreaDeviceName.setTypedText(TypedText(T_HOMEUIDEVICENAMEHOOD));
            break;

        case 3:
            imageDevice.setBitmap(Bitmap(BITMAP_FOOD_RECIPIES_ID));
            textAreaDeviceName.setTypedText(TypedText(T_HOMEUIDEVICENAMECONTROL));
            break;
        }
    }
    else
    {
        switch (item)
        {
        case 0:
            imageDevice.setBitmap(Bitmap(BITMAP_BATHTUB_SMALL_ID));
            textAreaDeviceName.setTypedText(TypedText(T_HOMEUIDEVICENAMEBATHTUB));
            break;
        case 1:
            imageDevice.setBitmap(Bitmap(BITMAP_WASHING_MACHINE_BIG_ID));
            textAreaDeviceName.setTypedText(TypedText(T_HOMEUIDEVICENAMEMACHINE));
            break;
        case 2:
            imageDevice.setBitmap(Bitmap(BITMAP_HOME_TEMP_BIG_ID));
            textAreaDeviceName.setTypedText(TypedText(T_HOMEUIDEVICENAMECONTROL));
            break;
        }
    }
}

void DeviceInfo::setAlpha(uint8_t alpha)
{
    textAreaState1.setAlpha(alpha);
    textAreaState1Value.setAlpha(alpha);
    textAreaState2.setAlpha(alpha);
    textAreaState2Value.setAlpha(alpha);
    textAreaState3.setAlpha(alpha);
    textAreaState3Value.setAlpha(alpha);
    imageDevice.setAlpha(alpha);
    textAreaDeviceName.setAlpha(alpha);
}

uint8_t DeviceInfo::getAlpha()
{
    return imageDevice.getAlpha();
}


void DeviceInfo::updateTextLine1(bool show, Unicode::UnicodeChar* buffer1, Unicode::UnicodeChar* buffer2)
{
    textAreaState1.setVisible(show);
    textAreaState2Value.setVisible(show);

    Unicode::snprintf(textAreaState1Buffer, TEXTAREASTATE1_SIZE, buffer1);
    Unicode::snprintf(textAreaState1ValueBuffer, TEXTAREASTATE1VALUE_SIZE, buffer2);
}

void DeviceInfo::updateTextLine2(bool show, Unicode::UnicodeChar* buffer1, Unicode::UnicodeChar* buffer2)
{
    textAreaState2.setVisible(show);
    textAreaState2Value.setVisible(show);

    Unicode::snprintf(textAreaState2Buffer, TEXTAREASTATE2_SIZE, buffer1);
    Unicode::snprintf(textAreaState2ValueBuffer, TEXTAREASTATE2VALUE_SIZE, buffer2);
}

void DeviceInfo::updateTextLine3(bool show, Unicode::UnicodeChar* buffer1, Unicode::UnicodeChar* buffer2)
{
    textAreaState3.setVisible(show);
    textAreaState3Value.setVisible(show);

    Unicode::snprintf(textAreaState3Buffer, TEXTAREASTATE3_SIZE, buffer1);
    Unicode::snprintf(textAreaState3ValueBuffer, TEXTAREASTATE3VALUE_SIZE, buffer2);
}

void DeviceInfo::setLightState(bool show)
{
    fanLight = show;
}

void DeviceInfo::setRecipeState(bool show)
{
    recipeOnline = show;
}

void DeviceInfo::setOvenState(bool show)
{
    ovenState = show;
}

void DeviceInfo::setFanState(bool state)
{
    fanState = state;
}

void DeviceInfo::setFanSpeed(uint16_t speed)
{
    fanSpeed = speed;
}

void DeviceInfo::setBathtubOn(bool on)
{
    bathtubOn = on;
}

void DeviceInfo::setTempCelcius(bool temp)
{
    tempCelcius = temp;
}

void DeviceInfo::setJetOn(bool on)
{
    jetOn = on;
}

void DeviceInfo::setLightOn(bool on)
{
    lightOn = on;
}

void DeviceInfo::setTempValue(uint16_t temp)
{
    tempValue = temp;
}

void DeviceInfo::setJetLevel(uint16_t level)
{
    jetLevel = level;
}

void DeviceInfo::setSelectedOvenMode(uint16_t mode)
{
    selectedOvenMode = mode;
}

void DeviceInfo::setOvenTimerHour(int16_t hour)
{
    ovenTimerHour = hour;
}


void DeviceInfo::setOvenTimerMinute(int16_t minute)
{
    ovenTimerMinute = minute;
}

void DeviceInfo::setOvenTemperature(uint16_t temp)
{
    ovenTemperature = temp;
}

void DeviceInfo::setWashingProgram(uint16_t program)
{
    currentProgram = program;
}

void DeviceInfo::setWashingTime(uint16_t time)
{
    washingTime = time;
}
