#include <gui/containers/wifiMenuContainer.hpp>

#include<gui/common/structs.hpp>

wifiMenuContainer::wifiMenuContainer() :
    wifiListElementClickedCallback(this, &wifiMenuContainer::wifiListElementClicked)
{

}

void wifiMenuContainer::initialize()
{
    wifiMenuContainerBase::initialize();

    wifiList.setWidth(wifiListElements[0].getWidth());
    wifiListContainer.setScrollbarPadding(4);
    wifiListContainer.setScrollbarsAlpha(100);
    wifiListContainer.setScrollbarWidth(5);
}

void wifiMenuContainer::updateWifiList(struct  wifiData *data, uint16_t numberOfPoints)
{
    wifiList.removeAll();
    
    for (int cnt = 0; cnt < numberOfPoints; cnt++)
    {
        wifiListElements[cnt].setWifiSSID(data[cnt].ssid);
        wifiListElements[cnt].setEncryption(data[cnt].encryption);
        wifiListElements[cnt].setSignalStrength(data[cnt].signalStrength);
        wifiListElements[cnt].setId(cnt);
        wifiList.add(wifiListElements[cnt]);

        wifiListElements[cnt].setAction(wifiListElementClickedCallback);
    }

    wifiListContainer.invalidate();
}

void wifiMenuContainer::wifiListElementClicked(wifiInfoLargeContainer& element)
{
    if (viewCallback->isValid())
    {
        viewCallback->execute(element.getId());
    }
}

void wifiMenuContainer::setAction(GenericCallback< int >& callback)
{
    viewCallback = &callback;
}
