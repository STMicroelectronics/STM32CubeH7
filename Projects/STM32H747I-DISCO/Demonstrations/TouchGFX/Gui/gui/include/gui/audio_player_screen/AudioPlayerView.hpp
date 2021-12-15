

#ifndef AUDIO_PLAYER_VIEW_HPP
#define AUDIO_PLAYER_VIEW_HPP

#include <gui/common/DemoView.hpp>
#include <gui/common/TopBar.hpp>
#include <gui/common/CommonDefinitions.hpp>
#include <gui/audio_player_screen/Equalizer.hpp>
#include <gui/audio_player_screen/AudioBottomBar.hpp>
#include <gui/audio_player_screen/AudioPlayerPresenter.hpp>
#include <gui/audio_player_screen/CoverWidget.hpp>
#include <gui/audio_player_screen/AudioPlayerPlaylist.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/PixelDataWidget.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <gui/audio_player_screen/AudioFolderWidget.hpp>
#include <gui/common/Playlist.hpp>
#include <touchgfx/mixins/ClickListener.hpp>
#include "touchgfx/mixins/MoveAnimator.hpp"

using namespace touchgfx;

class PixelDataWidgetWithAlpha: public PixelDataWidget
{
public:
    inline uint8_t getAlpha() const
    {
        return alpha;
    }
};

class AudioPlayerView : public DemoView<AudioPlayerPresenter>, AudioPlaylistManager
{
public:
    enum BAR_STATE
    {
        BAR_FADEIN,
        BAR_TOP_HIDE,
        BAR_FADEOUT,
        BAR_SHOW_IMMEDIATE,
        BAR_FADE_BOTTOM_IN,
        BAR_FADE_BOTTOM_OUT,
        BAR_BOTTOM_HIDE,
        BAR_HIDE,
        BAR_TOP_ONLY
    };

    AudioPlayerView(); //fast forward and rewind?
    virtual ~AudioPlayerView() { }

    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
    void playButtonClicked(const AbstractButton& btn);
    void playButtonTouched(const Button&, const ClickEvent&);
    void prevNextButtonClicked(const AbstractButton& btn);
    void handleVolumeChange(int newVal);
    void setVolume(int volume)
    {
        bottomBar.setVolume(volume);
    }
    void setSongInfo(int playlistIndex, char* name, uint32_t duration);
    void setCover(BitmapId cover);
    void addToPlaylist(BitmapId cover, char* artist, char* song);
    void setEqualizerSettings(int16_t _100Hz, int16_t _1kHz, int16_t _10kHz, int16_t _20kHz, int16_t loudness);
    void showNoMedia();
    void showInitializing();
    void showMainScreen();
    void showBars(BAR_STATE show);

    virtual bool addFile(uint32_t index);
    virtual void removeFile(uint32_t index);
    virtual void folderSelected(uint32_t index);
    virtual void upPressed();
    virtual void donePressed();

    void setupDirlist(const char* const foldername, fileinput::DirEntry* dirlist, uint32_t length);
    void clearPlaylist();

    void screenClick(ClickEvent::ClickEventType type, int x, int y);
    void screenDrag(int16_t x_old, int16_t y_old, int16_t x_new, int16_t y_new);

private:

    static const int CANVAS_BUFFER_SIZE = 1024 * 16;
    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];

    Callback<AudioPlayerView, const AbstractButton&> playCallback;
    Callback<AudioPlayerView, const Button&, const ClickEvent&> playTouchedCallback;

    Callback<AudioPlayerView, const AbstractButton&> backCallback;
    Callback<AudioPlayerView, const AbstractButton&> prevNextCallback;
    Callback<AudioPlayerView, int> volumeChangedCallback;
    Callback<AudioPlayerView, int> playlistElementSelectedCallback;
    Callback<AudioPlayer, const AudioPlayer::EqualizerSetting, const int16_t> equalizerSettingCallback;
    Callback<AudioPlayerView, Action> actionCallback;
    Callback<AudioPlayerView, uint32_t> progressSliderCallback;

    void onAction(Action action);
    void backButtonClicked(const AbstractButton& source);
    void playlistElementSelected(int index);
    void progressSliderChanged(uint32_t newValue);

    void applyCoverBlur(Bitmap& bmp);
    void horizontalBlur(uint8_t* src, uint8_t* work, int width, int height, int radius);
    void verticalBlur(uint8_t* src, uint8_t* work, int width, int height, int radius);
    void boxBlur(uint8_t* src, uint8_t* work, int width, int height, int radius);
    void gaussBlur(uint8_t* src, int width, int height, int radius);
    void upscaleBlurredImage(uint8_t* src, int width, int height);
    void upscaleBlurredImage2(uint8_t* src, int width, int height);
    void showBackButton(bool show);
    static const int BLUR_REGION_WIDTH = 200;
    static const int BLUR_REGION_HEIGHT = 120;
    uint8_t blurBuffer[BLUR_REGION_WIDTH * BLUR_REGION_HEIGHT * 3];
    AudioPlayerPlaylist playlist;
    Equalizer equalizer;
    PixelDataWidgetWithAlpha blurBackground;
    MoveAnimator<TopBar> topBar;
    MoveAnimator<AudioBottomBar> bottomBar;
    CoverWidget coverImage;
    ClickListener<Button> playButton;
    Button prevButton;
    Button nextButton;
    Button backButton;
    PlayerState playerState;
    uint32_t duration;

    fileinput::DirEntry dirlist[DIRLIST_LENGTH];
    char currentFolder[50];
    AudioFolderWidget folderWidget;

    uint32_t ticks;
};

#endif // AUDIO_PLAYER_VIEW_HPP
