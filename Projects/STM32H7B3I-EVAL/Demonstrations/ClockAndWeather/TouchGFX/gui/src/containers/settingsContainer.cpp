#include <gui/containers/settingsContainer.hpp>

settingsContainer::settingsContainer()  :
    wifiListElementClickedCallback(this, &settingsContainer::wifiListElementClicked)
{

}

void settingsContainer::initialize()
{
    settingsContainerBase::initialize();
 
}

bool settingsContainer::getClockButtonState()
{
    return clockModeButton.getState();
}

bool settingsContainer::getTemperatureButtonState()
{
    return temperatureUnitButton.getState();
}

void settingsContainer::updateWifiList(struct wifiData *data, int numberOfPoints)
{   
    wifiList.removeAll();
    
    int cnt = 0;
    while (cnt < numberOfPoints && cnt < MAX_FOR_PREVIEW)
    {
        wifiListElements[cnt].setWifiSSID(data[cnt].ssid);
        wifiListElements[cnt].setEncryption(data[cnt].encryption);
        wifiListElements[cnt].setSignalStrength(data[cnt].signalStrength);
        wifiListElements[cnt].setId(cnt);
        wifiList.add(wifiListElements[cnt]);

        wifiListElements[cnt].setAction(wifiListElementClickedCallback);

        cnt++;
    }

    moreNetweorksButton.setVisible(MAX_FOR_PREVIEW < numberOfPoints);
    moreNetweorksButton.invalidate();
}

void settingsContainer::wifiListElementClicked(wifiInfoSmallContainer& element)
{  
    if (viewCallback->isValid())
    {
        viewCallback->execute(element.getId());
    }
}

void settingsContainer::setAction(GenericCallback< int >& callback)
{
    viewCallback = &callback;
}
