#ifndef AUDIO_PLAYERVIEW_HPP
#define AUDIO_PLAYERVIEW_HPP

#include <gui_generated/audio_player_screen/audio_playerViewBase.hpp>
#include <gui/audio_player_screen/audio_playerPresenter.hpp>
#include <gui/containers/track.hpp>


class audio_playerView : public audio_playerViewBase
{
public:
    audio_playerView();
    virtual ~audio_playerView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void volumeButtonPressed(bool value);
    virtual void volumeSliderChanged(uint16_t value);
    virtual void musicSliderChanged(uint16_t value);
    virtual void playPauseButtonPressed(bool value);
    virtual void trackListPlayPausePressed(bool value);
    virtual void nextSongPressed();
    virtual void previousSongPressed();
    virtual void shufflePressed(bool value);
    virtual void repeatPressed(bool value);
    virtual void albumButtonPressed();
    virtual void cancalPowerSavePressed();
    virtual void powerSavePressed();
    virtual void backButtonPressed();

    void updateMusicVolume(int volume);
    
    void updateMusicDuration(int totalDuration, int minutes, int seconds);
    void setMusicDuration(int setTotalLength, int minutes, int seconds);
    void setTrackListSize(int size);
    void refreshTrackList();

    void setActiveSong(int song);
    void setPlayingGraphics(bool playing);

    void psCountUpdate(int cnt);

    void noMediaFilesDetected(bool filesDetected);

protected:

    static const int VOLUME_SLIDER_X                = 721;
    static const int VOLUME_SLIDER_MOVE_DURATION    = 30;   

    void updateTrackListHandler(track& currentTrack, int trackId);
    Callback<audio_playerView, track&, int> updateTrackListCallback;

    void trackListItemSelectedHandler(int item);
    Callback<audio_playerView, int> trackListItemSelectedCallback;

    bool sliderIsMoved;

    int activeTrack;
    int currentSongMinutes;
    int currentSongSeconds;

};

#endif // AUDIO_PLAYERVIEW_HPP
