#include <widgets/MJPEGWidget.hpp>
#include <touchgfx/Color.hpp>
#include <widgets/FileInput.hpp>

#include "jinclude.h"
#include "jpeglib.h"

namespace touchgfx
{

MJPEGWidget::MJPEGWidget() :
    PixelDataWidget(), frameno(0), offset(0),
    microSecPerFrame(0), firstFrameOffset(0), lastFrameEnd(0),
    timeBetweenTicks(16),
    movieLength(0), movieData(0)
{

}

void MJPEGWidget::setLineBuffer(uint8_t* buffer)
{
    lineBuffer = buffer;
}

void MJPEGWidget::setTimeBetweenTicks(uint32_t ms)
{
    timeBetweenTicks = ms;
}

void MJPEGWidget::setMovie(const uint8_t* movie, const uint32_t length)
{
    movieData = movie;
    movieLength = length;

    //decode header to find first frame
    const char* mpgdata = reinterpret_cast<const char*>(movieData); //as string

    //Must be RIFF file
    if (strncmp(mpgdata + offset, "RIFF", 4))
    {
        assert(!"RIFF header not found");
    }

    //skip fourcc and length
    offset += 8;
    if (strncmp(mpgdata + offset, "AVI ", 4))
    {
        assert(!"AVI header not found");
    }
    offset += 4;
    if (strncmp(mpgdata + offset, "LIST", 4))
    {
        assert(!"AVI LIST not found");
    }
    //save avi List info
    const uint32_t aviListSize = getU32(movieData + offset + 4);
    const uint32_t aviListOffset = offset;
    assert(aviListSize);

    //look into header to find frame rate
    offset += 8;
    if (strncmp(mpgdata + offset, "hdrl", 4))
    {
        assert(!"hdrl not found");
    }
    offset += 4;
    if (strncmp(mpgdata + offset, "avih", 4))
    {
        assert(!"avih not found");
    }
    offset += 8; //skip fourcc and cb in AVIMAINHEADER
    microSecPerFrame = getU32(movieData + offset);
    assert(microSecPerFrame);

    //skip rest of AVI header, start from end of AVI List

    //look for list with 'movi' header
    uint32_t listOffset = aviListOffset + aviListSize + 8;
    while (strncmp(mpgdata + listOffset + 8, "movi", 4) && listOffset < length)
    {
        const uint32_t listSize = getU32(movieData + listOffset + 4) + 8;
        listOffset += listSize;
    }

    //check for EOF
    assert(listOffset < length);

    //save first frame and end of last frame
    offset = listOffset + 8 + 4; //skip LIST and 'movi'
    frameno = 1; //next frame no is 1
    firstFrameOffset = offset;
    lastFrameEnd = listOffset + 8 + getU32(movieData + listOffset + 4);
}

void MJPEGWidget::gotoFrame(uint32_t frameno)
{
    offset = firstFrameOffset; //start over
    uint32_t frame = 1;
    while (frame <= frameno)
    {
        const uint8_t* chunk = movieData + offset;

        uint32_t streamNo  = getU16(chunk);
        uint32_t chunkType = getU16(chunk + 2);
        uint32_t chunkSize = getU32(chunk + 4);
        const uint16_t STREAM0 = 0x3030;
        const uint16_t TYPEDC  = 0x6364;

        //traverse frames
        if (offset + 8 + chunkSize < movieLength)
        {
            if (streamNo == STREAM0 && chunkType == TYPEDC)
            {
                //decode frame
                if (frame == frameno)
                {
                    decodeMJPEGFrame(chunk + 8, chunkSize);
                }
            }

            offset += chunkSize + 8;
            offset = (offset + 1) & 0xFFFFFFFE; //pad to next word
            if (offset == lastFrameEnd)
            {
                offset = firstFrameOffset; //start over
                return;
            }
        }
        else
        {
            offset = firstFrameOffset; //start over
            return;
        }
        frame++;
    }
}

bool MJPEGWidget::tick()
{
    if (frameno > 0)
    {
        //find next frame and decode it
        const uint8_t* chunk = movieData + offset;

        uint32_t streamNo  = getU16(chunk);
        uint32_t chunkType = getU16(chunk + 2);
        uint32_t chunkSize = getU32(chunk + 4);
        const uint16_t STREAM0 = 0x3030;
        const uint16_t TYPEDC  = 0x6364;

        //play frame if we have it all
        if (offset + 8 + chunkSize < movieLength)
        {
            if (streamNo == STREAM0 && chunkType == TYPEDC)
            {
                //decode frame
                decodeMJPEGFrame(chunk + 8, chunkSize);
                invalidate();
            }

            offset += chunkSize + 8;
            offset = (offset + 1) & 0xFFFFFFFE; //pad to next word
            frameno++;

            if (offset == lastFrameEnd)
            {
                frameno = 1;
                offset = firstFrameOffset; //start over
                return false;
            }
        }
        else
        {
            frameno = 1;
            offset = firstFrameOffset; //start over
            return false;
        }
        return true;
    }
    return false;
}

void MJPEGWidget::decodeMJPEGFrame(const uint8_t* const mjpgdata, const uint32_t length)
{

    if (length == 0)
    {
        return;
    }

    if (buffer && lineBuffer) //only decode if buffers are assigned.
    {
        /* This struct contains the JPEG decompression parameters */
        struct jpeg_decompress_struct cinfo;
        /* This struct represents a JPEG error handler */
        struct jpeg_error_mgr jerr;

        JSAMPROW lines[2] = {lineBuffer, 0}; /* Output row buffer */

        /* Step 1: allocate and initialize JPEG decompression object */
        cinfo.err = jpeg_std_error(&jerr);

        /* Initialize the JPEG decompression object */
        jpeg_create_decompress(&cinfo);

        //jpeg_stdio_src (&cinfo, file);
        jpeg_mem_src(&cinfo, const_cast<unsigned char *>(mjpgdata), length);

        /* Step 3: read image parameters with jpeg_read_header() */
        jpeg_read_header(&cinfo, TRUE);

        /* Step 4: set parameters for decompression */
        cinfo.dct_method = JDCT_FLOAT;

        /* Step 5: start decompressor */
        jpeg_start_decompress(&cinfo);

        //const uint32_t width = cinfo.image_width;  //maybe from widget
        const uint32_t widget_width = getWidth();
        const uint32_t widget_height = getHeight();
        const uint32_t output_width = MIN(widget_width, cinfo.image_width);
        const uint32_t output_height = MIN(widget_height, cinfo.output_height);

        const bool lcd16bit = (HAL::lcd().bitDepth() == 16);

        if (lcd16bit)
        {
            uint16_t* lineptr = (uint16_t*)buffer;
            while (cinfo.output_scanline < output_height)
            {
                (void) jpeg_read_scanlines(&cinfo, lines, 1);
                RGB* RGB_matrix = (RGB*)lineBuffer;
                //loop row
                for (uint32_t counter = 0; counter < output_width; counter++)
                {
                    const uint16_t pix = ((RGB_matrix[counter].R & 0xF8) << 8) | ((RGB_matrix[counter].G & 0xFC) << 3) | ((RGB_matrix[counter].B & 0xF7) >> 3);
                    *(lineptr + counter) = pix;
                }
                lineptr += widget_width; //move to previous line
            }
        }
        else
        {
            uint8_t* lineptr = buffer;
            while (cinfo.output_scanline < cinfo.output_height)
            {
                (void) jpeg_read_scanlines(&cinfo, lines, 1);
                RGB* RGB_matrix = (RGB*)lineBuffer;
                //loop row
                for (uint32_t counter = 0; counter < output_width; counter++)
                {
                    *lineptr++ = RGB_matrix[counter].B;
                    *lineptr++ = RGB_matrix[counter].G;
                    *lineptr++ = RGB_matrix[counter].R;
                }
            }
        }
#ifdef SIMULATOR
        cinfo.output_scanline = cinfo.output_height;
#endif
        /* Step 6: Finish decompression */
        jpeg_finish_decompress(&cinfo);

        /* Step 7: Release JPEG decompression object */
        jpeg_destroy_decompress(&cinfo);
    }
}

uint32_t MJPEGWidget::getU32(const uint8_t* d)
{
    return d[0] | (d[1] << 8) | (d[2] << 16) | (d[3] << 24);
}

uint32_t MJPEGWidget::getU16(const uint8_t* d)
{
    return d[0] | (d[1] << 8);
}

} // namespace touchgfx
