#ifndef WASHERWHEELCENTERITEM_HPP
#define WASHERWHEELCENTERITEM_HPP

#include <gui_generated/containers/WasherWheelCenterItemBase.hpp>

class WasherWheelCenterItem : public WasherWheelCenterItemBase
{
public:
    WasherWheelCenterItem();
    virtual ~WasherWheelCenterItem() {}

    virtual void initialize();

    void setIcon(Bitmap id);

protected:
};

#endif // WASHERWHEELCENTERITEM_HPP
