

#include <gui/audio_player_screen/AudioPlayer.hpp>

#ifndef SIMULATOR
#include "audio_player_app.h"
static WAV_InfoTypedef         WavInfo;
extern "C"
{
    void GUI_AUDIO_Resume();
    void GUI_AUDIO_Stop();
    void GUI_AUDIO_Pause();
}
AudioPlayer::AudioPlayer() : filename(0)
{

}

void AudioPlayer::init(int volume)
{
    AUDIOPLAYER_Init(volume);
}

void AudioPlayer::deInit()
{
    AUDIOPLAYER_DeInit();
}

void AudioPlayer::setVolume(int volume)
{
    AUDIOPLAYER_SetVolume(volume);
}

void AudioPlayer::selectFile(char *file)
{
    bool autoPlay = isPlaying();
    if (filename)
    {
        stop();
    }
    filename = file;
    AUDIOPLAYER_GetFileInfo((uint8_t *)filename, &WavInfo);
    AUDIOPLAYER_SelectFile((uint8_t *)filename);
    if (autoPlay)
    {
        play();
    }
}

void AudioPlayer::play()
{
    AUDIOPLAYER_Play(WavInfo.SampleRate);
}

void AudioPlayer::stop()
{
    filename = 0;
    AUDIOPLAYER_Stop();
}

void AudioPlayer::pause()
{
    if (AUDIOPLAYER_GetState()  == AUDIOPLAYER_PLAY)
    {
        AUDIOPLAYER_Pause();
    }
    else if (AUDIOPLAYER_GetState()  == AUDIOPLAYER_PAUSE)
    {
        AUDIOPLAYER_Resume();
    }
}

bool AudioPlayer::isPlaying() const
{
    return (AUDIOPLAYER_GetState()  == AUDIOPLAYER_PLAY);
}

bool AudioPlayer::isPaused() const
{
    return (AUDIOPLAYER_GetState()  == AUDIOPLAYER_PAUSE);
}

int16_t AudioPlayer::getEqualizerSetting(const EqualizerSetting setting)
{
#ifdef __GNUC__
    return 0;
#else
    if (setting == LOUDNESS)
    {
        return AUDIOPLAYER_GetLoudnessGain();
    }
    else
    {
        return AUDIOPLAYER_GetEqGain(setting);
    }
#endif
}

void AudioPlayer::setEqualizerSetting(const EqualizerSetting setting, const int16_t value)
{

#ifndef __GNUC__
    if (setting == LOUDNESS)
    {
        AUDIOPLAYER_SetLoudnessGain(value);
    }
    else
    {
        AUDIOPLAYER_SetEqGain(setting, value);
    }
#endif

}
uint32_t AudioPlayer::getSongProgressMs()
{
    if (filename)
    {
        uint64_t currPtr = AUDIOPLAYER_GetProgress();
        return 1000 * (currPtr / 4 / WavInfo.SampleRate);
    }
    return 0;
}

uint32_t AudioPlayer::getSongDurationMs()
{
    return (WavInfo.FileSize / 4 / WavInfo.SampleRate) * 1000;
}

void AudioPlayer::setSongProgressPercentage(uint32_t progress)
{
    AUDIOPLAYER_SetPosition(progress);
}

#else
static int isplaying = 0;
static uint32_t progress = 0;
AudioPlayer::AudioPlayer()
{

}

void AudioPlayer::init(int volume)
{
    isplaying = 0;
    progress = 0;
}

void AudioPlayer::deInit()
{

}

void AudioPlayer::selectFile(char *filename)
{
    progress = 0;
}

void AudioPlayer::play()
{
    isplaying = 1;
}

void AudioPlayer::stop()
{
    isplaying = 0;
}

void AudioPlayer::pause()
{
    if (isplaying == 1)
        isplaying = 2;
    else
        isplaying = 1;
}

bool AudioPlayer::isPlaying() const
{
    return isplaying==1;
}
bool AudioPlayer::isPaused() const
{
    return isplaying==2;
}

int16_t AudioPlayer::getEqualizerSetting(const EqualizerSetting setting)
{
    int16_t val = 0;
    switch (setting)
    {
    case _100HZ:
        val = -10;
        break;
    case _1KHZ:
        val = -2;
        break;
    case _10KHZ:
        val = 5;
        break;
    case _20KHZ:
        val = 10;
        break;
    default:
        break;

    }

    return val;
}

void AudioPlayer::setEqualizerSetting(const EqualizerSetting setting, const int16_t value)
{
    static int i = 0;
    i++;
}
uint32_t AudioPlayer::getSongProgressMs()
{
    progress += 20;
    return progress;
}

uint32_t AudioPlayer::getSongDurationMs()
{
    return 3*60000;
}
void AudioPlayer::setVolume(int volume)
{

}
void AudioPlayer::setSongProgressPercentage(uint32_t prog)
{
    progress = prog;
}
#endif
