#include <gui/videoplayer_screen/VideoPlayerView.hpp>
#include <gui/videoplayer_screen/VideoPlayerPresenter.hpp>

#include <stdlib.h>

VideoPlayerPresenter::VideoPlayerPresenter(VideoPlayerView& v)
    : view(v)
{
}

void VideoPlayerPresenter::activate()
{
   view.updateVolume(model->getVolume());
}

void VideoPlayerPresenter::deactivate()
{

}

void VideoPlayerPresenter::setVolume(int vol)
{
    model->setVolume(vol);
}

void VideoPlayerPresenter::setVideoDuration(int length)
{
    model->setVideoLength(length);

    int minutes, seconds;
    minutes = length / 60;
    seconds = minutes ? length % 60 : length;
    view.setVideoDuration(length, minutes, seconds);
}

void VideoPlayerPresenter::updateVideoDuration(int duration)
{
    model->setVideoDuration(duration);
    int minutes, seconds;
    minutes = duration / 60;
    seconds = minutes ? duration % 60 : duration;
    view.updateVideoDuration(duration, minutes, seconds);
}

int VideoPlayerPresenter::getElapsedTime()
{
  return model->getElapsedTime();
}

void VideoPlayerPresenter::setPlayVideo(bool play)
{
    model->setPlayVideo(play);
}

bool VideoPlayerPresenter::getPlayVideo()
{
    return model->getPlayVideo();
}


void VideoPlayerPresenter::setSelected(int id)
{
    model->setSelected(id);
}

bool VideoPlayerPresenter::getSelected()
{
    return model->getSelected();
}

void VideoPlayerPresenter::setNumberOfVideos(int videos)
{
    model->setNumberOfVideos(videos);
}

int VideoPlayerPresenter::getNumberOfVideos()
{
    return model->getNumberOfVideos();
}

bool VideoPlayerPresenter::hasVideoEnded()
{
  return model->hasVideoEnded();
}


void VideoPlayerPresenter::updateVideoList(const char * videoFile)
{
    model->updateVideoList(videoFile);
}

void VideoPlayerPresenter::nextPressed()
{
    if (model->getShuffle())
    {
        view.setVideo(rand() % getNumberOfVideos());
    }
    else if (getCurrentVideo() == (getNumberOfVideos() - 1))
    {
      view.setVideo(0);
    }
    else
    {
        view.setVideo(getCurrentVideo() + 1);
    }
}

void VideoPlayerPresenter::previousPressed()
{
    if (model->getShuffle())
    {
        view.setVideo(rand() % getNumberOfVideos());
    }
    else if (getCurrentVideo() != 0)
    {
        view.setVideo(getCurrentVideo() - 1);
    }
    else if (getCurrentVideo() == 0 && model->getRepeat())
    {
        view.setVideo(getNumberOfVideos() - 1);
    }
}

void VideoPlayerPresenter::setCurrentVideo(int video)
{
    model->setSelected(video);
}

int VideoPlayerPresenter::getCurrentVideo()
{
    return model->getSelected();
}

bool VideoPlayerPresenter::getRepeat()
{
    return model->getRepeat();
}
void VideoPlayerPresenter::setRepeat(bool set)
{
    model->setRepeat(set);
}

bool VideoPlayerPresenter::getShuffle()
{
    return model->getShuffle();
}

void VideoPlayerPresenter::setShuffle(bool set)
{
    model->setShuffle(set);
}

void VideoPlayerPresenter::refreshVideoList()
{
    view.setupContent();
}

void VideoPlayerPresenter::returnToMenuLauncher()
{
    model->returnToMenuLauncher();
}
