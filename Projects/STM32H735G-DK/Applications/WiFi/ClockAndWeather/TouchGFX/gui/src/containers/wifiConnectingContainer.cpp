#include <gui/containers/wifiConnectingContainer.hpp>
#include "BitmapDatabase.hpp"

wifiConnectingContainer::wifiConnectingContainer()
{

}

void wifiConnectingContainer::initialize()
{
    wifiConnectingContainerBase::initialize();
}

void wifiConnectingContainer::resetContainer()
{
    tickCnt = 0;
    animationCnt = 0;
    connecting = true;

    connectingImage.setBitmap(Bitmap(BITMAP_WIFI_LEVEL_1_ID));
    connectingImage.setVisible(true);

    Unicode::snprintf(connectingTextBuffer, CONNECTINGTEXT_SIZE, ".");
    connectingText.setVisible(true);

    connectingImage.invalidate();
    connectingText.invalidate();

    wifiConnectionStatus.setVisible(false);
    errorImage.setVisible(false);
    wifiRetryButton.setVisible(false);

    wifiConnectionStatus.invalidate();
    errorImage.invalidate();
    wifiRetryButton.invalidate();
}

void wifiConnectingContainer::connectionUnsuccesfull()
{
    connecting = false;

    connectingImage.setVisible(false);
    connectingText.setVisible(false);

    connectingImage.invalidate();
    connectingText.invalidate();

    wifiConnectionStatus.setVisible(true);
    errorImage.setVisible(true);
    wifiRetryButton.setVisible(true);

    wifiConnectionStatus.invalidate();
    errorImage.invalidate();
    wifiRetryButton.invalidate();

}

void wifiConnectingContainer::tickEvent()
{
    if (tickCnt == 30 && connecting)
    {
        switch (animationCnt)
        {
        case 0:
            connectingImage.setBitmap(Bitmap(BITMAP_WIFI_LEVEL_1_ID));
            Unicode::snprintf(connectingTextBuffer, CONNECTINGTEXT_SIZE, ".");
            animationCnt++;
            break;

        case 1:
            connectingImage.setBitmap(Bitmap(BITMAP_WIFI_LEVEL_2_ID));
            Unicode::snprintf(connectingTextBuffer, CONNECTINGTEXT_SIZE, "..");
            animationCnt++;
            break;

        case 2:
            connectingImage.setBitmap(Bitmap(BITMAP_WIFI_LEVEL_3_ID));
            Unicode::snprintf(connectingTextBuffer, CONNECTINGTEXT_SIZE, "...");
            animationCnt++;
            break;

        case 3:
            connectingImage.setBitmap(Bitmap(BITMAP_WIFI_LEVEL_4_ID));
            Unicode::snprintf(connectingTextBuffer, CONNECTINGTEXT_SIZE, "....");
            animationCnt = 0;
            break;

        default:
            break;
        }
        connectingImage.invalidate();
        connectingText.invalidate();

        tickCnt = 0;
    }
    else if (connecting)
    {
        tickCnt++;
    }
}
