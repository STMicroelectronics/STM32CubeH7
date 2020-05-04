#ifndef MJPEGREADER_HPP
#define MJPEGREADER_HPP

#include <touchgfx/widgets/PixelDataWidget.hpp>
#include <gui/video_player_screen/FileInput.hpp>

namespace touchgfx
{
/**
* @class MJPEGREADER
*
*/
class MJPEGReader
{
public:
    MJPEGReader();

    //buffers
    void setOutputWidget(PixelDataWidget& widget);
    void setFrameBuffer(uint8_t* const buffer);
    void setTemporaryFrameBuffer(uint8_t* const buffer);
    void setLineBuffer(uint8_t* const buffer);
    void setFileBuffer(uint8_t* const buffer, uint32_t size);

    //set file, reads header
    bool setAVIFile(fileinput::FileHdl fileHandle);
    bool closeFile();

    //playback control
    void setTimeBetweenTicks(uint32_t ms);
    void play();
    void pause();
    void setRepeat(bool repeat);
    void gotoFrame(uint32_t frameNumber);
    void showFirstFrame();
    void setHWDecodingEnabled(bool enabled);
    void tick();

    bool isPlaying()
    {
        return playing;
    };
    bool isEOF() { return (headerRead && (frameNumber >= numberOfFrames)); }
    bool isHWDecodingEnabled() { return useHWDecoding; }
    uint32_t getFrameNumber()
    {
        return frameNumber;
    };
    uint32_t getFPS()
    {
        return fps;
    };
    uint32_t getNumberOfFrames()
    {
        return numberOfFrames;
    };
    uint32_t getMsPerFrame()
    {
        return msPerFrame;
    };

    //getMovieThumbNail to bitmap
    bool decodeThumbNail(Bitmap bmp, uint32_t frameNumber);

    enum AVIErrors
    {
        AVI_NO_ERROR,
        AVI_NO_BUFFERS,
        AVI_NO_FILE,
        AVI_ERROR_NOT_RIFF,
        AVI_ERROR_AVI_HEADER_NOT_FOUND,
        AVI_ERROR_AVI_LIST_NOT_FOUND,
        AVI_ERROR_AVI_HDRL_NOT_FOUND,
        AVI_ERROR_AVI_AVIH_NOT_FOUND,
        AVI_ERROR_AVI_HEADER_TO_SHORT, //not full header provided
        AVI_ERROR_FILE_BUFFER_TO_SMALL,
        AVI_ERROR_MOVI_NOT_FOUND,
        AVI_ERROR_IDX1_NOT_FOUND,
        AVI_ERROR_FRAMENO_TO_HIGH,
        AVI_ERROR_EOF_REACHED
    };
    void setMovieEndedAction(GenericCallback<>& action) { movieEndedAction = &action; }
private:
    /**
     * @fn uint32_t getU32(const uint8_t* d);
     *
     * @brief Read 32 bit little endian unsigned value
     */
    uint32_t getU32(const uint32_t offset);

    /**
     * @fn uint32_t getU16(const uint8_t* d);
     *
     * @brief Read 16 bit little endian unsigned value
     */
    uint32_t getU16(const uint32_t offset);

    uint32_t compare(const uint32_t offset, const char* str, uint32_t num);

    /**
     *
     * @brief Decode one frame to buffer
     * @arg mode 0 decode in software, 1 attempt decoding in hardware then fallback to software
     */
    bool decodeFrame(uint32_t offset, uint8_t* const buffer, Bitmap::BitmapFormat format, int mode = 1);

    bool showNextFrame(bool show = true);
    void skipFrame();

    void readData(uint32_t offset, uint32_t length);

    inline uint32_t getFrameNoForMs(uint32_t ms)
    {
        return ms / msPerFrame;
    };
    GenericCallback<>* movieEndedAction;
    //Buffer pointers
    PixelDataWidget* widget;
    uint8_t* frameBuffer;
    uint8_t* temporaryFrameBuffer;
    uint8_t* lineBuffer;
    uint8_t* fileBuffer;
    uint32_t bufferSize;
    //Buffer management
    uint32_t bufferStartOffset;
    uint32_t dataLength;
    //input
    fileinput::FileHdl aviFile;
    AVIErrors lastError;
    //Output management
    uint32_t fps;
    uint32_t frameCounter;
    uint32_t frameTick16frame; //ticks for 16 frames
    uint32_t frameTickLost;
    uint32_t msBetweenTicks;
    uint32_t msTime;
    uint32_t frameNumber;
    uint32_t outputX, outputY;
    uint32_t outputWidth;
    uint32_t outputHeight;
    Rect outputRect;
    bool playing;
    bool repeating;

    //Header data
    bool headerRead;

    uint32_t msPerFrame;
    uint32_t numberOfFrames;
    uint16_t frameWidth;
    uint16_t frameHeight;
    uint32_t firstFrameOffset;
    uint32_t indexOffset;
    uint32_t playOffset;

    bool useHWDecoding;
    struct RGB
    {
        uint8_t R;
        uint8_t G;
        uint8_t B;
    };
};

} // namespace touchgfx

#endif
