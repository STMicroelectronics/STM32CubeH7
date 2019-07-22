#include <gui/containers/WasherWheelCenterItem.hpp>

WasherWheelCenterItem::WasherWheelCenterItem()
{

}

void WasherWheelCenterItem::initialize()
{
    WasherWheelCenterItemBase::initialize();
}

void WasherWheelCenterItem::setIcon(Bitmap id)
{
    iconMode.setBitmap(id);
    iconMode.invalidate();
}
