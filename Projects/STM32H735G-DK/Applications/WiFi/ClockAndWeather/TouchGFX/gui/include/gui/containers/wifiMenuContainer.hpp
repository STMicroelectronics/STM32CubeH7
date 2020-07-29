#ifndef WIFIMENUCONTAINER_HPP
#define WIFIMENUCONTAINER_HPP

#include <gui_generated/containers/wifiMenuContainerBase.hpp>
#include <gui/containers/wifiInfoLargeContainer.hpp>

class wifiMenuContainer : public wifiMenuContainerBase
{
public:
    wifiMenuContainer();
    virtual ~wifiMenuContainer() {}

    virtual void initialize();
    void updateWifiList(struct  wifiData *data, uint16_t numberOfPoints);

    void setAction(GenericCallback<int>& callback);

    void wifiListElementClicked(wifiInfoLargeContainer& element);

protected:

    GenericCallback<int>* viewCallback;

    static const int numberOfListElements = 20;
    wifiInfoLargeContainer wifiListElements[numberOfListElements];

    // Callback that is assigned to each list element
    Callback<wifiMenuContainer, wifiInfoLargeContainer&> wifiListElementClickedCallback;
};

#endif // WIFIMENUCONTAINER_HPP
