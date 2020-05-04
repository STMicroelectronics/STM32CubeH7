#include <gui/audio_player_screen/audio_playerView.hpp>
#include <gui/audio_player_screen/audio_playerPresenter.hpp>

#include <stdio.h>
#include <stdlib.h>

audio_playerPresenter::audio_playerPresenter(audio_playerView& v)
    : view(v)
{
}

void audio_playerPresenter::activate()
{   
#ifdef SIMULATOR
    for (int i = 0; i < model->getNumberOfSongs(); i++)
    {
        touchgfx::Unicode::UnicodeChar temp[30];
        touchgfx::Unicode::snprintf(temp, 30, "tracknumber %d", i);
        model->setTrackName(temp, i);

        int templength;
        templength = 20 + i + 21 * i + 3;
        model->setTrackLength(templength, i);
    }
#else
    model->audioPlayerInit();
    model->setTrackList();
#endif
    view.updateMusicVolume(model->getMusicVolume());
    setMusicDuration(model->getSongLength());
    updateMusicDuration(model->getSongDuration());
    setNumberOfSongs(model->getNumberOfSongs());

    if (model->getNumberOfSongs() > 0)
    {
        setSelected(true, 0);
        model->noMediaFileDetected(true);
    }
    else
    {
        model->noMediaFileDetected(false);
    }

    view.refreshTrackList();
}

void audio_playerPresenter::deactivate()
{

}

void audio_playerPresenter::setMusicVolume(int vol)
{
    model->setMusicVolume(vol);
}

void audio_playerPresenter::setMusicDuration(int length)
{
    int minutes, seconds;
    minutes = length / 60;
    seconds = minutes ? length % 60 : length;
    view.setMusicDuration(length, minutes, seconds);
}

void audio_playerPresenter::updateMusicDuration(int duration)
{
    int minutes, seconds;
    minutes = duration / 60;
    seconds = minutes ? duration % 60 : duration;
    view.updateMusicDuration(duration, minutes, seconds);
}

void audio_playerPresenter::setSongDuration(int duration)
{
    model->setSongDuration(duration);
}

void audio_playerPresenter::setPlaySong(bool play)
{
    model->setPlaySong(play);
}

bool audio_playerPresenter::getPlaySong()
{
    return model->getPlaySong();
}

void audio_playerPresenter::setTrackLength(int id, int length)
{
    model->setTrackLength(id, length);
}

int audio_playerPresenter::getTrackLengthMinutes(int id)
{
    return model->getTrackLength(id) / 60;
}


int audio_playerPresenter::getTrackLengthSeconds(int id)
{
    return model->getTrackLength(id) / 60 ? model->getTrackLength(id) % 60 : model->getTrackLength(id);
}

void audio_playerPresenter::setTrackName(touchgfx::Unicode::UnicodeChar* name, int id)
{
    model->setTrackName(name, id);
}

void audio_playerPresenter::getTrackName(touchgfx::Unicode::UnicodeChar* name, int id)
{
    model->getTrackName(name, id);
}

void audio_playerPresenter::setSelected(bool selected, int id)
{
    model->setSelected(selected, id);
}

bool audio_playerPresenter::getSelected(int id)
{
    return model->getSelected(id);
}

void audio_playerPresenter::setNumberOfSongs(int songs)
{
    view.setTrackListSize(songs);
}

int audio_playerPresenter::getNumberOfSongs()
{
    return model->getNumberOfSongs();
}

void audio_playerPresenter::nextSongPressed()
{
    if (model->getShuffle())
    {
        setSelected(false, getCurrentSong());
        setSelected(true, rand() % getNumberOfSongs());
    }
    else if (getCurrentSong() == getNumberOfSongs() - 1)
    {        
        setSelected(false, getCurrentSong());
        setSelected(true, 0);

        //model->setPlaySong(model->getRepeat());
    }
    else
    {
        setSelected(false, getCurrentSong());
        setSelected(true, getCurrentSong() + 1);
    }
    view.refreshTrackList();
}

void audio_playerPresenter::previousSongPressed()
{
//    if (model->getSongDuration() > 3 || (getCurrentSong() == 0 && !model->getRepeat()))
//    {
//        setSelected(true, getCurrentSong());
//    }
//    else
    {
        if (model->getShuffle())
        {
            setSelected(false, getCurrentSong());
            setSelected(true, rand() % model->getNumberOfSongs());
        }
        else if (getCurrentSong() != 0)
        {
            setSelected(false, getCurrentSong());
            setSelected(true, getCurrentSong() - 1);
            //view.refreshTrackList();
        }
        else if (getCurrentSong() == 0 /*&& model->getRepeat()*/)
        {
            setSelected(false, getCurrentSong());
            setSelected(true, getNumberOfSongs() - 1);
            //view.refreshTrackList();
        }
        
        view.refreshTrackList();
    }
}

void audio_playerPresenter::setCurrentSong(int song)
{
    model->setCurrentSong(song);
}

int audio_playerPresenter::getCurrentSong()
{
    return model->getCurrentSong();
}

void audio_playerPresenter::setRepeat(bool set)
{
    model->setRepeat(set);
}

void audio_playerPresenter::setShuffle(bool set)
{
    model->setShuffle(set);
}

void audio_playerPresenter::psCountUpdate(int cnt)
{
    view.psCountUpdate(cnt);
}

void audio_playerPresenter::setPlayingGraphics(bool playing)
{
    view.setPlayingGraphics(playing);
}
void audio_playerPresenter::cancelPowerSave()
{
    model->cancelPowerSave();
}

void audio_playerPresenter::powerSavePressed()
{
    model->powerSavePressed();
}

void audio_playerPresenter::returnToMenuPressed()
{
    model->returnToMenuLanucher();
}

void audio_playerPresenter::noMediaFilesDetected(bool filesDetected)
{
    view.noMediaFilesDetected(filesDetected);
}
