

#ifndef VIDEO_PLAYER_VIEW_HPP
#define VIDEO_PLAYER_VIEW_HPP

#include <touchgfx/Application.hpp>
#include <touchgfx/mixins/ClickListener.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/PixelDataWidget.hpp>
#include <touchgfx/widgets/TouchArea.hpp>

#include <gui/common/DemoView.hpp>
#include <gui/common/Playlist.hpp>
#include <gui/common/TopBar.hpp>
#include <gui/common/CommonDefinitions.hpp>
#include <gui/video_player_screen/VideoBottomBar.hpp>
#include <gui/video_player_screen/FolderWidget.hpp>
#include <gui/video_player_screen/PlaylistWidget.hpp>
#include <gui/video_player_screen/VideoPlayerPresenter.hpp>
#include <gui/video_player_screen/VideoProgressBar.hpp>
#include <gui/video_player_screen/mjpegreader.hpp>

using namespace touchgfx;

class VideoPlayerView : public DemoView<VideoPlayerPresenter>, PlaylistManager
{
public:
    VideoPlayerView();
    virtual ~VideoPlayerView() { }

    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
    //Playlist Manager functions
    virtual bool addFile(uint32_t index);
    virtual void removeFile(uint32_t index);
    virtual void folderSelected(uint32_t index);
    virtual void upPressed();
    virtual void donePressed();
    virtual void handleClickEvent(const ClickEvent& evt);

    void screenClick(ClickEvent::ClickEventType type, int x, int y);
private:
    Callback<VideoPlayerView, Action> actionCallback;
    Callback<VideoPlayerView, const uint32_t> playlistCallback;
    Callback<VideoPlayerView, const uint32_t> folderlistCallback;
    Callback<VideoPlayerView, const AbstractButton&> playlistBackCallback;
    Callback<VideoPlayerView, const PixelDataWidget&, const ClickEvent&> movieCallback;
    Callback<VideoPlayerView, const uint32_t> progressCallback;
    Callback<VideoPlayerView, const MoveAnimator<VideoBottomBar>&>  animationCallback;
    Callback<VideoPlayerView> movieEndedCallback;
    Callback<VideoPlayerView, const AbstractButton&> largeButtonPressedCallback;

    void onAction(Action action);
    void onPlaylistClicked(const uint32_t index);
    void onPlaylistBack(const AbstractButton&);
    void onMovieClicked(const PixelDataWidget&, const ClickEvent&);
    void onFolderlistFolderClicked(const uint32_t index);
    void onProgressBarDragged(const uint32_t percent);
    void onAnimationEnded(const MoveAnimator<VideoBottomBar>& src);
    void onMovieEnded();
    void loadRootFiles();
    void selectNextPlaylistMovie();

    void largeButtonPressedHandler(const AbstractButton& src);

    enum BAR_STATE
    {
        BAR_FADEIN,
        BAR_FADEOUT,
        BAR_SHOW_IMMEDIATE,
        BAR_HIDE,
        BAR_TOP_ONLY
    };

    void showBars(BAR_STATE show);

    enum
    {
        FPS_TITLE_LENGTH   = 4,
        LOAD_TITLE_LENGTH  = 4
    };

    enum State
    {
        INITIAL,
        WAIT_PLAYLIST,
        PAUSED,
        PLAYING
    };

    //Buffers in internal RAM
    //Buffer to decode one line of JPEG
    uint8_t lineBuffer[RGB24BYTES(800, 1)];
    //RAM for AVI file buffer, 100 Kb, please encode video at max 800 kbits/s
    uint8_t fileBuffer[100 * 1024];

    fileinput::DirEntry dirlist[DIRLIST_LENGTH];
    char currentFolder[50];
    FolderWidget folderWidget;

    Image noMovieBackground;
    ClickListener<PixelDataWidget> moviePixelWidget;
    MoveAnimator<TopBar> topBar;
    MoveAnimator<VideoBottomBar> bottomBar;
    MoveAnimator<VideoProgressBar> progressBar;

    Button playlistLargeButton;
    Button folderLargeButton;

    Image moviePlayButton;

    PlaylistElement playlist[PLAYLIST_LENGTH];
    uint32_t loopPlaylistIndex;
    PlaylistWidget playlistWidget;

    MJPEGReader mJPEGReaderMovie;
    MJPEGReader mJPEGReaderThumb;

    BitmapId movieBufferBitmapId;
    BitmapId temporaryBufferBitmapId;

    uint8_t load, fps;
    TextAreaWithOneWildcard fpsInfo;
    Unicode::UnicodeChar fpsInfoBuf[FPS_TITLE_LENGTH];

    State state;
    char* currentAviFileName;
};

#endif // VIDEO_PLAYER_VIEW_HPP
