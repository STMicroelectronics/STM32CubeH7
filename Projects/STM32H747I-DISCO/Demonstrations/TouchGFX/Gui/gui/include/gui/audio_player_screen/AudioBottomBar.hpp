

#ifndef AUDIO_BOTTOM_BAR_HPP
#define AUDIO_BOTTOM_BAR_HPP

#include <touchgfx/containers/Slider.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <gui/common/BottomBar.hpp>

class AudioBottomBar : public BottomBar
{
public:
    AudioBottomBar();
    virtual ~AudioBottomBar() {}
    void setCurrentTime(int minutes, int seconds);
    void setTotalTime(int minutes, int seconds);
    void setPlayState(PlayerState state);
    void setVolume(int volume);
    void handleVolumeChanged(const Slider& slider, int val);
    void setVolumeAction(GenericCallback<int>& action);
private:
    virtual void buttonClicked(const AbstractButton& btn);

    Callback<AudioBottomBar, const Slider&, int> newVolumeCallback;
    GenericCallback<int>* volumeAction;
    TextAreaWithOneWildcard currentTimeText;
    TextAreaWithOneWildcard totalTimeText;
    TextArea timeSeparator;
    Slider volumeSlider;
    Image volumeIcon;
    Unicode::UnicodeChar currentTimeBuf[6];
    Unicode::UnicodeChar totalTimeBuf[6];
    PlayerState playerState;
    Button equalizerButton;
    int currMinutes;
    int currSeconds;
};

#endif /* AUDIO_BOTTOM_BAR_HPP */
