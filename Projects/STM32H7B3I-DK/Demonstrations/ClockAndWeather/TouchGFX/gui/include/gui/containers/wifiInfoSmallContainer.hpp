#ifndef WIFIINFOSMALLCONTAINER_HPP
#define WIFIINFOSMALLCONTAINER_HPP

#include <gui_generated/containers/wifiInfoSmallContainerBase.hpp>

class wifiInfoSmallContainer : public wifiInfoSmallContainerBase
{
public:
    wifiInfoSmallContainer();
    virtual ~wifiInfoSmallContainer() {}

    virtual void initialize();

    void setWifiSSID(touchgfx::Unicode::UnicodeChar* name);
    void setEncryption(const touchgfx::Unicode::UnicodeChar* encryption);
    void setSignalStrength(uint16_t strength);

    void setId(int newId);
    int getId();

    virtual void elementSelected();
    void setAction(GenericCallback< wifiInfoSmallContainer& >& callback);

protected:

    GenericCallback< wifiInfoSmallContainer& >* viewCallback;
    int id;

};

#endif // WIFIINFOSMALLCONTAINER_HPP
