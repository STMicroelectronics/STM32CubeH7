#ifndef DEVICEINFOWHEELELEMENTS_HPP
#define DEVICEINFOWHEELELEMENTS_HPP

#include <gui_generated/containers/DeviceInfoWheelElementsBase.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>
#include <gui/containers/DeviceInfo.hpp>

class DeviceInfoWheelElements : public DeviceInfoWheelElementsBase
{
public:
    DeviceInfoWheelElements();
    virtual ~DeviceInfoWheelElements() {}

    virtual void initialize();

    void updateDeviceInfo(int16_t item, bool kitchen);
    void startFadeAnimation(uint8_t endAlpha, uint16_t duration, uint16_t delay = 0, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);
    void setAlpha(uint8_t alpha);
    uint8_t getAlpha();

    void updateTextLine1(bool show, Unicode::UnicodeChar* buffer1, Unicode::UnicodeChar* buffer2);
    void updateTextLine2(bool show, Unicode::UnicodeChar* buffer1, Unicode::UnicodeChar* buffer2);
    void updateTextLine3(bool show, Unicode::UnicodeChar* buffer1, Unicode::UnicodeChar* buffer2);

protected:

    FadeAnimator<DeviceInfo> deviceInfoElement;
};

#endif // DEVICEINFOWHEELELEMENTS_HPP
