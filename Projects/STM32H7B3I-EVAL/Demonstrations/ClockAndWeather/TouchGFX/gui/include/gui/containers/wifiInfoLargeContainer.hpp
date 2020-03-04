#ifndef WIFIINFOLARGECONTAINER_HPP
#define WIFIINFOLARGECONTAINER_HPP

#include <gui_generated/containers/wifiInfoLargeContainerBase.hpp>

class wifiInfoLargeContainer : public wifiInfoLargeContainerBase
{
public:
    wifiInfoLargeContainer();
    virtual ~wifiInfoLargeContainer() {}

    virtual void initialize();

    void setWifiSSID(touchgfx::Unicode::UnicodeChar* name);
    void setEncryption(const touchgfx::Unicode::UnicodeChar* encryptionType);
    void setSignalStrength(uint16_t strength);

    virtual void elementSelected();
    void setAction(GenericCallback< wifiInfoLargeContainer& >& callback);

    void setId(int newId);
    int getId();

protected:

    GenericCallback< wifiInfoLargeContainer& >* viewCallback;
    int id;
};

#endif // WIFIINFOLARGECONTAINER_HPP
