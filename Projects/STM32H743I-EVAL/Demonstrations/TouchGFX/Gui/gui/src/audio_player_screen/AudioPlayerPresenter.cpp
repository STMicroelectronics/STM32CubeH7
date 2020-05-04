#include <gui/audio_player_screen/AudioPlayerPresenter.hpp>
#include <gui/audio_player_screen/AudioPlayerView.hpp>
#include <gui/audio_player_screen/BMPFileLoader.hpp>
#include <string.h>
#include <stdio.h>
#include <gui/common/Defines.hpp>

AudioPlayerPresenter::AudioPlayerPresenter(AudioPlayerView& v) :
    DemoPresenter(v),
    view(v),
    counter(0),
    tickCnt(0)
{
}

void AudioPlayerPresenter::activate()
{
    // At startup show "Initializing audio.."
    view.showInitializing();
    model->setCurrentScreen(Defines::AUDIO_PLAYER_SCREEN);
}

void AudioPlayerPresenter::initializeFileSystem()
{
    for (int i = 0; i < PLAYLIST_LENGTH; i++)
    {
        playlist[i].used = false;
        covers[i].used = false;
    }

    if(fileinput::Init() != 0)
    {
        uint32_t n = 0;
        strncpy(currentFolder, fileinput::getDrive(), sizeof(currentFolder));
        fileinput::getDirectoryList(currentFolder, ".WAV", dirlist, DIRLIST_LENGTH);

        n = MIN(fileinput::getFileListR(currentFolder, ".BMP", filelist, DIRLIST_LENGTH), PLAYLIST_LENGTH);

        for (uint32_t i = 0; i < n; i++)
        {
            strncpy(covers[i].filename, filelist[i].name, 50);
            covers[i].bmpId = Bitmap::dynamicBitmapCreate(256, 256, Bitmap::RGB888);
            assert(covers[i].bmpId != BITMAP_INVALID && "could not allocate memory for album cover bitmap");
            readCover(covers[i]);
            covers[i].used = true;
        }

        n = MIN(fileinput::getFileListR(currentFolder, ".WAV", filelist, DIRLIST_LENGTH), PLAYLIST_LENGTH);
        for (uint32_t i = 0; i < n; i++)
        {
            addFile(i);
        }
        if (playlist[0].used)
        {
            selectSong(0);

            view.setVolume(model->getVolume());
            view.setEqualizerSettings(player.getEqualizerSetting(AudioPlayer::_100HZ), player.getEqualizerSetting(AudioPlayer::_1KHZ), player.getEqualizerSetting(AudioPlayer::_10KHZ), player.getEqualizerSetting(AudioPlayer::_20KHZ), player.getEqualizerSetting(AudioPlayer::LOUDNESS));
            player.init(model->getVolume());
        }
    }

    if (!playlist[0].used)
    {
        view.showNoMedia();
    }
    else
    {
        view.showBars(AudioPlayerView::BAR_BOTTOM_HIDE);
        view.showBars(AudioPlayerView::BAR_FADE_BOTTOM_IN);
        view.showMainScreen();
    }
}

void AudioPlayerPresenter::selectSong(int index)
{
    char* fullname = playlist[index].filename;

    fullname++;

    char copy[50];
    char unknown[10];

    strncpy(copy, fullname, 50);

    char* extension = strrchr(copy, '.');
    *extension = 0; //remove extension

    char* filename = strrchr(copy, fileinput::DirectorySeparator[0]);
    *filename = 0; //split filename from artist
    filename++;
    char* artist = strrchr(copy, fileinput::DirectorySeparator[0]);
    if (artist)
    {
        *artist = 0;
        artist++;
    }
    else
    {
        strncpy(unknown, "Unknown", 10);
        artist = unknown;
    }

#ifdef _MSC_VER
    _snprintf_s(currentSong, 50, "%s - %s", artist, filename);
#else
    snprintf(currentSong, 50, "%s - %s", artist, filename);
#endif

    currentPlayListItem = index;
    player.selectFile(playlist[currentPlayListItem].filename);
    view.setSongInfo(currentPlayListItem, currentSong, player.getSongDurationMs());
    view.setCover(playlist[currentPlayListItem].bmpId);
}

bool AudioPlayerPresenter::removeFile(uint32_t index)
{
    char fullname[FOLDERNAME_LENGTH + FILENAME_LENGTH];
    strncpy(fullname, currentFolder, FOLDERNAME_LENGTH);
    strcat(fullname, fileinput::DirectorySeparator);
    strcat(fullname, dirlist[index].name);

    //find index
    uint32_t activeSongs = 0;
    uint32_t pl_index = PLAYLIST_LENGTH;
    for (int i = 0; i < PLAYLIST_LENGTH; i++)
    {
        if (playlist[i].used)
        {
            if (strcmp(playlist[i].filename, fullname) == 0)
            {
                pl_index = i;
            }

            activeSongs++;
        }
    }

    //Update order of playlist by overwriting that index
    for (int i = pl_index; i < PLAYLIST_LENGTH - 1; i++)
    {
        playlist[i] = playlist[i + 1];
    }
    
    //If the deleted item is below current song playing, decrease index by one
    if (pl_index < currentPlayListItem)
    {
        currentPlayListItem--;
    }

    //clear last
    playlist[PLAYLIST_LENGTH - 1].used = false;

    //If this method was called, we certainly do have an sd card installed.
    //If we have no entries in the playlist when activate is called, no media is shown (This works because we do not store the playlist and repopulate it every time)
    bool empty = true;
    for (int i = 0; i < PLAYLIST_LENGTH - 1; i++)
    {
        if (playlist[i].used == true)
        {
            empty  = false;
        }
    }

    //If play list empty
    if (empty)
    {
        char title[15] = "PLAYLIST EMPTY";
        view.setSongInfo(0, title, 0);
        view.setCover(BITMAP_COVER_UNKNOWN_ARTIST_ID);
    }    
    //If we're removing the active song and that song is not the last song, choose next
    //If we're removing the active song and the song is the last song, choose previous
    else
    {
        if (currentPlayListItem == pl_index)
        {
            if (currentPlayListItem == activeSongs - 1)
            {
                selectSong(0);
            }
            else
            {
                //currentPlayListItem is essentially "next song"
                selectSong(currentPlayListItem);
            }
            player.stop();
        }
    }

    refreshPlaylist();

    activeSongs = 0;
    return true;
}

bool AudioPlayerPresenter::addFile(uint32_t index)
{
    uint32_t pl_index = PLAYLIST_LENGTH;
    for (int i = 0; i < PLAYLIST_LENGTH; i++)
    {
        if (playlist[i].used == false)
        {
            pl_index = i;
            break;
        }
    }
    if (pl_index == PLAYLIST_LENGTH)
    {
        return false; //no space
    }

    //set to first free place in play list
    playlist[pl_index].used = true;
    playlist[pl_index].filename[0] = 0; //cleared
    playlist[pl_index].bmpId = BITMAP_COVER_UNKNOWN_ARTIST_ID;
    strncpy(playlist[pl_index].filename, filelist[index].name, 50);

    char coverFolder[50];
    char songFolder[50];

    getFolder(playlist[pl_index].filename, songFolder, 50);
    for (int i = 0; i < PLAYLIST_LENGTH; i++)
    {
        if (covers[i].used)
        {
            getFolder(covers[i].filename, coverFolder, 50);
            if (strcmp(coverFolder, songFolder) == 0)
            {
                playlist[pl_index].bmpId = covers[i].bmpId;
                break;
            }
        }
    }

    char* fullname = playlist[index].filename;

    fullname++;

    char copy[50];
    char unknown[10];

    strncpy(copy, fullname, 50);

    char* extension = strrchr(copy, '.');
    *extension = 0; //remove extension

    char* filename = strrchr(copy, fileinput::DirectorySeparator[0]);
    *filename = 0; //split filename from artist
    filename++;
    char* artist = strrchr(copy, fileinput::DirectorySeparator[0]);
    if (artist)
    {
        *artist = 0;
        artist++;
    }
    else
    {
        strncpy(unknown, "Unknown", 10);
        artist = unknown;
    }

    view.addToPlaylist(playlist[pl_index].bmpId, artist, filename);

    return true;
}

void AudioPlayerPresenter::getFolder(char* file, char* buffer, uint32_t max)
{
    uint32_t i = 0;
    char* end = strrchr(file, fileinput::DirectorySeparator[0]);
    while (file != end && i < max)
    {
        *buffer = *file;
        buffer++;
        file++;
        i++;
    }
    *buffer = 0;
}

void AudioPlayerPresenter::readCover(CoverElement& cover)
{
    fileinput::FileHdl file = fileinput::openFile(cover.filename);
    if(file)
    {
      BMPFileLoader::readBMP24File(Bitmap(cover.bmpId), (BMPFileLoader::FileHdl)file);
      fileinput::closeFile(file);
    }
}

void AudioPlayerPresenter::deactivate()
{
    if(!isPlaylistEmpty())
    {
        player.deInit();
    }

    fileinput::deInit();
    for (uint32_t i = 0; i < PLAYLIST_LENGTH; i++)
    {
        if (covers[i].used)
        {
            Bitmap::dynamicBitmapDelete(covers[i].bmpId);
        }
    }
}

void AudioPlayerPresenter::setVolume(int val)
{
    if (val > 100)
    {
        val = 100;
    }
    else if (val < 0)
    {
        val = 0;
    }

    if (model->getVolume() != val)
    {
        model->setVolume(val);
        player.setVolume(val);
    }
}

void AudioPlayerPresenter::selectPreviousSong()
{
    if (!currentPlayListItem)
    {
        for (int i = PLAYLIST_LENGTH - 1; i >= 0; i--)
        {
            if (playlist[i].used)
            {
                currentPlayListItem = i;
                break;
            }
        }
    }
    else
    {
        currentPlayListItem--;
    }
    selectSong(currentPlayListItem);
}
void AudioPlayerPresenter::selectNextSong()
{
    if (currentPlayListItem < PLAYLIST_LENGTH)
    {
        currentPlayListItem++;
        if (!playlist[currentPlayListItem].used)
        {
            currentPlayListItem = 0;
        }
    }
    else
    {
        currentPlayListItem = 0;
    }
    selectSong(currentPlayListItem);
}

void AudioPlayerPresenter::playPauseClicked()
{
    if (player.isPlaying() || player.isPaused())
    {
        player.pause();
    }
    else
    {
        player.play();
    }
}

uint32_t AudioPlayerPresenter::getSongProgressMs()
{
    return player.getSongProgressMs();
}

void AudioPlayerPresenter::setSongProgressPercentage(uint32_t newVal)
{
    player.setSongProgressPercentage(newVal);
}

void AudioPlayerPresenter::notifyAudioEndOfFile()
{
    selectNextSong();
    player.play();
}

void AudioPlayerPresenter::backPressed()
{
    updateActiveSong();
}


void AudioPlayerPresenter::notifyMediaDisconnected()
{
    application()->gotoAudioPlayerScreen();
}

void AudioPlayerPresenter::notifyMediaConnected()
{
    application()->gotoAudioPlayerScreen();
}

void AudioPlayerPresenter::upPressed()
{
    char* pch = strrchr(currentFolder, fileinput::DirectorySeparator[0]);
    if (pch && (pch != currentFolder + 2))
    {
        *pch = 0; //end string at delimiter
        uint32_t n = fileinput::getDirectoryList(currentFolder, ".wav", dirlist, DIRLIST_LENGTH);
        view.setupDirlist(currentFolder, dirlist, n);
        //call in view
    }
}

void AudioPlayerPresenter::donePressed()
{
    updateActiveSong();
}

void AudioPlayerPresenter::folderSelected(uint32_t index)
{
    if (dirlist[index].isDirectory)
    {
        strcat(currentFolder, fileinput::DirectorySeparator);
        strcat(currentFolder, dirlist[index].name);

        uint32_t n = fileinput::getDirectoryList(currentFolder, ".wav", dirlist, DIRLIST_LENGTH);
        view.setupDirlist(currentFolder, dirlist, n);
    }
}

bool AudioPlayerPresenter::fileAdded(uint32_t index)
{
    uint32_t pl_index = PLAYLIST_LENGTH;
    for (int i = 0; i < PLAYLIST_LENGTH; i++)
    {
        if (playlist[i].used == false)
        {
            pl_index = i;
            break;
        }
    }
    if (pl_index == PLAYLIST_LENGTH)
    {
        return false; //no space
    }

    //set to first free place in play list
    playlist[pl_index].filename[0] = 0; //cleared
    strcat(playlist[pl_index].filename, currentFolder);
    strcat(playlist[pl_index].filename, fileinput::DirectorySeparator);
    strcat(playlist[pl_index].filename, dirlist[index].name);
    playlist[pl_index].used = true;
    playlist[pl_index].bmpId = BITMAP_COVER_UNKNOWN_ARTIST_ID;

    char coverFolder[50];
    char songFolder[50];

    getFolder(playlist[pl_index].filename, songFolder, 50);
    for (int i = 0; i < PLAYLIST_LENGTH; i++)
    {
        if (covers[i].used)
        {
            getFolder(covers[i].filename, coverFolder, 50);
            if (strcmp(coverFolder, songFolder) == 0)
            {
                playlist[pl_index].bmpId = covers[i].bmpId;
                break;
            }
        }
    }

    //If adding a song from an empty playlist
    uint8_t activeSongs = 0;
    for (int i = 0; i < PLAYLIST_LENGTH - 1; i++)
    {
        if (playlist[i].used == true)
        {
            activeSongs++;
        }
    }

    //If we've added a song to a an empty playlist, immediately select the song
    if (activeSongs == 1)
    {
        selectNextSong();
    }

    refreshPlaylist();

    return true;
}

void AudioPlayerPresenter::resetDirectoryList()
{
    strncpy(currentFolder, fileinput::getDrive(), sizeof(currentFolder));

    uint32_t n = fileinput::getDirectoryList(currentFolder, ".wav", dirlist, DIRLIST_LENGTH);
    view.setupDirlist(currentFolder, dirlist, n);
}


void AudioPlayerPresenter::refreshPlaylist()
{
    view.clearPlaylist();

    for (int i = 0; i < PLAYLIST_LENGTH; i++)
    {
        if (playlist[i].used == true)
        {
            char* fullname = playlist[i].filename;

            fullname++;

            char copy[50];
            char unknown[10];

            strncpy(copy, fullname, 50);

            char* extension = strrchr(copy, '.');
            *extension = 0; //remove extension

            char* filename = strrchr(copy, fileinput::DirectorySeparator[0]);
            *filename = 0; //split filename from artist
            filename++;
            char* artist = strrchr(copy, fileinput::DirectorySeparator[0]);
            if (artist)
            {
                *artist = 0;
                artist++;
            }
            else
            {
                strncpy(unknown, "Unknown", 10);
                artist = unknown;
            }

            view.addToPlaylist(playlist[i].bmpId, artist, filename);
        }
    }
}

bool AudioPlayerPresenter::isPlaylistEmpty()
{
    return !(playlist[0].used);
}

void AudioPlayerPresenter::updateActiveSong()
{
    bool empty = true;
    for (int i = 0; i < PLAYLIST_LENGTH - 1; i++)
    {
        if (playlist[i].used == true)
        {
            empty = false;
        }

    }

    //If play list empty
    if (empty)
    {
        char title[15] = "PLAYLIST EMPTY";
        view.setSongInfo(0, title, 0);
        view.setCover(BITMAP_COVER_UNKNOWN_ARTIST_ID);
    }
    else
    {
        view.setSongInfo(currentPlayListItem, currentSong, player.getSongDurationMs());
    }
}

void AudioPlayerPresenter::oneSecond()
{
    counter++;
    switch (counter)
    {
    case 6:
        view.setEqualizerSettings(-2, -7, -5, 2, -12);
        view.screenClick(ClickEvent::PRESSED, 320, 240);
        break;
    case 8:
        view.screenClick(ClickEvent::RELEASED, 320, 240);
        break;
    case 28:
        view.screenClick(ClickEvent::PRESSED, 528, 240);
        break;
    case 30:
        view.screenClick(ClickEvent::RELEASED, 528, 240);
        break;
    case 32:
        view.screenClick(ClickEvent::PRESSED, 528, 240);
        break;
    case 34:
        view.screenClick(ClickEvent::RELEASED, 528, 240);
        break;
    case 36:
        view.screenClick(ClickEvent::PRESSED, 528, 240);
        break;
    case 38:
        view.screenClick(ClickEvent::RELEASED, 528, 240);
        break;
    case 50:
        view.screenClick(ClickEvent::PRESSED, 497, 440);
        break;
    case 52:
        view.screenClick(ClickEvent::RELEASED, 497, 440);
        break;
    case 58:
        view.screenClick(ClickEvent::PRESSED, 72, 117);
        break;
    case 63:
        view.screenClick(ClickEvent::RELEASED, 72, 117);
        break;
    case 68:
        view.screenClick(ClickEvent::PRESSED, 548, 440);
        break;
    case 70:
          view.screenClick(ClickEvent::RELEASED, 548, 440);
        break;
    case 72:
        slideCnt = -7;
        break;
    case 78:
        slideCnt = -12;
        break;
    case 85:
        view.screenClick(ClickEvent::PRESSED, 34, 42);
        break;
    case 86:
        view.screenClick(ClickEvent::RELEASED, 34, 42);
        break;
    case 94:
        view.screenClick(ClickEvent::PRESSED, 34, 42);
        break;
    case 96:
        view.screenClick(ClickEvent::RELEASED, 34, 42);
        model->setAutoDemoDone(true);
        break;
    }
}

void AudioPlayerPresenter::presenterTick()
{
    if (tickCnt == 1)
    {
        if (counter > 73 && counter < 76)
        {
            view.setEqualizerSettings(-2, slideCnt, -5, 2, -12);
            slideCnt++;
        }

        if (counter > 78 && counter <= 80)
        {
            view.setEqualizerSettings(-2, 7, -5, 2, slideCnt);
            slideCnt++;
        }
        tickCnt = 0;
    }
    else
    {
        tickCnt++;
    }
}

void AudioPlayerPresenter::screenSaverMinorTick()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreen();
}
