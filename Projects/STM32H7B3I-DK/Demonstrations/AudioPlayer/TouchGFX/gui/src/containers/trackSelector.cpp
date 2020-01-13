#include <gui/containers/trackSelector.hpp>

trackSelector::trackSelector() :
    trackListItemSelectedCallback(this, &trackSelector::trackListItemSelectedHandler),
    trackListPLayPausePressedCallback(this, &trackSelector::trackListPLayPausePressed)
{

}

void trackSelector::initialize()
{
    trackSelectorBase::initialize();

    trackList.setItemSelectedCallback(trackListItemSelectedCallback);

    for (int cnt = 0; cnt < trackListListItems.getNumberOfDrawables(); cnt++)
    {
        trackListListItems[cnt].setPlayPauseAction(trackListPLayPausePressedCallback);
    }
}

void trackSelector::trackListUpdateItem(track& item, int16_t itemIndex)
{
    item.setCurrentItem(itemIndex);

    if (action && action->isValid())
    {
        action->execute(item, itemIndex);
    }
}

void trackSelector::setUpdatePlaylist(GenericCallback< track&, int >& callback)
{
    action = &callback;
}

void trackSelector::setTrackListSize(int size)
{
    trackList.setNumberOfItems(size);
}

void trackSelector::refreshTrackList()
{
    for (int cnt = 0; cnt < trackListListItems.getNumberOfDrawables(); cnt++)
    {
        trackListUpdateItem(trackListListItems[cnt], trackListListItems[cnt].getCurrentItem());
    }
    trackList.invalidate();
}

void trackSelector::trackListItemSelectedHandler(int16_t selectedItem)
{
    if (itemSelectedAction && itemSelectedAction->isValid())
    {
        itemSelectedAction->execute(selectedItem);
    }       
}

void trackSelector::setSelectedItemCallback(GenericCallback<int>& callback)
{
    itemSelectedAction = &callback;
}

void trackSelector::trackListPLayPausePressed(bool play)
{
    emitTrackPlayPausedPressedCallback(play);
}

void trackSelector::setRepeatButtonState(bool on)
{
    repeatButton.forceState(on);
    repeatButton.invalidate();
}

void trackSelector::setShuffleButtonState(bool on)
{
    shuffelButton.forceState(on);
    shuffelButton.invalidate();
}