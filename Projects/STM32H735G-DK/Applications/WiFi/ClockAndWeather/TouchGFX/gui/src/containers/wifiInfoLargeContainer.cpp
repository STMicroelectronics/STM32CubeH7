#include <gui/containers/wifiInfoLargeContainer.hpp>
#include <gui/common/enums.hpp>
#include "BitmapDatabase.hpp"

wifiInfoLargeContainer::wifiInfoLargeContainer()
{

}

void wifiInfoLargeContainer::initialize()
{
    wifiInfoLargeContainerBase::initialize();
}


void wifiInfoLargeContainer::setWifiSSID(touchgfx::Unicode::UnicodeChar* name)
{
    Unicode::strncpy(wifiSSIDBuffer, name, WIFISSID_SIZE);
    wifiSSID.invalidate();
}

void wifiInfoLargeContainer::setEncryption(const touchgfx::Unicode::UnicodeChar* encryptionType)
{
    Unicode::strncpy(encryptionBuffer, encryptionType, ENCRYPTION_SIZE);
    encryption.invalidate();
}

void wifiInfoLargeContainer::setSignalStrength(uint16_t strength)
{
    switch (strength)
    {
    case WIFI_STRENGTH::MINIMAL:
        signalStrengthIcon.setBitmap(Bitmap(BITMAP_WIFI_DOT_SETTINGS_ID));
        break;

    case WIFI_STRENGTH::LOW:
        signalStrengthIcon.setBitmap(Bitmap(BITMAP_WIFI_LOW_SETTINGS_ID));
        break;

    case WIFI_STRENGTH::MEDIUM:
        signalStrengthIcon.setBitmap(Bitmap(BITMAP_WIFI_MID_SETTINGS_ID));
        break;

    case WIFI_STRENGTH::HIGH:
        signalStrengthIcon.setBitmap(Bitmap(BITMAP_WIFI_FULL_SETTINGS_ID));
        break;
    }
    signalStrengthIcon.invalidate();
}

void wifiInfoLargeContainer::elementSelected()
{
    // Inform the view of the event
    if (viewCallback->isValid())
    {
        viewCallback->execute(*this);
    }
}

void wifiInfoLargeContainer::setAction(GenericCallback< wifiInfoLargeContainer& >& callback)
{
    viewCallback = &callback;
}

void wifiInfoLargeContainer::setId(int newId)
{
    id = newId;
}

int wifiInfoLargeContainer::getId()
{
    return id;
}
