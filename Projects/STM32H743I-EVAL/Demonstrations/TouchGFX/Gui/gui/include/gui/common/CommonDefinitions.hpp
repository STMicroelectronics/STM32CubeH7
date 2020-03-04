#ifndef COMMON_DEFINITIONS_HPP
#define COMMON_DEFINITIONS_HPP

enum Action
{
    HW_ON,
    HW_OFF,
    REPEAT_ON,
    REPEAT_OFF,
    PLAY,
    PAUSE,
    REPEAT,
    PLAYLIST,
    PLAYLIST_BACK,
    FOLDER,
    EQUALIZER
};

enum PlayerState
{
    STOPPED,
    PAUSED,
    PLAYING
};

enum
{
    PLAYLIST_LENGTH   = 20,
    DIRLIST_LENGTH    = 20,
    FILENAME_LENGTH   = 20,
    FOLDERNAME_LENGTH = 40
};

#define RGB24BYTES(w, h) ((w)*(h)*3)

#endif
