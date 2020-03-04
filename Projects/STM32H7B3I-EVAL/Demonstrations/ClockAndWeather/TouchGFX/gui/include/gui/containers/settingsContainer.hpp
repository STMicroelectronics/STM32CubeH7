#ifndef SETTINGSCONTAINER_HPP
#define SETTINGSCONTAINER_HPP

#include <gui_generated/containers/settingsContainerBase.hpp>
#include <gui/containers/wifiInfoSmallContainer.hpp>

class settingsContainer : public settingsContainerBase
{
public:
    settingsContainer();
    virtual ~settingsContainer() {}

    virtual void initialize();

    bool getClockButtonState();
    bool getTemperatureButtonState();

    void updateWifiList(struct wifiData *data, int numberOfPoints);

    void wifiListElementClicked(wifiInfoSmallContainer& element);

    void setAction(GenericCallback<int>& callback);

protected:

    GenericCallback<int>* viewCallback;

    static const int MAX_FOR_PREVIEW = 4;
    static const int numberOfListElements = 20;
    wifiInfoSmallContainer wifiListElements[numberOfListElements];

    // Callback that is assigned to each list element
    Callback<settingsContainer, wifiInfoSmallContainer&> wifiListElementClickedCallback;
};

#endif // SETTINGSCONTAINER_HPP
