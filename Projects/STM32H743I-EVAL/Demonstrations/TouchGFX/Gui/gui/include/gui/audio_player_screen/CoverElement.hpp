#ifndef COVER_ELEMENT_HPP
#define COVER_ELEMENT_HPP

#include <touchgfx/Bitmap.hpp>

struct CoverElement
{
    char filename[60];
    touchgfx::BitmapId bmpId;
    bool used;
};


#endif
