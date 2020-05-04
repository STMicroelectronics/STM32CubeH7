#include <gui/containers/optionButton.hpp>
#include "BitmapDatabase.hpp"

optionButton::optionButton()
{

}

void optionButton::initialize()
{
    optionButtonBase::initialize();
}

void optionButton::setColor(int colorVal)
{
    smallBox.setBitmap(Bitmap(BITMAP_SMALL_BOX_01_ID + colorVal));
    largeBox.setBitmap(Bitmap(BITMAP_BOX_01_ID + colorVal));
}
void optionButton::setModule(int index)
{
    smallIcon.setBitmap(Bitmap(BITMAP_SMALL_ICON_01_ID + index));
    largeIcon.setBitmap(Bitmap(BITMAP_ICON_01_ID + index));
}

void optionButton::setSize(bool large)
{
    largeElements.setVisible(large);
    smallElements.setVisible(!large);
}


void optionButton::setCurrrentItem(int currentItem)
{
    this->currentItem = currentItem;
}

int optionButton::getCurrrentItem()
{
    return currentItem;
}
