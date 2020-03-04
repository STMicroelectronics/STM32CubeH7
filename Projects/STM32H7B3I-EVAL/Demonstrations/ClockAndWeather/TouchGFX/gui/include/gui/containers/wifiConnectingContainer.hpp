#ifndef WIFICONNECTINGCONTAINER_HPP
#define WIFICONNECTINGCONTAINER_HPP

#include <gui_generated/containers/wifiConnectingContainerBase.hpp>

class wifiConnectingContainer : public wifiConnectingContainerBase
{
public:
    wifiConnectingContainer();
    virtual ~wifiConnectingContainer() {}

    virtual void initialize();

    void tickEvent();
    void resetContainer();
    void connectionUnsuccesfull();
    void connectionSuccesfull(int signalStrength);

protected:

    int tickCnt;
    int animationCnt;
    bool connecting;
};

#endif // WIFICONNECTINGCONTAINER_HPP
