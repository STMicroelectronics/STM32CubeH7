#ifndef INFOBOX_HPP
#define INFOBOX_HPP

#include <gui_generated/containers/infoBoxBase.hpp>

class infoBox : public infoBoxBase
{
public:
    infoBox();
    virtual ~infoBox() {}

    virtual void initialize();

    void setUpContent(int module);
    void setFirmwareVersion(const char* fwVersion);

protected:

};

#endif // INFOBOX_HPP
