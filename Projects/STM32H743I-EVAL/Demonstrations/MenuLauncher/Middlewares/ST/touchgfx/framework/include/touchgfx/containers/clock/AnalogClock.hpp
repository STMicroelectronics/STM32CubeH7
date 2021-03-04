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

#ifndef ANALOGCLOCK_HPP
#define ANALOGCLOCK_HPP

#include <touchgfx/containers/clock/AbstractClock.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextureMapper.hpp>
#include <touchgfx/widgets/AnimationTextureMapper.hpp>
#include <touchgfx/EasingEquations.hpp>

namespace touchgfx
{
/**
 * @class AnalogClock AnalogClock.hpp touchgfx/containers/clock/AnalogClock.hpp
 *
 * @brief An analog clock.
 *
 *        An analog clock. Should be supplied with images for the background, hour hand, minute
 *        hand and the optional second hand. You setup the AnalogClock by specifying the
 *        rotation point of each hand as well as the global rotation point of the clock.
 *
 *        You can customize the behavior of the AnalogClock in respect to animations and
 *        relations between the hands e.g. the hour hand moves gradually towards the next hour
 *        as the minute hand progresses (setHourHandMinuteCorrection())
 */
class AnalogClock : public AbstractClock
{
public:

    /**
     * @fn AnalogClock::AnalogClock();
     *
     * @brief Default constructor.
     */
    AnalogClock();

    /**
     * @fn virtual AnalogClock::~AnalogClock();
     *
     * @brief Destructor.
     */
    virtual ~AnalogClock();

    /**
     * @fn virtual void AnalogClock::setBackground(const BitmapId backgroundBitmapId);
     *
     * @brief Sets the background image of the clock.
     *
     *        Sets the background image of the clock. The clock rotation center is set to the
     *        background image center. The clock rotation center is the point that the clock
     *        hands rotates around.
     *
     * @param backgroundBitmapId Identifier for the background bitmap.
     */
    virtual void setBackground(const BitmapId backgroundBitmapId);

    /**
     * @fn virtual void AnalogClock::setBackground(const BitmapId backgroundBitmapId, int16_t rotationCenterX, int16_t rotationCenterY);
     *
     * @brief Sets the background image of the clock and the rotation center of the clock.
     *
     *        Sets the background image of the clock and the rotation center of the clock. The
     *        clock rotation center is the point that the clock hands rotates around.
     *
     * @param backgroundBitmapId Identifier for the background bitmap.
     * @param rotationCenterX    The rotation center x coordinate.
     * @param rotationCenterY    The rotation center y coordinate.
     */
    virtual void setBackground(const BitmapId backgroundBitmapId, int16_t rotationCenterX, int16_t rotationCenterY);

    /**
     * @fn virtual void AnalogClock::setRotationCenter(int16_t rotationCenterX, int16_t rotationCenterY);
     *
     * @brief Sets the rotation center of the clock.
     *
     *        Sets the rotation center of the clock. The clock rotation center is the point
     *        that the clock hands rotates around.
     *
     * @param rotationCenterX The rotation center x coordinate.
     * @param rotationCenterY The rotation center y coordinate.
     */
    virtual void setRotationCenter(int16_t rotationCenterX, int16_t rotationCenterY);

    /**
     * @fn virtual void AnalogClock::setupHourHand(const BitmapId hourHandBitmapId, int16_t rotationCenterX, int16_t rotationCenterY);
     *
     * @brief Sets up the hour hand.
     *
     *        Sets up the hour hand. The specified rotation center is the point of the hand
     *        that is to be placed on top of the clock rotation center. That is the point that
     *        the hand rotates around. The rotation point is relative to the supplied bitmap
     *        but can be placed outside it.
     *
     *        If not called the hour hand will just be omitted.
     *
     * @param hourHandBitmapId Identifier for the hour hand bitmap.
     * @param rotationCenterX  The hand rotation center x coordinate.
     * @param rotationCenterY  The hand rotation center y coordinate.
     */
    virtual void setupHourHand(const BitmapId hourHandBitmapId, int16_t rotationCenterX, int16_t rotationCenterY);

    /**
     * @fn virtual void AnalogClock::setupMinuteHand(const BitmapId minuteHandBitmapId, int16_t rotationCenterX, int16_t rotationCenterY);
     *
     * @brief Sets up the minute hand.
     *
     *        Sets up the minute hand. The specified rotation center is the point of the hand
     *        that is to be placed on top of the clock rotation center. That is the point that
     *        the hand rotates around. The rotation point is relative to the supplied bitmap
     *        but can be placed outside it.
     *
     *        If not called the minute hand will just be omitted.
     *
     * @param minuteHandBitmapId Identifier for the minute hand bitmap.
     * @param rotationCenterX    The hand rotation center x coordinate.
     * @param rotationCenterY    The hand rotation center y coordinate.
     */
    virtual void setupMinuteHand(const BitmapId minuteHandBitmapId, int16_t rotationCenterX, int16_t rotationCenterY);

    /**
     * @fn virtual void AnalogClock::setupSecondHand(const BitmapId secondHandBitmapId, int16_t rotationCenterX, int16_t rotationCenterY);
     *
     * @brief Sets up the second hand.
     *
     *        Sets up the second hand. The specified rotation center is the point of the hand
     *        that is to be placed on top of the clock rotation center. That is the point that
     *        the hand rotates around. The rotation point is relative to the supplied bitmap
     *        but can be placed outside it.
     *
     *        If not called the second hand will just be omitted.
     *
     * @param secondHandBitmapId Identifier for the second hand bitmap.
     * @param rotationCenterX    The hand rotation center x coordinate.
     * @param rotationCenterY    The hand rotation center y coordinate.
     */
    virtual void setupSecondHand(const BitmapId secondHandBitmapId, int16_t rotationCenterX, int16_t rotationCenterY);

    /**
     * @fn virtual void AnalogClock::setHourHandMinuteCorrection(bool active);
     *
     * @brief Sets hour hand minute correction.
     *
     *        If set to true the hour hand will be positioned between the current hour and the
     *        next depending on the minute hands position.
     *
     * @param active true to use hour hand correction.
     *
     * @see getHourHandMinuteCorrection
     */
    virtual void setHourHandMinuteCorrection(bool active);

    /**
     * @fn virtual bool AnalogClock::getHourHandMinuteCorrection() const;
     *
     * @brief Gets hour hand minute correction.
     *
     *        Gets hour hand minute correction.
     *
     * @return true if hour hand minute correction is active.
     *
     * @see setHourHandMinuteCorrection
     */
    virtual bool getHourHandMinuteCorrection() const;

    /**
     * @fn virtual void AnalogClock::setMinuteHandSecondCorrection(bool active);
     *
     * @brief Sets minute hand second correction.
     *
     *        If set to true the minute hand will be positioned between the current minute and
     *        the next depending on the second hands position.
     *
     * @param active true to use.
     *
     * @see setMinuteHandSecondCorrection
     */
    virtual void setMinuteHandSecondCorrection(bool active);

    /**
     * @fn virtual bool AnalogClock::getMinuteHandSecondCorrection() const;
     *
     * @brief Gets minute hand second correction.
     *
     *        Gets minute hand second correction.
     *
     * @return true if minute hand second correction is active.
     *
     * @see setHourHandMinuteCorrection
     */
    virtual bool getMinuteHandSecondCorrection() const;

    /**
     * @fn virtual void AnalogClock::setAnimation(uint16_t duration = 10, EasingEquation animationProgressionEquation = EasingEquations::backEaseInOut);
     *
     * @brief Setup the clock to use animation for hand movements.
     *
     * @param duration                     (Optional) The animation duration.
     * @param animationProgressionEquation (Optional) The animation progression equation.
     */
    virtual void setAnimation(uint16_t duration = 10, EasingEquation animationProgressionEquation = EasingEquations::backEaseInOut);

    /**
     * @fn virtual uint16_t AnalogClock::getAnimationDuration()
     *
     * @brief Gets the animation duration.
     *
     * @return The animation duration.
     */
    virtual uint16_t getAnimationDuration()
    {
        return animationDuration;
    }

    /**
     * @fn virtual void AnalogClock::initializeTime24Hour(uint8_t hour, uint8_t minute, uint8_t second);
     *
     * @brief Sets the time with input format as 24H. No animations are performed.
     *
     *        Sets the time with input format as 24H. No animations are performed regardless of
     *        the animation settings. This is often useful when setting up the AnalogClock
     *        where you do not want an initial animation. Note that this does not affect any
     *        selected presentation formats.
     *
     * @param hour   The hours (in 24H format).
     * @param minute The minutes (in 24H format).
     * @param second The seconds (in 24H format).
     */
    virtual void initializeTime24Hour(uint8_t hour, uint8_t minute, uint8_t second);

    /**
     * @fn virtual void AnalogClock::initializeTime12Hour(uint8_t hour, uint8_t minute, uint8_t second, bool am);
     *
     * @brief Sets the time with input format as 12H. No animations are performed.
     *
     *        Sets the time with input format as 12H. No animations are performed regardless of
     *        the animation settings. This is often useful when setting up the AnalogClock
     *        where you do not want an initial animation. Note that this does not affect any
     *        selected presentation formats.
     *
     * @param hour   The hours (in 12H format).
     * @param minute The minutes (in 12H format).
     * @param second The seconds (in 12H format).
     * @param am     AM/PM setting. True = AM, false = PM.
     */
    virtual void initializeTime12Hour(uint8_t hour, uint8_t minute, uint8_t second, bool am);

protected:
    Image background;   ///< The background image of the AnalogClock

    AnimationTextureMapper hourHand;    ///< TextureMapper that represents the hourHand
    AnimationTextureMapper minuteHand;  ///< TextureMapper that represents the minuteHand
    AnimationTextureMapper secondHand;  ///< TextureMapper that represents the secondHand

    EasingEquation animationEquation;   ///< The easing equation used by hand animations
    uint16_t animationDuration;         ///< The duration of hand animations. If 0 animations are disabled

    int16_t clockRotationCenterX;       ///< The rotation point (X) of the hands
    int16_t clockRotationCenterY;       ///< The rotation point (Y) of the hands

    uint8_t lastHour;                   ///< The last know hour value
    uint8_t lastMinute;                 ///< The last know minute value
    uint8_t lastSecond;                 ///< The last know second value

    bool hourHandMinuteCorrectionActive;    ///< Is hour hand minute correction active
    bool minuteHandSecondCorrectionActive;  ///< Is minute hand second correction active

    virtual void updateClock();

    /**
     * @fn virtual void AnalogClock::setupHand(TextureMapper& hand, const BitmapId bitmapId, int16_t rotationCenterX, int16_t rotationCenterY);
     *
     * @brief Sets up a given the hand.
     *
     *        Sets up a given the hand.
     *
     * @param [in,out] hand   Reference to the hand being setup.
     * @param bitmapId        The bitmap identifier for the given hand.
     * @param rotationCenterX The hand rotation center x coordinate.
     * @param rotationCenterY The hand rotation center y coordinate.
     */
    virtual void setupHand(TextureMapper& hand, const BitmapId bitmapId, int16_t rotationCenterX, int16_t rotationCenterY);

    /**
     * @fn virtual float AnalogClock::convertHandValueToAngle(uint8_t steps, uint8_t handValue, uint8_t secondHandValue = 0) const;
     *
     * @brief Convert hand value to angle.
     *
     *        Convert hand value to angle.
     *
     * @param steps           Number of steps the primary hand value is divided into (e.g. 60 for
     *                        minutes/seconds and 12 for hour).
     * @param handValue       The actual value for the hand in question (in the range [0;steps]).
     * @param secondHandValue (Optional) If the angle should be corrected for a secondary hand its
     *                        value should be specified here (in the range [0;60]). This is the
     *                        case when setHourHandMinuteCorrection(true) or
     *                        setMinuteHandSecondCorrection(true) is selected.
     *
     * @return The converted value to angle.
     */
    virtual float convertHandValueToAngle(uint8_t steps, uint8_t handValue, uint8_t secondHandValue = 0) const;

    /**
     * @fn virtual bool AnalogClock::animationEnabled() const;
     *
     * @brief Is animation enabled.
     *
     *        Is animation enabled.
     *
     * @return true if animation is enabled.
     */
    virtual bool animationEnabled() const;
};
}

#endif // ANALOGCLOCK_HPP
