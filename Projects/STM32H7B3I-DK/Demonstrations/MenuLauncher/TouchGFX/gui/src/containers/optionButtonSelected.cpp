#include <gui/containers/optionButtonSelected.hpp>
#include "BitmapDatabase.hpp"

optionButtonSelected::optionButtonSelected()
{

}

void optionButtonSelected::initialize()
{
    optionButtonSelectedBase::initialize();
}

void optionButtonSelected::setColor(int colorVal)
{
    boxSmall.setBitmap(Bitmap(BITMAP_SMALL_BOX_01_ID + colorVal));
    boxLarge.setBitmap(Bitmap(BITMAP_BOX_01_ID + colorVal));
}

void optionButtonSelected::setModule(int index)
{
    iconSmall.setBitmap(Bitmap(BITMAP_SMALL_ICON_01_ID + index));
    iconLarge.setBitmap(Bitmap(BITMAP_ICON_01_ID + index));
}

void optionButtonSelected::setSize(bool large)
{
    boxSmall.setVisible(!large);
    iconSmall.setVisible(!large);
        
    boxLarge.setVisible(large);    
    iconLarge.setVisible(large);
}

void optionButtonSelected::setCurrrentItem(int currentItem)
{
    this->currentItem = currentItem;
}

int optionButtonSelected::getCurrrentItem()
{
    return currentItem;
}
