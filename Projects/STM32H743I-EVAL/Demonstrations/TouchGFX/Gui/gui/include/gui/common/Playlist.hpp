#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include <touchgfx/Bitmap.hpp>

struct PlaylistElement
{
    char filename[60];
    touchgfx::BitmapId bmpId;
    bool used;
};

#endif
