#include <gui/containers/trackSelector.hpp>

trackSelector::trackSelector() :
    videoListItemSelectedCallback(this, &trackSelector::videoListItemSelectedHandler),
    videoListPLayPausePressedCallback(this, &trackSelector::videoListPLayPausePressedHandler),
    currentSelectedVideo(0)
{
}

void trackSelector::initialize()
{
    trackSelectorBase::initialize();
       
    for (int cnt = 0; cnt < 20; cnt++)
    {
        if (cnt % 2)
        {
            thumbNails[cnt].setXY(162 * (cnt / 2), 103);
        }
        else
        {
            thumbNails[cnt].setXY(162 * (cnt / 2), 0);
        }

        thumbNails[cnt].setPlayPausePressedCallback(videoListPLayPausePressedCallback);
        thumbNails[cnt].setItemSelectedCallback(videoListItemSelectedCallback);

        thumbNails[cnt].setVisible(false);
        thumbNails[cnt].setId(cnt);
        thumbNailsContainer.add(thumbNails[cnt]);
    }

    thumbNails[currentSelectedVideo].setSelected(true);
}

void trackSelector::setupThumbnail(int id, bool used, Bitmap image)
{
    thumbNails[id].setVisible(used);
    thumbNails[id].setBitmap(image);
    thumbNails[id].invalidate();
}

void trackSelector::setSelectedThumbnail(int selected)
{
    thumbNails[currentSelectedVideo].setSelected(false);
    thumbNails[currentSelectedVideo].invalidate();

    currentSelectedVideo = selected;
    thumbNails[currentSelectedVideo].setSelected(true);
    thumbNails[currentSelectedVideo].invalidate();
}

void trackSelector::setTrackListSize(int size)
{
    
}

void trackSelector::refreshTrackList()
{
    
}

void trackSelector::videoListItemSelectedHandler(int selectedItem)
{
    setSelectedThumbnail(selectedItem);
    emitVideoSelectedCallback(selectedItem);
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

void trackSelector::videoListPLayPausePressedHandler(bool value)
{
    emitVideoPlayPausedPressedCallback(value);
}

void trackSelector::playPauseSelectedVideo(bool play)
{
    thumbNails[currentSelectedVideo].setPlayPause(play);
}
