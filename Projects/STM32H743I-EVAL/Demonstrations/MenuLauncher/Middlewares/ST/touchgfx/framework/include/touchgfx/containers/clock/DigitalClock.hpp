/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.12.3 distribution.
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef DIGITALCLOCK_HPP
#define DIGITALCLOCK_HPP

#include <touchgfx/containers/clock/AbstractClock.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/Color.hpp>

namespace touchgfx
{
/**
 * @class DigitalClock DigitalClock.hpp touchgfx/containers/clock/DigitalClock.hpp
 *
 * @brief A digital clock.
 *
 *        A digital clock. Can be set in either 12 or 24 hour mode. Seconds are optional. Width and
 *        height must be set manually to match the typography and alignment specified in the
 *        text database. The Digital Clock requires a typedText with one wildcard and uses the
 *        following characters (not including quotes)
 *        "AMP :0123456789" These must be present in the text database with the same typography
 *        as the wildcard text.
 *        Leading zero for the hour indicator can be enabled/disable by the
 *        displayLeadingZeroForHourIndicator method.
 */
class DigitalClock : public AbstractClock
{
public:

    /**
     * @enum DisplayMode
     *
     * @brief Values that represent different display modes.
     */
    enum DisplayMode
    {
        DISPLAY_12_HOUR_NO_SECONDS,     ///< 12 Hour clock. Seconds are not displayed
        DISPLAY_24_HOUR_NO_SECONDS,     ///< 24 Hour clock. Seconds are not displayed
        DISPLAY_12_HOUR,                ///< 12 Hour clock. Seconds are displayed
        DISPLAY_24_HOUR                 ///< 24 Hour clock. Seconds are displayed
    };

    /**
     * @fn DigitalClock::DigitalClock();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    DigitalClock();

    /**
     * @fn virtual DigitalClock::~DigitalClock();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~DigitalClock();

    /**
     * @fn virtual void DigitalClock::setWidth(int16_t width);
     *
     * @brief Sets the width of the DigitalClock.
     *
     *        Sets the width of the DigitalClock. The text area that displays the clock is
     *        expanded to match the dimension of the DigitalClock.
     *
     * @param width The width.
     */
    virtual void setWidth(int16_t width);

    /**
     * @fn virtual void DigitalClock::setHeight(int16_t height);
     *
     * @brief Sets the height of the DigitalClock.
     *
     *        Sets the height of the DigitalClock. The text area that displays the clock is
     *        expanded to match the dimension of the DigitalClock.
     *
     * @param height The height.
     */
    virtual void setHeight(int16_t height);

    /**
     * @fn virtual void DigitalClock::setBaselineY(int16_t baselineY);
     *
     * @brief Adjusts the digital clocks y coordinate to place the text at the specified baseline.
     *
     *        Adjusts the digital clocks y coordinate so the text will have its baseline at the
     *        specified value. The placements is relative to the specified TypedText so if this
     *        changes you have to set the baseline again.
     *
     *        Note that setTypedText must be called prior to setting the baseline.
     *
     * @param baselineY The y coordinate of the baseline.
     */
    virtual void setBaselineY(int16_t baselineY);

    /**
     * @fn virtual void DigitalClock::setTypedText(TypedText typedText);
     *
     * @brief Sets the typed text of the DigitalClock.
     *
     *        Sets the typed text of the DigitalClock. Expects a typed text with one wildcard
     *        and that the following characters are defined in the text spreadsheet (for the
     *        same typography):
     *
     *        AMP :0123456789
     *
     *        Invalidates the DigitalClock.
     *
     * @param typedText Describes the typed text to use.
     */
    virtual void setTypedText(TypedText typedText);

    /**
     * @fn virtual void DigitalClock::setColor(colortype color);
     *
     * @brief Sets the color of the text.
     *
     *        Sets the color of the text. Invalidates the DigitalClock.
     *
     * @param color The new text color.
     */
    virtual void setColor(colortype color);

    /**
     * @fn virtual void DigitalClock::setDisplayMode(DisplayMode dm)
     *
     * @brief Sets the display mode.
     *
     * @param dm The new DisplayMode.
     */
    virtual void setDisplayMode(DisplayMode dm)
    {
        displayMode = dm;
    }

    /**
     * @fn virtual DisplayMode DigitalClock::getDisplayMode() const
     *
     * @brief Gets the display mode.
     *
     * @return The display mode.
     */
    virtual DisplayMode getDisplayMode() const
    {
        return displayMode;
    }

    /**
     * @fn void DigitalClock::displayLeadingZeroForHourIndicator(bool displayLeadingZero);
     *
     * @brief Sets whether to display a leading zero for the hour indicator or not.
     *
     *        Sets whether to display a leading zero for the hour indicator or not. That is the
     *        if an hour value less than 10 will be displayed as "8:" or "08:".
     *
     *        Default value for this setting is false.
     *
     * @param displayLeadingZero true = show leading zero. false = do not show leading zero.
     */
    void displayLeadingZeroForHourIndicator(bool displayLeadingZero);

    /**
     * @fn virtual void DigitalClock::setAlpha(uint8_t alpha);
     *
     * @brief Sets the alpha value of the digital clock.
     *
     *        Sets the alpha value of the digital clock.
     *
     * @param alpha The alpha value. 255 = completely solid. 0 = invisible.
     */
    virtual void setAlpha(uint8_t alpha);

    /**
     * @fn virtual uint8_t DigitalClock::getAlpha() const;
     *
     * @brief Gets the alpha value of the digital clock.
     *
     *        Gets the alpha value of the digital clock.
     *
     * @return The alpha value. 255 = completely solid. 0 = invisible.
     */
    virtual uint8_t getAlpha() const;

    /**
     * @fn virtual uint16_t DigitalClock::getTextWidth() const
     *
     * @brief Gets text width.
     *
     * @return The text width of the current content of the digital clock.
     */
    virtual uint16_t getTextWidth() const
    {
        return text.getTextWidth();
    }

protected:
    static const int BUFFER_SIZE = 16;        ///< Buffer size of the wild card

    DisplayMode displayMode;                  ///< The current display mode
    bool useLeadingZeroForHourIndicator;      ///< Print a leading zero if the hour is less than 10

    TextAreaWithOneWildcard text;             ///< The clock text
    Unicode::UnicodeChar buffer[BUFFER_SIZE]; ///< Wild card buffer for the clock text

    virtual void updateClock();
};
}

#endif // DIGITALCLOCK_HPP
