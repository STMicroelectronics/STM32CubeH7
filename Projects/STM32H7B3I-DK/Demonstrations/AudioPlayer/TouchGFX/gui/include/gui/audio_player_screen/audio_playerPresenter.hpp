#ifndef AUDIO_PLAYERPRESENTER_HPP
#define AUDIO_PLAYERPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class audio_playerView;

class audio_playerPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    audio_playerPresenter(audio_playerView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~audio_playerPresenter() {};

    void setMusicVolume(int vol);

    void updateMusicDuration(int duration);
    void setMusicDuration(int length);
    void setSongDuration(int duration);

    void setPlaySong(bool play);
    bool getPlaySong();

    int getTrackLengthMinutes(int id);
    int getTrackLengthSeconds(int id);

    void setTrackLength(int id, int length);

    void getTrackName(touchgfx::Unicode::UnicodeChar* name, int id);
    void setTrackName(touchgfx::Unicode::UnicodeChar* name, int id);

    void setSelected(bool selected, int id);
    bool getSelected(int id);

    void setNumberOfSongs(int songs);
    int getNumberOfSongs();

    void nextSongPressed();
    void previousSongPressed();

    void setCurrentSong(int song);
    int getCurrentSong();

    void setRepeat(bool set);
    void setShuffle(bool set);

    virtual void psCountUpdate(int cnt);
    virtual void setPlayingGraphics(bool playing);

    void cancelPowerSave();
    void powerSavePressed();

    void returnToMenuPressed();
    virtual void noMediaFilesDetected(bool filesDetected);

private:
    audio_playerPresenter();

    audio_playerView& view;
};


#endif // AUDIO_PLAYERPRESENTER_HPP
