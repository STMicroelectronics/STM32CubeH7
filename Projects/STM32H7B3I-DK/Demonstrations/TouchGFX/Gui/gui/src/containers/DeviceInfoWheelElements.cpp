#include <gui/containers/DeviceInfoWheelElements.hpp>

DeviceInfoWheelElements::DeviceInfoWheelElements()
{    
    
}

void DeviceInfoWheelElements::initialize()
{
    DeviceInfoWheelElementsBase::initialize();

    deviceInfoElement.initialize();
    deviceInfoElement.setXY(0, 0);
    add(deviceInfoElement);
}

void DeviceInfoWheelElements::updateDeviceInfo(int16_t item, bool kitchen)
{
    deviceInfoElement.updateDeviceInfo(item, kitchen);
}

void DeviceInfoWheelElements::startFadeAnimation(uint8_t endAlpha, uint16_t duration, uint16_t delay, EasingEquation alphaProgressionEquation)
{
    deviceInfoElement.setFadeAnimationDelay(delay);
    deviceInfoElement.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
}
void DeviceInfoWheelElements::setAlpha(uint8_t alpha)
{
    deviceInfoElement.setAlpha(alpha);
}

uint8_t DeviceInfoWheelElements::getAlpha()
{
    return deviceInfoElement.getAlpha();
}

void DeviceInfoWheelElements::updateTextLine1(bool show, Unicode::UnicodeChar* buffer1, Unicode::UnicodeChar* buffer2)
{
    deviceInfoElement.updateTextLine1(show, buffer1, buffer2);
}

void DeviceInfoWheelElements::updateTextLine2(bool show, Unicode::UnicodeChar* buffer1, Unicode::UnicodeChar* buffer2)
{
    deviceInfoElement.updateTextLine2(show, buffer1, buffer2);
}

void DeviceInfoWheelElements::updateTextLine3(bool show, Unicode::UnicodeChar* buffer1, Unicode::UnicodeChar* buffer2)
{
    deviceInfoElement.updateTextLine3(show, buffer1, buffer2);
}
