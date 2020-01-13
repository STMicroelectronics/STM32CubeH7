#ifndef WASHERWHEELITEM_HPP
#define WASHERWHEELITEM_HPP

#include <gui_generated/containers/WasherWheelItemBase.hpp>

class WasherWheelItem : public WasherWheelItemBase
{
public:
    WasherWheelItem();
    virtual ~WasherWheelItem() {}

    virtual void initialize();

    void setIcon(Bitmap id);

    BitmapId getIcon()
    {
        return iconMode.getBitmap();
    }

protected:
};

#endif // WASHERWHEELITEM_HPP
