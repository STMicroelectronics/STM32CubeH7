
#include <gui/audio_player_screen/BMPFileLoader.hpp>
#include <touchgfx/Color.hpp>
#ifdef SIMULATOR
#include <stdio.h>
#else
#include <string.h>
#include <ff.h>
#endif

uint8_t *BMPFileLoader::data = 0;

int BMPFileLoader::readFile(FileHdl hdl, uint8_t* const buffer, uint32_t length)
{
  if (!hdl)
  {
    return 1;
  }
#ifdef SIMULATOR
    uint32_t r = fread(buffer, 1, length, (FILE*)hdl);
#else
    uint32_t r = 0;
    f_read((FIL*)hdl, buffer, length, (unsigned int*)&r);

#endif
    if (r != length)
    {
        return 1;
    }
    return 0;
}

void BMPFileLoader::seekFile(FileHdl hdl, uint32_t offset)
{
  if (!hdl)
  {
    return;
  }
#ifdef SIMULATOR
    fseek((FILE*)hdl, offset, SEEK_SET);
#else
    f_lseek((FIL*)hdl, offset);
#endif
}

void BMPFileLoader::getBMP24Dimensions(FileHdl fileHandle, uint16_t& width, uint16_t& height)
{
    if (!fileHandle)
    {
      width = 0;
      height = 0;
      return;
    }

    if (!data) {
        BitmapId id = Bitmap::dynamicBitmapCreate(256, 1, Bitmap::RGB888);
        data = const_cast<uint8_t*>(Bitmap(id).getData());
    }

    seekFile(fileHandle, 0);
    readFile(fileHandle, data, 26); //read first part of header.

    width = data[18] | (data[19] << 8) | (data[20] << 16) | (data[21] << 24);
    height = data[22] | (data[23] << 8) | (data[24] << 16) | (data[25] << 24);
}

void BMPFileLoader::readBMP24File(Bitmap bitmap, FileHdl fileHandle)
{
    if (!fileHandle)
    {
      return;
    }

    if (!data) {
        BitmapId id = Bitmap::dynamicBitmapCreate(640, 1, Bitmap::RGB888);
        data = const_cast<uint8_t*>(Bitmap(id).getData());
    }

    seekFile(fileHandle, 0);
    readFile(fileHandle, data, 26); //read first part of header.

    const uint32_t offset = data[10] | (data[11] << 8) | (data[12] << 16) | (data[12] << 24);
    const uint32_t width = data[18] | (data[19] << 8) | (data[20] << 16) | (data[21] << 24);
    const uint32_t height = data[22] | (data[23] << 8) | (data[24] << 16) | (data[25] << 24);

    //could check offset here
    readFile(fileHandle, data, offset - 26); //read rest of header.

    const uint32_t buffer_width = bitmap.getWidth();
    const uint32_t rowpadding = (4 - ((width * 3) % 4)) % 4;
    uint8_t* const  buffer8 = Bitmap::dynamicBitmapGetAddress(bitmap.getId());
    
    for (uint32_t y = 0; y < height; y++)
    {
        uint32_t inx;
        uint16_t *dest;
        readFile(fileHandle, data, (width - 0) * 3 + rowpadding); //rest of line
        
        switch (bitmap.getFormat()) {
        case Bitmap::RGB888:
            inx = 3 * (0 + (height - y - 1) * buffer_width);
            memcpy(&buffer8[inx], data, 768);
            break;
        case Bitmap::RGB565:
            inx = 2 * (0 + (height - y - 1) * buffer_width);
            dest = (uint16_t*)&buffer8[inx];
            for (int i = 0; i < 256; i++) {
                *dest++ =   ((data[3 * i + 0] & 0xf8) << 8)
                          | ((data[3 * i + 1] & 0xfc) << 3)
                          | ((data[3 * i + 2] & 0xf8) >> 3);
            }
            break;
        default:
            assert(0 && "Unsupported destination buffer");
            break;
        }
    }
}
