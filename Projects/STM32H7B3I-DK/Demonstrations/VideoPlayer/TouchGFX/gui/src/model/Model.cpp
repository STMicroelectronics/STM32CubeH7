#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include <string.h>
#include <stdlib.h>

#include "avi_player.h"

static  media_player_handle_t  audio_player;

Model::Model() : modelListener(0), numberOfVideos(0), volume(AUDIO_VOLUME_LEVEL), playVideo(false), repeat(false), shuffle(false)
{
    media_player_new(&audio_player);
    audio_player.init(&audio_player);
}

void Model::tick()
{

}

void Model::setVolume(int volume)
{
    uint32_t vol;
    if (volume == 0)
      vol = 0;

    else if (volume  < 10)
      vol = 30;

    else if (volume < 30)
      vol = 40;

    else if (volume < 50)
      vol = 50;

    else
      vol = volume;

    audio_player.set_volume(&audio_player, vol);
}

int Model::getVolume()
{
    return this->volume;
}

void Model::setVideoLength(int length)
{
    videoLength = length;
}

int Model::getVideoLength()
{
    return videoLength;
}

void Model::setVideoDuration(int duration)
{
    videoDuration = duration;
}

int Model::getVideoDuration()
{
    return videoDuration;
}

void Model::setPlayVideo(bool play)
{
    playVideo = play;
    if (play)
    {
        audio_player.play(&audio_player);
    }
    else
    {
      audio_player.pause(&audio_player);
    }
}

bool Model::getPlayVideo()
{
    return playVideo;
}

void Model::setSelected(int id)
{
    selected = id;
    audio_player.set_avi_file(&audio_player, id);

    if (playVideo)
    {
        audio_player.play(&audio_player);
    }
    else
    {
       audio_player.pause(&audio_player);
    }
}

int Model::getSelected()
{
    return selected;
}

void Model::setRepeat(bool set)
{
    repeat = set;
}

bool Model::getRepeat()
{
    return repeat;
}

void Model::setShuffle(bool set)
{
    shuffle = set;
}

bool Model::getShuffle()
{
    return shuffle;
}

void Model::setNumberOfVideos(int amount)
{
    numberOfVideos = amount;
}

int Model::getNumberOfVideos()
{
    return numberOfVideos;
}

bool  Model::hasVideoEnded()
{
  return (audio_player.get_state(&audio_player) == PLAYER_EOF_REACHED);
}

int Model::getElapsedTime()
{
  return audio_player.get_elapsed_time(&audio_player);
}

void Model::updateVideoList(const char *videoFile)
{
    audio_player.update_play_list(&audio_player, videoFile);
}

void Model::refreshVideoList()
{
    modelListener->refreshVideoList();
}

void Model::returnToMenuLauncher()
{
   HAL_NVIC_SystemReset();
}
