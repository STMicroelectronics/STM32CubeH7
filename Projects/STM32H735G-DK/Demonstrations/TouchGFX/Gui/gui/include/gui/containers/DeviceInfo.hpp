#ifndef DEVICEINFO_HPP
#define DEVICEINFO_HPP

#include <gui_generated/containers/DeviceInfoBase.hpp>

class DeviceInfo : public DeviceInfoBase
{
public:
    DeviceInfo();
    virtual ~DeviceInfo() {}

    virtual void initialize();

    void updateDeviceInfo(int16_t item, bool kitchen);    
    void setAlpha(uint8_t alpha);
    uint8_t getAlpha();

    void updateTextLine1(bool show, Unicode::UnicodeChar* buffer1, Unicode::UnicodeChar* buffer2);
    void updateTextLine2(bool show, Unicode::UnicodeChar* buffer1, Unicode::UnicodeChar* buffer2);
    void updateTextLine3(bool show, Unicode::UnicodeChar* buffer1, Unicode::UnicodeChar* buffer2);

    void setLightState(bool show);
    void setRecipeState(bool show);
    void setOvenState(bool show);
    void setFanState(bool state);
    void setFanSpeed(uint16_t speed);
    void setBathtubOn(bool on);
    void setTempCelcius(bool temp);
    void setJetOn(bool on);
    void setLightOn(bool on);
    void setTempValue(uint16_t temp);
    void setJetLevel(uint16_t level);
    void setSelectedOvenMode(uint16_t mode);
    void setOvenTimerHour(int16_t hour);
    void setOvenTimerMinute(int16_t minute);
    void setOvenTemperature(uint16_t temp);
    void setWashingProgram(uint16_t program);
    void setWashingTime(uint16_t time);

protected:

    /* Active kitchen elements */
    bool fanLight;
    bool recipeOnline;
    bool ovenState;

    bool fanState;
    uint16_t fanSpeed;

    /*Bathtub States*/
    bool bathtubOn;
    bool tempCelcius;
    bool jetOn;
    bool lightOn;

    uint16_t tempValue;
    uint16_t jetLevel;
    uint32_t colorCoordinateX;
    uint32_t colorCoordinateY;

    /*Oven States*/
    uint16_t selectedOvenMode;
    int16_t ovenTimerHour;
    int16_t ovenTimerMinute;
    uint16_t ovenTemperature;
    uint16_t currentProgram;
    uint16_t washingTime;
};

#endif // DEVICEINFO_HPP
