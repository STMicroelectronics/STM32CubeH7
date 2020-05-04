#ifndef AUDIO_PLAYER_HPP
#define AUDIO_PLAYER_HPP

#include <touchgfx/hal/Types.hpp>

class AudioPlayer
{
public:
    typedef enum
    {
        _100HZ,
        _1KHZ,
        _10KHZ,
        _20KHZ,
        LOUDNESS
    } EqualizerSetting;

    AudioPlayer();
    ~AudioPlayer() {}
    void init(int volume);
    void deInit();
    void selectFile(char* file);
    void play();
    void stop();
    void pause();
    void setVolume(int volume);
    bool isPlaying() const;
    bool isPaused() const;
    int16_t getEqualizerSetting(const EqualizerSetting);
    void setEqualizerSetting(const EqualizerSetting, const int16_t);
    uint32_t getSongProgressMs();
    uint32_t getSongDurationMs();
    void setSongProgressPercentage(uint32_t prog);
    
private:
    char* filename;
};
#endif /* AUDIO_PLAYER_HPP */
