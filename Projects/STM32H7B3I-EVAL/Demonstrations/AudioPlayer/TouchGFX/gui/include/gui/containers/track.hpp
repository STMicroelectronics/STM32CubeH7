#ifndef TRACK_HPP
#define TRACK_HPP

#include <gui_generated/containers/trackBase.hpp>

class track : public trackBase
{
public:
    track();
    virtual ~track() {}

    virtual void initialize();

    void setTrackNumber(int number);
    void setTrackName(touchgfx::Unicode::UnicodeChar* name);
    void setTrackLengthDuration(int minutes, int seconds);
    void setTrackPlaying(bool playing);
    void setSelectedTrack(bool selected);
    void setCurrentItem(int item);
    int getCurrentItem();

    virtual void playPausePressed(bool value);

    void setPlayPauseAction(GenericCallback<bool> &callback);

protected:

    GenericCallback<bool>* playAction;
    int currentItem;
};

#endif // TRACK_HPP
