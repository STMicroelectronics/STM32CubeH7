#include <widgets/FileInput.hpp>
#include "jinclude.h"

//link libjpeg file operation to fileinput namespace
extern "C"
{
    size_t jpeg_read_file(FILE*  file, uint8_t* buf, uint32_t sizeofbuf);
    size_t jpeg_read_file(FILE*  file, uint8_t* buf, uint32_t sizeofbuf)
    {
        return touchgfx::fileinput::readFile(file, buf, sizeofbuf);
    }
}
