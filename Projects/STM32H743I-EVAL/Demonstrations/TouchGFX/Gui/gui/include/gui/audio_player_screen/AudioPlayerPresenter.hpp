#ifndef AUDIO_PLAYER_PRESENTER_HPP
#define AUDIO_PLAYER_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>
#include <gui/common/Playlist.hpp>
#include <gui/common/CommonDefinitions.hpp>
#include <gui/video_player_screen/FileInput.hpp>
#include <gui/audio_player_screen/AudioPlayer.hpp>
#include <gui/audio_player_screen/CoverElement.hpp>
#include <gui/audio_player_screen/AudioFolderWidget.hpp>

using namespace touchgfx;

class AudioPlayerView;

class AudioPlayerPresenter : public DemoPresenter/*, AudioPlaylistManager*/
{
public:
    AudioPlayerPresenter(AudioPlayerView& v);
    virtual ~AudioPlayerPresenter() {};

    virtual void activate();
    void initializeFileSystem(); 
    virtual void deactivate();

    void playPauseClicked();
    void selectSong(int index);
    void selectPreviousSong();
    void setSongProgressPercentage(uint32_t prog);
    bool isPlaylistEmpty();
    void selectNextSong();
    void setVolume(int val);
    uint32_t getSongProgressMs();
    bool isAudioPlaying()
    {
        return player.isPlaying();
    }
    AudioPlayer* getPlayer()
    {
        return &player;
    }
    PlaylistElement* getPlayList()
    {
        return &playlist[0];
    }

    bool addFile(uint32_t index);
    bool removeFile(uint32_t index);
    bool fileAdded(uint32_t index);

    /* Events from Model/backend */
    virtual void notifyAudioEndOfFile();
    virtual void notifyMediaDisconnected();
    virtual void notifyMediaConnected();

    void backPressed();
    void upPressed();
    void donePressed();
    void folderSelected(uint32_t index);
    void resetDirectoryList();

    virtual void oneSecond();
    virtual void presenterTick();
    virtual void screenSaverMinorTick();
    
private:
    void getFolder(char* file, char* buffer, uint32_t max);
    CoverElement covers[PLAYLIST_LENGTH];
    PlaylistElement playlist[PLAYLIST_LENGTH];
    fileinput::DirEntry filelist[DIRLIST_LENGTH];
    fileinput::DirEntry dirlist[DIRLIST_LENGTH];

    AudioPlayer player;
    char currentFolder[50];
    char currentSong[50];
    AudioPlayerPresenter();
    void readCover(CoverElement& cover);
    void refreshPlaylist();
    void updateActiveSong();
    AudioPlayerView& view;
    uint32_t currentPlayListItem;

    uint16_t counter;
    int16_t slideCnt;
    uint16_t tickCnt;
};

#endif // AUDIO_PLAYER_PRESENTER_HPP
