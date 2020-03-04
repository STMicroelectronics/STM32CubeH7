#ifndef VIDEO_PLAYER_PRESENTER_HPP
#define VIDEO_PLAYER_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class VideoPlayerView;

class VideoPlayerPresenter : public DemoPresenter
{
public:
    VideoPlayerPresenter(VideoPlayerView& v);

    virtual void activate();
    virtual void deactivate();

    virtual void notifyMediaDisconnected();
    virtual void notifyMediaConnected();

    virtual ~VideoPlayerPresenter() {};

    virtual void oneSecond();
    virtual void screenSaverMinorTick();

private:
    VideoPlayerPresenter();

    VideoPlayerView& view;

    int16_t counter;
};

#endif // VIDEO_PLAYER_PRESENTER_HPP
