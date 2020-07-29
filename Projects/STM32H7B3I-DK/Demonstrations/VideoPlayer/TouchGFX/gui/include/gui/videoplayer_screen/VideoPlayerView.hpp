#ifndef VIDEOPLAYERVIEW_HPP
#define VIDEOPLAYERVIEW_HPP

#include <gui_generated/videoplayer_screen/VideoPlayerViewBase.hpp>
#include <gui/videoplayer_screen/VideoPlayerPresenter.hpp>
#include <gui/containers/track.hpp>

#include <widgets/MJPEGReader.hpp>

#ifdef _MSC_VER
#define strcat strcat_s
#define strncpy strncpy_s
#endif

class VideoPlayerView : public VideoPlayerViewBase
{
public:
    VideoPlayerView();
    virtual ~VideoPlayerView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void volumeButtonPressed(bool value);
    virtual void volumeSliderChanged(uint16_t value);
    virtual void videoSliderChanged(uint16_t value);
    virtual void playPauseButtonPressed(bool value);
    virtual void videoListPlayPausePressed(bool value);
    virtual void nextPressed();
    virtual void previousPressed();
    virtual void shufflePressed(bool value);
    virtual void repeatPressed(bool value);
    virtual void newVideoSelected(int value);
    virtual void videoButtonPressed();
    virtual void cancelButtonPressed();
    virtual void backButtonPressed();

    void updateVolume(int volume);

    void updateVideoDuration(int totalDuration, int minutes, int seconds);
    void setVideoDuration(int setTotalLength, int minutes, int seconds);

    virtual void handleTickEvent();

    void setVideo(int id);
    bool addFile(uint32_t index);
    void loadRootFiles();
    void setupThumbnails();
    void setupContent();

protected:

    struct PlaylistElement
    {
        char filename[60];
        touchgfx::BitmapId bmpId;
        bool used;
    };

    MJPEGReader mJPEGReaderThumb;

    static const int VOLUME_SLIDER_X = 424;
    static const int VOLUME_SLIDER_MOVE_DURATION = 30;
    static const int ANIMATION_DURATION = 40;
    static const int ANIMATION_DELAY = 150;
    static const int PLAYLIST_LENGTH = 10;
    static const int DIRLIST_LENGTH = PLAYLIST_LENGTH;

    void movieEndedHandler();
    Callback<VideoPlayerView> movieEndedCallback;

    MJPEGReader mJPEGReaderMovie;
    fileinput::FileHdl currentMovie;
    BitmapId videoBitmap;
    BitmapId temporaryBufferBitmap;

    //Buffer to decode one line of JPEG
    uint8_t lineBuffer[480 * 1 * 3 /*Width * Height * Bitdepth*/];
    //RAM for AVI file buffer, 100 Kb, please encode video at max 800 kbits/s
    uint8_t fileBuffer[100 * 1024];

    fileinput::DirEntry dirlist[DIRLIST_LENGTH];
    PlaylistElement playlist[PLAYLIST_LENGTH];
    char currentFolder[50];
    char* currentAviFileName;

    bool sliderIsMoved;

    int msPerFrame;
    int numberOfFrames;
    int currentSongSeconds;

    int totalDuration;

    int animationDelay;
    int startAnimationVal;
    bool animate;
};

#endif // VIDEOPLAYERVIEW_HPP
