
#include <touchgfx/Color.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <widgets/MJPEGReader.hpp>

#ifndef SIMULATOR
#include <cmsis_os.h>

#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_jpeg.h>

#include "hwjpeg_decode.h"

extern struct HwJPEG_Context_s HwJPEG_Context;

osMessageQId FrameEvent = 0;
#endif

#include <setjmp.h>

#include "jinclude.h"
#include "jpeglib.h"


namespace touchgfx
{
struct my_error_mgr
{
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

extern "C"
{
    void jpeglib_panic(j_common_ptr cinfo);

    void jpeglib_panic(j_common_ptr cinfo)
    {
        struct my_error_mgr* myerr = (struct my_error_mgr*) cinfo->err;
        longjmp(myerr->setjmp_buffer, 1);
    }
}


MJPEGReader::MJPEGReader()
    : movieEndedAction(0),
      frameBuffer(0),
      temporaryFrameBuffer(0),
      lineBuffer(0),
      fileBuffer(0),
      bufferStartOffset(0),
      dataLength(0),
      aviFile(0),
      lastError(AVI_NO_ERROR),
      fps(0),
      frameCounter(0),
      frameTick16frame(0),
      frameTickLost(0),
      msBetweenTicks(0),
      msTime(0),
      frameNumber(0),
      outputX(0),
      outputY(0),
      outputWidth(0),
      outputHeight(0),
      outputRect(),
      playing(false),
      repeating(false),
      headerRead(false),
      msPerFrame(0),
      numberOfFrames(0),
      frameWidth(0),
      frameHeight(0),
      firstFrameOffset(0),
      indexOffset(0)
{
#ifdef SIMULATOR
    useHWDecoding = false;
#else
    useHWDecoding = true;
#endif
}

inline uint32_t MJPEGReader::getU32(const uint32_t offset)
{
    const uint32_t bufOff = offset - bufferStartOffset;
    return fileBuffer[bufOff + 0] | (fileBuffer[bufOff + 1] << 8) | (fileBuffer[bufOff + 2] << 16) | (fileBuffer[bufOff + 3] << 24);
}

inline uint32_t MJPEGReader::getU16(const uint32_t offset)
{
    const uint32_t bufOff = offset - bufferStartOffset;
    return fileBuffer[bufOff + 0] | (fileBuffer[bufOff + 1] << 8);
}

uint32_t MJPEGReader::compare(const uint32_t offset, const char* str, uint32_t num)
{
    const char* src = reinterpret_cast<const char*>(fileBuffer + (offset - bufferStartOffset));
    return strncmp(src, str, num);
}

void MJPEGReader::setOutputWidget(PixelDataWidget& widget)
{
    this->widget = &widget;
}

void MJPEGReader::setLineBuffer(uint8_t* const buffer)
{
    lineBuffer = buffer;
}

void MJPEGReader::setFileBuffer(uint8_t* const buffer, uint32_t size)
{
    fileBuffer = buffer;
    bufferSize = size;
}

void MJPEGReader::setFrameBuffer(uint8_t* const buffer)
{
    frameBuffer = buffer;
}

void MJPEGReader::setTemporaryFrameBuffer(uint8_t* const buffer)
{
    temporaryFrameBuffer = buffer;
}

void MJPEGReader::setTimeBetweenTicks(uint32_t ms)
{
    msBetweenTicks = ms;
}

void MJPEGReader::play()
{
    playing = true;
}

void MJPEGReader::setRepeat(bool repeat)
{
    repeating = repeat;
}

void MJPEGReader::pause()
{
    playing = false;
}

void MJPEGReader::showFirstFrame()
{
    playOffset = firstFrameOffset;

    outputWidth = getWidth();
    outputHeight = getHeight();

#ifdef SIMULATOR
    memset(frameBuffer, 0, outputWidth * outputHeight * 3);
#endif

    //center vertically
    outputY = (int32_t)(outputHeight - frameHeight) / 2;
    //center horozontally
    outputX = (int32_t)(outputWidth - frameWidth) / 2;
    outputRect = Rect(outputX, outputY, frameWidth, frameHeight);
    showNextFrame();
}

void MJPEGReader::gotoFrame(uint32_t frameNumber)
{
    if (frameNumber == 0)
    {
        frameNumber = 1;
    }
    uint32_t offset = indexOffset + 8 + (frameNumber - 1) * 16;

    readData(offset, 16);

    playOffset = getU32(offset + 8) + firstFrameOffset - 4;
    this->frameNumber = frameNumber;

    //set time
    msTime = frameNumber * msPerFrame;

    //skip empty frames, show first frame after
    while (!showNextFrame() && (this->frameNumber < this->numberOfFrames))
    {
        //nop
    }

}

void MJPEGReader::skipFrame()
{
    showNextFrame(false);
}

bool MJPEGReader::showNextFrame(bool show)
{
    //output frame
    //decode frame to  buffer
    uint32_t offset = playOffset;

    if (offset == firstFrameOffset)
    {
        frameNumber = 0;
        msTime = 0;
    }

    readData(offset, 8);
    uint32_t streamNo  = getU16(offset);
    uint32_t chunkType = getU16(offset + 2);
    uint32_t chunkSize = getU32(offset + 4);
    const uint16_t STREAM0 = 0x3030;
    const uint16_t TYPEDC  = 0x6364;

    if (lastError == AVI_NO_ERROR)
    {
        if (streamNo == STREAM0 && chunkType == TYPEDC)
        {
            //count frame
            frameNumber++;
            if (show)
            {
                // uint16_t *framebuffer = HAL::getInstance()->lockFrameBuffer();
                // HAL::HAL::getInstance()->unlockFrameBuffer();

                if (!decodeFrame(offset, frameBuffer))
                {
                    //forward to next frame
                    playOffset += chunkSize + 8;
                    playOffset = (playOffset + 1) & 0xFFFFFFFE; //pad to next word
                    return false;
                }
//                if (useHWDecoding && playing)
//                {
//                    Rect invalidRect(0, 0, 1, 1);
//                    invalidateRect(invalidRect);
//                }
//                else
                {
                    Rect rect = Rect(outputX, outputY, frameWidth, frameHeight);
                    invalidateRect(rect); /*outputRect*/
                }
            }
        }
        //forward to next frame
        playOffset += chunkSize + 8;
        playOffset = (playOffset + 1) & 0xFFFFFFFE; //pad to next word
    }
    else
    {
        playing = (repeating) ? true : false;
        //allow it to play again
        if (lastError == AVI_ERROR_EOF_REACHED)
        {
            playOffset = firstFrameOffset;
            lastError = AVI_NO_ERROR;
            if (!repeating)
            {
                if (movieEndedAction && movieEndedAction->isValid())
                {
                    movieEndedAction->execute();
                }
            }
        }
    }
    return true;
}
void MJPEGReader::setHWDecodingEnabled(bool enabled)
{
#ifndef SIMULATOR
    useHWDecoding = enabled;
#endif
}

void MJPEGReader::tick()
{
    if (playing)
    {
        const uint32_t lostTicks = HAL::getInstance()->getLCDRefreshCount();

        frameTickLost = MAX(frameTickLost, lostTicks);

        if (lostTicks == 1)
        {
            //calculate fps
            if (frameCounter == 25)
            {
                // 25000 ms / time for 25 displayed frames
                fps = (25 * 10000) / (frameTick16frame * 156); /* (msBetweenTicks - 0.4) * 10 */

                frameTick16frame = 0;
                frameCounter = 0;
            }
            frameTick16frame += frameTickLost;

            //update wall time
            msTime += msBetweenTicks * frameTickLost;

            //decide on next frame
            const uint32_t newFrameNo = getFrameNoForMs(msTime);
            int skip = newFrameNo - (frameNumber + 1);
            if (skip < 0)
            {
                frameTickLost = 0;
                return; /* Ahead of decoding / presentation time */
            }

            while (skip > 0 && frameNumber < numberOfFrames)
            {
                skip --;
                skipFrame();
            }

            frameCounter++;
            showNextFrame();

            //reset for next tick
            frameTickLost = 0;

        }
    }
}

void MJPEGReader::readData(uint32_t offset, uint32_t length)
{
    if (length > bufferSize)
    {
        lastError = AVI_ERROR_FILE_BUFFER_TO_SMALL;
        assert(!"Buffer too small");
    }

    uint32_t n = 0;

    if (aviFile)
    {
        fileinput::seekFile(aviFile, offset);
        n = fileinput::readFile(aviFile, fileBuffer, length);
    }
    else
    {
        n = (offset + length) > movieLength ? movieLength - offset : length;
        memcpy(fileBuffer, movieData + offset, n);
    }
    bufferStartOffset = offset;
    dataLength = n; //length;

    if (n < length)
    {
        lastError = AVI_ERROR_EOF_REACHED;
    }
}

bool MJPEGReader::setMovie(const uint8_t* movie, const uint32_t length)
{
    uint32_t offset = 0;

    movieData = movie;
    movieLength = length;

    headerRead = false;
    lastError = AVI_NO_ERROR;

    //if (fileHandle == 0)
    //{
    //    return false;
    //}

    frameNumber = 0;
    msTime = 0;

    //aviFile = fileHandle;

    //decode header to find first frame
    readData(0, 72);

    //Must be RIFF file
    if (compare(offset, "RIFF", 4))
    {
        lastError = AVI_ERROR_NOT_RIFF;
        return false;
    }

    //skip fourcc and length
    offset += 8;
    if (compare(offset, "AVI ", 4))
    {
        lastError = AVI_ERROR_AVI_HEADER_NOT_FOUND;
        return false;
    }

    offset += 4;
    if (compare(offset, "LIST", 4))
    {
        lastError = AVI_ERROR_AVI_LIST_NOT_FOUND;
        return false;
    }

    //save avi List info
    const uint32_t aviListSize = getU32(offset + 4);
    const uint32_t aviListOffset = offset;
    if (!aviListSize)
    {
        return false;
    }

    //look into header to find frame rate
    offset += 8;
    if (compare(offset, "hdrl", 4))
    {
        lastError = AVI_ERROR_AVI_HDRL_NOT_FOUND;
        return false;
    }

    offset += 4;
    if (compare(offset, "avih", 4))
    {
        lastError = AVI_ERROR_AVI_AVIH_NOT_FOUND;
        return false;
    }

    offset += 8; //skip fourcc and cb in AVIMAINHEADER
    msPerFrame = getU32(offset) / 1000;
    numberOfFrames = getU32(offset + 16);
    frameWidth = getU32(offset + 32);
    frameHeight = getU32(offset + 36);

    //skip rest of AVI header, start from end of AVI List

    //look for list with 'movi' header
    uint32_t listOffset = aviListOffset + aviListSize + 8;
    readData(listOffset, 12);
    while (compare(listOffset + 8, "movi", 4) && (lastError == AVI_NO_ERROR))
    {
        const uint32_t listSize = getU32(listOffset + 4) + 8;
        listOffset += listSize;
        readData(listOffset, 12);
    }

    if (lastError != AVI_NO_ERROR)
    {
        lastError = AVI_ERROR_MOVI_NOT_FOUND;
        return false;
    }

    //save first frame offset
    firstFrameOffset = listOffset + 8 + 4; //skip LIST and 'movi'

    //find idx
    const uint32_t listSize = getU32(listOffset + 4) + 8;
    listOffset += listSize;
    readData(listOffset, 12);
    if (!compare(listOffset, "idx1", 4) && (lastError == AVI_NO_ERROR))
    {
        indexOffset = listOffset;
    }
    else
    {
        lastError = AVI_ERROR_IDX1_NOT_FOUND;
        return false;
    }

    headerRead = true;

    return true;
}

bool MJPEGReader::decodeThumbNail(Bitmap bmp, uint32_t frameNumber)
{
    uint32_t offset = firstFrameOffset;
    uint32_t frameno = 0;

    if (headerRead == false)
    {
        lastError = AVI_NO_FILE;
        return false;
    }

    if (!(temporaryFrameBuffer && lineBuffer))
    {
        lastError = AVI_NO_BUFFERS;
        return false;
    }

    const uint32_t width = bmp.getWidth();
    const uint32_t height = bmp.getHeight();
    uint8_t* const buffer = Bitmap::dynamicBitmapGetAddress(bmp.getId());

    if (frameNumber + 1 > numberOfFrames)
    {
        lastError = AVI_ERROR_FRAMENO_TO_HIGH;
        return false;
    }

    //find frame
    while (frameno < frameNumber)
    {
        //const uint8_t* chunk = fileBuffer + offset;
        readData(offset, 8);
        uint32_t streamNo  = getU16(offset);
        uint32_t chunkType = getU16(offset + 2);
        uint32_t chunkSize = getU32(offset + 4);
        const uint16_t STREAM0 = 0x3030;
        const uint16_t TYPEDC  = 0x6364;

        //do we have it all
        if (lastError == AVI_NO_ERROR)
        {
            if (streamNo == STREAM0 && chunkType == TYPEDC)
            {
                //count frame
                frameno++;
            }
            offset += chunkSize + 8;
            offset = (offset + 1) & 0xFFFFFFFE; //pad to next word
        }
        else
        {
            //EOF
            lastError  = AVI_ERROR_EOF_REACHED;
            return false;
        }
        //should check that next frame is a DC chunk
    }

    if ((width != frameWidth) || (height != frameHeight))
    {
        //decode full frame to temporary buffer
        outputX = outputY = 0;
        outputWidth = frameWidth;
        outputHeight = frameHeight;
        // always decode thumbnails in software
        if (!decodeFrame(offset, temporaryFrameBuffer, 0))
        {
            return false;
        }
        //output scaled image to buffer, nearest neighbor
        uint8_t* pDst = buffer;

        const uint32_t rowStep = (frameHeight / height) * (frameWidth * 3);
        const uint32_t rowErr  = frameHeight % height;

        const uint32_t colStep = frameWidth / width;
        const uint32_t colErr  = frameWidth % width;
        const uint32_t rgbStep = (colStep - 1) * 3;

        uint8_t* pSrc0 = &temporaryFrameBuffer[0];
        uint32_t rowError = 0;

        for (uint32_t y = 0; y < height; y++)
        {
            uint32_t colAccumError = 0;
            uint8_t* pSrc = pSrc0;
            for (uint32_t x = 0; x < width; x++)
            {
                *pDst++ = *pSrc++; //RGB
                *pDst++ = *pSrc++;
                *pDst++ = *pSrc++;

                pSrc += rgbStep;
                colAccumError += colErr;
                if (colAccumError > width)
                {
                    pSrc += 3;
                    colAccumError -= width;
                }
            }
            pSrc0 += rowStep;

            rowError += rowErr;
            if (rowError > height)
            {
                pSrc0 += frameWidth * 3;
                rowError -= height;
            }
        }
    }
    else
    {
        //directly to buffer
        return decodeFrame(offset, buffer, 0);
    }

    return true;
}

bool MJPEGReader::decodeFrame(uint32_t offset, uint8_t* const outputBuf, int mode)
{
#ifndef SIMULATOR
    if (useHWDecoding && mode)
    {
        uint32_t length = 0;

        //find length of chunk
        readData(offset, 8);
        length = getU32(offset + 4);

        if (length == 0 || length > bufferSize)
        {
            //skip empty frame or too large frames
            return false;
        }

        //read whole frame
        offset += 8;
        readData(offset, length);

        JPEG_Decode_DMA(&HwJPEG_Context, fileBuffer, dataLength, (uint32_t)temporaryFrameBuffer, (uint32_t)(!outputBuf ? buffer : outputBuf), frameWidth);
    }
    else
#endif
    {
        uint32_t length = 0;

        //find length of chunk
        readData(offset, 8);
        length = getU32(offset + 4);

        if (length == 0 || length > bufferSize)
        {
            //skip empty frame or too large frames
            return false;
        }

        //read whole frame
        offset += 8;
        readData(offset, length);

        if (lastError == AVI_NO_ERROR)
        {
            /* This struct contains the JPEG decompression parameters */
            struct jpeg_decompress_struct cinfo;
            /* This struct represents a JPEG error handler */
            struct my_error_mgr jerr;

            JSAMPROW lines[2] = {lineBuffer, 0}; /* Output row buffer */

            /* Step 1: allocate and initialize JPEG decompression object */
            cinfo.err = jpeg_std_error(&jerr.pub);
            jerr.pub.error_exit = jpeglib_panic;

            if (setjmp(jerr.setjmp_buffer))
            {
                jpeg_destroy_decompress(&cinfo);
                return false;
            }

            /* Initialize the JPEG decompression object */
            jpeg_create_decompress(&cinfo);

            jpeg_mem_src(&cinfo, fileBuffer, length);

            /* Step 3: read image parameters with jpeg_read_header() */
            jpeg_read_header(&cinfo, TRUE);

            /* Step 4: set parameters for decompression */
            cinfo.dct_method = JDCT_FLOAT;

            /* Step 5: start decompressor */
            jpeg_start_decompress(&cinfo);

            uint8_t* lineptr = !outputBuf ? buffer : outputBuf;
            //center vertically
            lineptr += outputY * outputWidth * 3;
            //center horozontally
            lineptr += outputX * 3;

            while (cinfo.output_scanline < frameHeight)
            {
                (void) jpeg_read_scanlines(&cinfo, lines, 1);
                //loop row
                uint32_t* dest = (uint32_t*)lineptr;
                uint8_t* src = lineBuffer;
                //copy 12 bytes for each 4 pixels
                for (uint32_t counter = 0; counter < frameWidth; counter += 4)
                {
                    // Read bytes from source, and write it as 3 32bit
                    // R1 G1 B1 R2 | G2 B2 R3 G3 | B3 R4 G4 B4
                    const unsigned int b1 = *src++;
                    const unsigned int g1 = *src++;
                    const unsigned int r1 = *src++;
                    const unsigned int b2 = *src++;
                    const unsigned int g2 = *src++;
                    const unsigned int r2 = *src++;

                    const uint32_t pix1 = (r2 << 24) | (b1 << 16) | (g1 << 8) | r1;
                    *dest++ = pix1;

                    const unsigned int b3 = *src++;
                    const unsigned int g3 = *src++;
                    const unsigned int r3 = *src++;

                    const uint32_t pix2 = (g3 << 24) | (r3 << 16) | (b2 << 8) | g2;
                    *dest++ = pix2;

                    const unsigned int b4 = *src++;
                    const unsigned int g4 = *src++;
                    const unsigned int r4 = *src++;
                    const uint32_t pix3 = (b4 << 24) | (g4 << 16) | (r4 << 8) | b3;
                    *dest++ = pix3;
                }

                lineptr += outputWidth * 3;
            }

            /* Step 6: Finish decompression */
            jpeg_finish_decompress(&cinfo);

            /* Step 7: Release JPEG decompression object */
            jpeg_destroy_decompress(&cinfo);
        }
    }

    return true;
}

bool MJPEGReader::closeFile()
{
    //close old file.
    if (aviFile != 0)
    {
        fileinput::closeFile(aviFile);
        aviFile = 0;
        return true;
    }
    return false;
}

bool MJPEGReader::setAVIFile(fileinput::FileHdl fileHandle)
{
    uint32_t offset = 0;

    headerRead = false;
    lastError = AVI_NO_ERROR;

    if (fileHandle == 0)
    {
        return false;
    }

    frameNumber = 0;
    msTime = 0;

    aviFile = fileHandle;

    //decode header to find first frame
    readData(0, 72);

    //Must be RIFF file
    if (compare(offset, "RIFF", 4))
    {
        lastError = AVI_ERROR_NOT_RIFF;
        return false;
    }

    //skip fourcc and length
    offset += 8;
    if (compare(offset, "AVI ", 4))
    {
        lastError = AVI_ERROR_AVI_HEADER_NOT_FOUND;
        return false;
    }

    offset += 4;
    if (compare(offset, "LIST", 4))
    {
        lastError = AVI_ERROR_AVI_LIST_NOT_FOUND;
        return false;
    }

    //save avi List info
    const uint32_t aviListSize = getU32(offset + 4);
    const uint32_t aviListOffset = offset;
    if (!aviListSize)
    {
        return false;
    }

    //look into header to find frame rate
    offset += 8;
    if (compare(offset, "hdrl", 4))
    {
        lastError = AVI_ERROR_AVI_HDRL_NOT_FOUND;
        return false;
    }

    offset += 4;
    if (compare(offset, "avih", 4))
    {
        lastError = AVI_ERROR_AVI_AVIH_NOT_FOUND;
        return false;
    }

    offset += 8; //skip fourcc and cb in AVIMAINHEADER
    msPerFrame = getU32(offset) / 1000;
    numberOfFrames = getU32(offset + 16);
    frameWidth = getU32(offset + 32);
    frameHeight = getU32(offset + 36);

    //skip rest of AVI header, start from end of AVI List

    //look for list with 'movi' header
    uint32_t listOffset = aviListOffset + aviListSize + 8;
    readData(listOffset, 12);
    while (compare(listOffset + 8, "movi", 4) && (lastError == AVI_NO_ERROR))
    {
        const uint32_t listSize = getU32(listOffset + 4) + 8;
        listOffset += listSize;
        readData(listOffset, 12);
    }

    if (lastError != AVI_NO_ERROR)
    {
        lastError = AVI_ERROR_MOVI_NOT_FOUND;
        return false;
    }

    //save first frame offset
    firstFrameOffset = listOffset + 8 + 4; //skip LIST and 'movi'

    //find idx
    const uint32_t listSize = getU32(listOffset + 4) + 8;
    listOffset += listSize;
    readData(listOffset, 12);
    if (!compare(listOffset, "idx1", 4) && (lastError == AVI_NO_ERROR))
    {
        indexOffset = listOffset;
    }
    else
    {
        lastError = AVI_ERROR_IDX1_NOT_FOUND;
        return false;
    }

    headerRead = true;

    return true;
}

} // namespace touchgfx
