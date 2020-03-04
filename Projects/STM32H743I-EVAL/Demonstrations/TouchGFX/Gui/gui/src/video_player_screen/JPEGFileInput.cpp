#include <gui/video_player_screen/FileInput.hpp>
#include "jinclude.h"

#include <stdint.h>

#ifdef SIMULATOR
#include <stdio.h>
#define FIL FILE
#endif

//link LibJPEG file read operation to touchgfx::fileinput function.
extern "C"
{
    size_t jpeg_read_file (FIL *file, uint8_t *buf, uint32_t sizeofbuf);

    size_t jpeg_read_file (FIL *file, uint8_t *buf, uint32_t sizeofbuf)
    {
        return touchgfx::fileinput::readFile(file, buf, sizeofbuf);
    }
}

