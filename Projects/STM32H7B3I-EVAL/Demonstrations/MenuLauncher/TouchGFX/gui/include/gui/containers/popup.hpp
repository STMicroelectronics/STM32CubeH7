#ifndef POPUP_HPP
#define POPUP_HPP

#include <gui_generated/containers/popupBase.hpp>

class popup : public popupBase
{
public:
    popup();
    virtual ~popup() {}

    virtual void initialize();

    void setUpContent(int module);
    void setFirmwareVersion(const char* fwVersion);

protected:
};

#endif // POPUP_HPP
