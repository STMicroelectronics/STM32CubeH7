#include <gui/containers/wifiInfoSmallContainer.hpp>
#include <BitmapDatabase.hpp>

#include "gui/common/enums.hpp"

wifiInfoSmallContainer::wifiInfoSmallContainer()
{

}

void wifiInfoSmallContainer::initialize()
{
    wifiInfoSmallContainerBase::initialize();
}

void wifiInfoSmallContainer::setWifiSSID(touchgfx::Unicode::UnicodeChar* name)
{
    Unicode::strncpy(wifiSSIDBuffer, name, WIFISSID_SIZE);
    wifiSSID.invalidate();
}

void wifiInfoSmallContainer::setEncryption(const touchgfx::Unicode::UnicodeChar* encryptionType)
{
    Unicode::strncpy(encryptionBuffer, encryptionType, ENCRYPTION_SIZE);
    encryption.invalidate();
}

void wifiInfoSmallContainer::setSignalStrength(uint16_t strength)
{
    switch (strength)
    {
    case WIFI_STRENGTH::MINIMAL:
        signalStrengthIcon.setBitmap(Bitmap(BITMAP_WIFI_DOT_ICON_2X_ID));
        break;

    case WIFI_STRENGTH::LOW:
        signalStrengthIcon.setBitmap(Bitmap(BITMAP_WIFI_LOW_ICON_2X_ID));
        break;

    case WIFI_STRENGTH::MEDIUM:
        signalStrengthIcon.setBitmap(Bitmap(BITMAP_WIFI_MID_ICON_2X_ID));
        break;

    case WIFI_STRENGTH::HIGH:
        signalStrengthIcon.setBitmap(Bitmap(BITMAP_WIFI_FULL_ICON_2X_ID));
        break;
    }
    signalStrengthIcon.invalidate();
}

void wifiInfoSmallContainer::setId(int newId)
{
    id = newId;
}

int wifiInfoSmallContainer::getId()
{
    return id;
}

void wifiInfoSmallContainer::elementSelected()
{
    // Inform the view of the event
    if (viewCallback->isValid())
    {
        viewCallback->execute(*this);
    }
}

void wifiInfoSmallContainer::setAction(GenericCallback< wifiInfoSmallContainer& >& callback)
{
    viewCallback = &callback;
}
