#include <gui/containers/WasherWheelItem.hpp>

WasherWheelItem::WasherWheelItem()
{

}

void WasherWheelItem::initialize()
{
    WasherWheelItemBase::initialize();
}

void WasherWheelItem::setIcon(Bitmap id)
{
    iconMode.setBitmap(id);
    iconMode.invalidate();
}
