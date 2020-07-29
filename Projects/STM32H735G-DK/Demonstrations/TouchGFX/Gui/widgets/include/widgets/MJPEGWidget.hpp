#ifndef MJPEG_WIDGET_HPP
#define MJPEG_WIDGET_HPP

#include <touchgfx/widgets/PixelDataWidget.hpp>
#include <widgets/FileInput.hpp>

namespace touchgfx
{
/**
* @class MJPEGWidget
*
*        @sa touchgfx::Widget
*/
class MJPEGWidget : public PixelDataWidget
{
public:
    /**
     * @fn MJPEGWidget::MJPEGWidget();
     *
     * @brief Default constructor
     */
    MJPEGWidget();

    /**
     * @fn void setLineBuffer(uint8_t* buffer);
     *
     * @brief Set pointer to line buffer used during decompression.
     */
    void setLineBuffer(uint8_t* buffer);

    /**
     * @fn void setTimeBetweenTicks(uint32_t ms);
     *
     * @brief Set time between ticks. Used to replay video in correct speed.
     */
    void setTimeBetweenTicks(uint32_t ms);

    /**
     * @fn void setMovie(const uint8_t* movie, const uint32_t length);
     *
     * @brief Set movie data to replay
     */
    void setMovie(const uint8_t* movie, const uint32_t length);

    /**
     * @fn void tick();
     *
     *
     * @brief Advance to next frame according to framerate
     */
    bool tick();

    void gotoFrame(uint32_t frameno);

    uint16_t getFrameNo()
    {
        return frameno;
    }
private:

    /**
     * @fn void decodeMJPEGFrame(const uint8_t* const data, const uint32_t length);
     *
     * @brief Read one MJPEG frame from pointer.
     */
    void decodeMJPEGFrame(const uint8_t* const data, const uint32_t length);

    /**
     * @fn uint32_t getU32(const uint8_t* d);
     *
     * @brief Read 32 bit little endian unsigned value
     */
    uint32_t getU32(const uint8_t* d);

    /**
     * @fn uint32_t getU16(const uint8_t* d);
     *
     * @brief Read 16 bit little endian unsigned value
     */
    uint32_t getU16(const uint8_t* d);

    uint32_t frameno;
    uint32_t offset;
    uint32_t microSecPerFrame;
    uint32_t firstFrameOffset;
    uint32_t lastFrameEnd;
    uint32_t timeBetweenTicks;
    uint32_t movieLength;
    const uint8_t* movieData;
    uint8_t* lineBuffer;
    struct RGB
    {
        uint8_t R;
        uint8_t G;
        uint8_t B;
    };
};

} // namespace touchgfx
#endif
