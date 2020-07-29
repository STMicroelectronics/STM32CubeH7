#include <gui/containers/iconContainer.hpp>
#include "BitmapDatabase.hpp"

iconContainer::iconContainer() : index(0)
{

}

void iconContainer::initialize()
{
    iconContainerBase::initialize();
}

void iconContainer::setScale(float scale)
{
    iconSides.setScale(scale * 2);
    iconCenter.setScale(scale);
}

float iconContainer::getScale()
{
    return iconCenter.getScale();
}

void iconContainer::setSideAlpha(uint8_t alpha)
{
    iconSides.setAlpha(alpha);

    if (alpha >= 255)
    {
        iconCenter.setVisible(false);
    }
    else
    {
        iconCenter.setVisible(true);
    }

    if (alpha == 0)
    {
        iconSides.setVisible(false);
    }
    else
    {
        iconSides.setVisible(true);
    }
}

void iconContainer::setBitmap(int index)
{
    this->index = index;

    iconCenter.setBitmap(Bitmap(BITMAP_ICON_CENTER_01_ID + index));
    iconCenter.setWidth(200);
    iconCenter.setHeight(200);

    iconSides.setBitmap(Bitmap(BITMAP_SMALL_ICON_CENTER_01_ID + index));
    iconSides.setWidth(200);
    iconSides.setHeight(200);

}

int iconContainer::getIndex()
{
    return index;
}
