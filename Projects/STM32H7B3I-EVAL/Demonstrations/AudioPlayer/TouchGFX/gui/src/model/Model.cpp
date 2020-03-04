#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "storage.h"
#include "audio_player_app.h"
#include "audio_lowpower.h"
extern osMessageQId   LPModeEvent;
extern __IO uint16_t GUITaskResumed ;

Model::Model() : modelListener(0), musicVolume(70), songLength(524), songDuration(0), playSong(false), psCountDown(false), numberOfSongs(10),
                 repeat(false), shuffle(false), tickCnt(0), psTicks(0), psCountDownCnt(5),elapsedTime(0)
{
    memset(tracks,0x0,sizeof(trackList));
}

void Model::tick()
{
    if (playSong)
    {
        if (tickCnt == TICK_COUNT)
        {
            if (songDuration == songLength)
            {
                if (shuffle)
                {
                    setSelected(false, getCurrentSong());
                    setSelected(true, rand() % numberOfSongs);
                }
                else
                {
                    setSelected(false, getCurrentSong());

                    if (getCurrentSong() == numberOfSongs - 1)
                    {                        
                        modelListener->setPlayingGraphics(repeat);
                        playSong = repeat;
                        setSelected(true, 0);
                    }
                    else
                    {
                        setSelected(true, getCurrentSong() + 1);
                    }
                    
                }
            }
            else
            {

              if(GUITaskResumed == 0x1)
              {
                elapsedTime = AUDIOPLAYER_GetProgress();
                if(getCurrentSong() != FileList.FileIndex)
                {
                  tracks[getCurrentSong()].selected = false;
                  
                  setCurrentSong(FileList.FileIndex);
                  tracks[getCurrentSong()].selected = true;
                  songLength = tracks[FileList.FileIndex].trackLength;         
                  modelListener->setMusicDuration(songLength);               
                }
    
                songDuration = elapsedTime;
                modelListener->updateMusicDuration(elapsedTime);
                GUITaskResumed = 0x0;
              }
              else
              {
                 songDuration++;
                 modelListener->updateMusicDuration(songDuration);
              }

            }
            tickCnt = 0;
        }
        else
        {
            tickCnt++;
        }
    }
    if (psCountDown)
    {
        if (psTicks == TICK_COUNT)
        {
            psCountDownCnt--;
            modelListener->psCountUpdate(psCountDownCnt);
            if (psCountDownCnt == 0)
            {
                cancelPowerSave();
                osMessagePut ( LPModeEvent, AUDIOLP_ENTER_LPMODE, 0);
            }
            psTicks = 0;
        }
        else
        {
            psTicks++;
        }
    }
}

void Model::cancelPowerSave()
{
    psCountDown = false;
}

void Model::powerSavePressed()
{
    psCountDown = true;
    psTicks = 0;
    psCountDownCnt = 5;
}

void Model::setMusicVolume(int volume)
{
  if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
  {

    AUDIOPLAYER_SetVolume(volume);
  }
   musicVolume = volume;
}

int Model::getMusicVolume()
{
    return musicVolume;
}

void Model::setSongLength(int length)
{
    songLength = length;
}

int Model::getSongLength()
{
    return songLength;
}

void Model::setSongDuration(int duration)
{
  songDuration = duration;
  AUDIOPLAYER_SeekToTime(duration);
}

int Model::getSongDuration()
{
    return songDuration;
}

void Model::setPlaySong(bool play)
{
  
  if(play)
  {
    if(AUDIOPLAYER_GetState() == AUDIOPLAYER_STOP)
    {
      FileList.FileIndex = getCurrentSong();
      AUDIOPLAYER_Play(FileList.file[FileList.FileIndex].full_name,getMusicVolume());
      setSongDuration(songDuration);
    }
    else if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PAUSE)
    {
      AUDIOPLAYER_Resume();
      if(elapsedTime != songDuration)
      {
        setSongDuration(songDuration);
        elapsedTime = songDuration;
      }
    }  
  }
  else
  {
    if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
    {
      AUDIOPLAYER_Pause();
      elapsedTime = songDuration;
    }
  }
  
  playSong = play;
}

bool Model::getPlaySong()
{
    return playSong;
}

void Model::setTrackLength(int length, int id)
{
    tracks[id].trackLength = length;
}

int Model::getTrackLength(int id)
{
    return tracks[id].trackLength;
}

void Model::setTrackName(touchgfx::Unicode::UnicodeChar* name, int id)
{
	touchgfx::Unicode::strncpy(tracks[id].songName, name, MAX_SONG_NAME_LENGTH);
}

void Model::getTrackName(touchgfx::Unicode::UnicodeChar* name, int id)
{
	touchgfx::Unicode::strncpy(name, tracks[id].songName, MAX_SONG_NAME_LENGTH);
}

void Model::setSelected(bool selected, int id)
{    
    tracks[id].selected = selected;
    if (selected)
    {   
        setCurrentSong(id);
        songLength = tracks[id].trackLength;
        songDuration = 0;
        tickCnt = 0;

        modelListener->updateMusicDuration(songDuration);
        modelListener->setMusicDuration(songLength);        
        
      if(AUDIOPLAYER_GetState() != AUDIOPLAYER_STOP)
      {
        AUDIOPLAYER_Stop();
      }
      if(getPlaySong())
      {
        FileList.FileIndex = id;
        AUDIOPLAYER_Play(FileList.file[FileList.FileIndex].full_name,getMusicVolume());
      } 
    }
}

bool Model::getSelected(int id)
{
    return tracks[id].selected;
}

void Model::setCurrentSong(int song)
{
    currentSong = song;
}

int Model::getCurrentSong()
{
    return currentSong;
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

void Model::returnToMenuLanucher()
{
  HAL_NVIC_SystemReset();
}

void Model::setNumberOfSongs(int number)
{
  numberOfSongs = number;
}

void Model::noMediaFileDetected(bool filesDetected)
{
    modelListener->noMediaFilesDetected(filesDetected);
}

void Model::audioPlayerInit()
{
  AUDIOPLAYER_Init(musicVolume);
}

void Model::setTrackList()
{
  for (int i = 0; i < FileList.ptr; i++)
  {
    touchgfx::Unicode::UnicodeChar temp[30];
    touchgfx::Unicode::fromUTF8(FileList.file[i].name, temp, 30); 
    setTrackName(temp, i);
    setTrackLength(FileList.file[i].length, i); 
  } 
  setNumberOfSongs(FileList.ptr);
}
