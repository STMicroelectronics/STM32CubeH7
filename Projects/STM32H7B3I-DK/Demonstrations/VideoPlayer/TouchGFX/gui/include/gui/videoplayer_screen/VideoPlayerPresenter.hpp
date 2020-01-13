#ifndef VIDEOPLAYERPRESENTER_HPP
#define VIDEOPLAYERPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class VideoPlayerView;

class VideoPlayerPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    VideoPlayerPresenter(VideoPlayerView& v);

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

    virtual ~VideoPlayerPresenter() {};

    void setVolume(int vol);

    void updateVideoDuration(int duration);
    void setVideoDuration(int length);
    int getElapsedTime();

    void setPlayVideo(bool play);
    bool getPlayVideo();

    void setSelected(int id);
    bool getSelected();

    void setNumberOfVideos(int songs);
    int getNumberOfVideos();

    bool hasVideoEnded();
    void updateVideoList(const char * videoFile);

    void nextPressed();
    void previousPressed();

    bool getRepeat();
    void setRepeat(bool set);

    void setShuffle(bool set);
    bool getShuffle();

    void setCurrentVideo(int);
    int  getCurrentVideo();

    virtual void refreshVideoList();
    void returnToMenuLauncher();

private:
    VideoPlayerPresenter();

    VideoPlayerView& view;
};


#endif // VIDEOPLAYERPRESENTER_HPP
