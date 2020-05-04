#ifndef MODEL_HPP
#define MODEL_HPP

#include <touchgfx/Unicode.hpp>

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    void setMusicVolume(int volume);
    int getMusicVolume();

    void setSongLength(int length);
    int getSongLength();

    void setSongDuration(int duration);
    int getSongDuration();

    void setPlaySong(bool play);
    bool getPlaySong();

    int getTrackLength(int id);
    void setTrackLength(int length, int id);

    void getTrackName(touchgfx::Unicode::UnicodeChar* name, int id);
    void setTrackName(touchgfx::Unicode::UnicodeChar* name, int id);

    void setSelected(bool selected, int id);
    bool getSelected(int id);

    int getNumberOfSongs() { return numberOfSongs; }
    void setNumberOfSongs(int number);

    void setCurrentSong(int song);
    int getCurrentSong();

    void setRepeat(bool set);
    bool getRepeat();

    void setShuffle(bool set);
    bool getShuffle();

    void cancelPowerSave();
    void powerSavePressed();

    void returnToMenuLanucher();
    void noMediaFileDetected(bool filesDetected);

    void audioPlayerInit();
    void audioPlayerDeInit();
    void setTrackList();

protected:
    ModelListener* modelListener;

    static const int MAX_SONG_NAME_LENGTH = 30;
    static const int MAX_NUMBER_OF_SONGS = 30;
    static const int TICK_COUNT = 60;

    struct trackList
    {
        touchgfx::Unicode::UnicodeChar  songName[MAX_SONG_NAME_LENGTH];
        int                             trackLength;
        int                             id;
        bool                            selected;
    };

    trackList tracks[MAX_NUMBER_OF_SONGS];

    int musicVolume;
    int songLength;
    int songDuration;
    bool playSong;
    bool psCountDown;

    int numberOfSongs;
    int currentSong;

    bool repeat;
    bool shuffle;

    int tickCnt;
    int psTicks;
    int psCountDownCnt;
    int elapsedTime;
};

#endif // MODEL_HPP
