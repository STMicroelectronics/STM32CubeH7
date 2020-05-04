#include <gui/mainscreen_screen/mainScreenView.hpp>


#include <stdio.h>
#include <stdlib.h>

mainScreenView::mainScreenView() :
    itemSelectedCallback(this, &mainScreenView::itemSelectedHandler),
    animationEnddedCallback(this, &mainScreenView::animationEnddedHandler),
    animateToCallback(this, &mainScreenView::animateToHandler), 
    selectedModule(4),
    totalDrag(0),
    isDragged(false)
{

}

void mainScreenView::setupScreen()
{
    moduleSelectorWheel.animateToItem(selectedModule, 100);

    moduleSelectorWheel.setAnimationEndedCallback(animationEnddedCallback);
    moduleSelectorWheel.setAnimateToCallback(animateToCallback);
    moduleSelectorWheel.setItemSelectedCallback(itemSelectedCallback);
    moduleSelectorWheel.setNumberOfItems(NUMBER_OF_MODULES);
    
    for (int i = 0; i < moduleSelectorWheelListItems.getNumberOfDrawables(); i++)
    {
        if (moduleSelectorWheelListItems[i].getCurrrentItem() == moduleSelectorWheel.getSelectedItem())
        {
            moduleSelectorWheelListItems[i].setSize(true);
        }
    }
}

void mainScreenView::tearDownScreen()
{
    mainScreenViewBase::tearDownScreen();
}

void mainScreenView::itemSelectedHandler(int16_t item)
{
    if (selectedModule == item && !isDragged)
    {
        infoPopUp.setUpContent(selectedModule);

        infoPopUp.setVisible(true);
        infoPopUp.invalidate();

        moduleSelectorWheel.setTouchable(false);
    }
    
    isDragged = false;

    selectedModule = item;
}

void mainScreenView::popUpBackPressed()
{
    infoPopUp.setVisible(false);
    infoPopUp.invalidate();

    moduleSelectorWheel.setTouchable(true);
}

void mainScreenView::popUpStartPressed()
{
    presenter->moduleSelected(selectedModule);
}

void mainScreenView::setFirmwareVersion(const char* fwVersion)
{
    infoPopUp.setFirmwareVersion(fwVersion);
}

void mainScreenView::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::RELEASED)
    {
        totalDrag = 0;
        //isDragged = false;
    }
    View::handleClickEvent(evt);

}

void mainScreenView::handleDragEvent(const DragEvent& evt)
{
    totalDrag += evt.getDeltaX();
    isDragged = true;

    if (moduleSelectorWheel.isAnimating())
    {
        if (totalDrag > 6 || totalDrag < -6)
        {
            for (int i = 0; i < moduleSelectorWheelListItems.getNumberOfDrawables(); i++)
            {
                moduleSelectorWheelListItems[i].setSize(false);
            }
            View::handleDragEvent(evt);
        }
    }
    else
    {
        View::handleDragEvent(evt);
    }
}

void mainScreenView::moduleSelectorWheelUpdateItem(optionButtonSelected& item, int16_t itemIndex)
{
    boxColor[itemIndex] = rand() % 7;

    if (itemIndex == 0)
    {
        while (boxColor[NUMBER_OF_MODULES-1] == boxColor[itemIndex] || boxColor[itemIndex + 1] == boxColor[itemIndex])
        {
            boxColor[itemIndex] = rand() % 7;
        }
    }
    else if (itemIndex == NUMBER_OF_MODULES - 1)
    {
        while (boxColor[itemIndex - 1] == boxColor[itemIndex] || boxColor[0] == boxColor[itemIndex])
        {
            boxColor[itemIndex] = rand() % 7;
        }
    }
    else
    {
        while (boxColor[itemIndex - 1] == boxColor[itemIndex] || boxColor[ itemIndex + 1] == boxColor[itemIndex])
        {
            boxColor[itemIndex] = rand() % 7;
        }
    }

    item.setColor(boxColor[itemIndex]);
    
    item.setModule(itemIndex);
    item.setCurrrentItem(itemIndex);
}

void mainScreenView::animationEnddedHandler()
{
    int selectedItem = moduleSelectorWheel.getSelectedItem();

    for (int i = 0; i < moduleSelectorWheelListItems.getNumberOfDrawables(); i++)
    {
        if (moduleSelectorWheelListItems[i].getCurrrentItem() == selectedItem)
        {
            moduleSelectorWheelListItems[i].setSize(true);
            moduleSelectorWheelListItems[i].invalidate();
        }
    }
}

void mainScreenView::animateToHandler(int16_t itemSelected)
{
    for (int i = 0; i < moduleSelectorWheelListItems.getNumberOfDrawables(); i++)
    {
        moduleSelectorWheelListItems[i].setSize(false);
    }
}
