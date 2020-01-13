#include <gui/containers/track.hpp>
#include <touchgfx/Color.hpp>

track::track()
{

}

void track::initialize()
{
    trackBase::initialize();
}

void track::setTrackNumber(int number)
{
    Unicode::snprintf(trackNumberBuffer, TRACKNUMBER_SIZE, "%d", number);
}

void track::setTrackName(touchgfx::Unicode::UnicodeChar* name)
{
    Unicode::strncpy(trackNameBuffer, name, TRACKNAME_SIZE);
}

void track::setTrackLengthDuration(int minutes, int seconds)
{
    Unicode::snprintf(trackDurationBuffer, TRACKDURATION_SIZE, "%.2d:%.2d", minutes, seconds);
}

void track::setTrackPlaying(bool playing)
{
    playPauseButton.forceState(playing);
}

void track::setSelectedTrack(bool selected)
{
    if (selected)
    {
        playPauseButton.setVisible(true);
        selectedTrackBorder.setVisible(true);
        border.setVisible(false);
        trackDuration.setColor(Color::getColorFrom24BitRGB(0, 255, 198));
        trackNumber.setColor(Color::getColorFrom24BitRGB(0, 255, 198));
    }
    else
    {
        playPauseButton.setVisible(false);
        selectedTrackBorder.setVisible(false);
        border.setVisible(true);
        trackDuration.setColor(Color::getColorFrom24BitRGB(85, 81, 120));
        trackNumber.setColor(Color::getColorFrom24BitRGB(177, 174, 203));
    }
}

void track::setCurrentItem(int item)
{
    currentItem = item;
}

int track::getCurrentItem()
{
    return currentItem;
}

void track::playPausePressed(bool value)
{
    if (playAction && playAction->isValid())
    {
        playAction->execute(value);
    }
}

void track::setPlayPauseAction(GenericCallback<bool> &callback)
{
    playAction = &callback;
}