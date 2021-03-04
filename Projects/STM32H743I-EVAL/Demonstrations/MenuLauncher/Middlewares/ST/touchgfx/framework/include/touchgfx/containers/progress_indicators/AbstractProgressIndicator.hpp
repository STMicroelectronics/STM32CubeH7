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

#ifndef ABSTRACTPROGRESSINDICATOR_HPP
#define ABSTRACTPROGRESSINDICATOR_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>

namespace touchgfx
{
/**
 * @class AbstractProgressIndicator AbstractProgressIndicator.hpp touchgfx/containers/progress_indicators/AbstractProgressIndicator.hpp
 *
 * @brief An abstract progress indicator.
 *
 *        The AbstractProgressIndicator declares methods that provides the basic mechanisms and
 *        tools to implement a progress indicator. For more specific implementations see
 *        classes that inherit from AbstractProgressIndicator.
 *
 * @see BoxProgess
 * @see CircleProgress
 * @see ImageProgress
 * @see LineProgress
 * @see TextProgress
 */
class AbstractProgressIndicator : public Container
{
public:

    /**
     * @fn AbstractProgressIndicator::AbstractProgressIndicator();
     *
     * @brief Default constructor.
     *
     *        Initialized the progress indicator with default range 0-100.
     *
     * @see setRange
     */
    AbstractProgressIndicator();

    /**
     * @fn virtual AbstractProgressIndicator::~AbstractProgressIndicator();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~AbstractProgressIndicator();

    /**
     * @fn virtual void AbstractProgressIndicator::setBackground(const Bitmap& bmpBackground);
     *
     * @brief Sets the background image.
     *
     *        Sets the background image. The width and height of the widget is updated
     *        according to the dimension of the image.
     *
     * @param bmpBackground The background bitmap.
     */
    virtual void setBackground(const Bitmap& bmpBackground);

    /**
     * @fn virtual void AbstractProgressIndicator::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);
     *
     * @brief Sets the position and dimension of the actual progress indicator.
     *
     *        Sets the position and dimension of the actual progress indicator relative to the
     *        background image.
     *
     * @param x      The x coordinate.
     * @param y      The y coordinate.
     * @param width  The width of the box progress indicator.
     * @param height The height of the box progress indicator.
     */
    virtual void setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);

    /**
     * @fn virtual int16_t AbstractProgressIndicator::getProgressIndicatorX() const;
     *
     * @brief Gets progress indicator x coordinate.
     *
     *        Gets progress indicator x coordinate.
     *
     * @return The progress indicator x coordinate.
     */
    virtual int16_t getProgressIndicatorX() const;

    /**
     * @fn virtual int16_t AbstractProgressIndicator::getProgressIndicatorY() const;
     *
     * @brief Gets progress indicator y coordinate.
     *
     *        Gets progress indicator y coordinate.
     *
     * @return The progress indicator y coordinate.
     */
    virtual int16_t getProgressIndicatorY() const;

    /**
     * @fn virtual int16_t AbstractProgressIndicator::getProgressIndicatorWidth() const;
     *
     * @brief Gets progress indicator width.
     *
     *        Gets progress indicator width.
     *
     * @return The progress indicator width.
     */
    virtual int16_t getProgressIndicatorWidth() const;

    /**
     * @fn virtual int16_t AbstractProgressIndicator::getProgressIndicatorHeight() const;
     *
     * @brief Gets progress indicator height.
     *
     *        Gets progress indicator height.
     *
     * @return The progress indicator height.
     */
    virtual int16_t getProgressIndicatorHeight() const;

    /**
     * @fn virtual void AbstractProgressIndicator::setRange(int16_t min, int16_t max, uint16_t steps = 0, uint16_t minStep = 0);
     *
     * @brief Sets the range for the progress indicator.
     *
     *        Sets the range for the progress indicator. The range is the values that are given
     *        to the progress indicator while progressing through the task at hand. If an app
     *        needs to work through 237 items to finish a task, the range should be set to (0,
     *        237) assuming that 0 items is the minimum. Though the minimum is often 0, it is
     *        possible to customize this.
     *
     *        The steps parameter, is used to specify at what granularity you want the progress
     *        indicator to report a new progress value. If the 237 items to be reported as 0%,
     *        10%, 20%, ... 100%, the steps should be set to 10 as there are ten steps from 0%
     *        to 100%. If you want to update a widget which is 150 pixels wide, you might want
     *        to set steps to 150 to get a new progress value for every pixel. If you are
     *        updating a clock and want this to resemble an analog clock, you might want to use
     *        12 or perhaps 60 as number of steps.
     *
     *        The minStep parameter is used when the value min should give a progress different
     *        from 0. For example, if progress is a clock face, you want to count from 0..1000
     *        and you want progress per minute, but want to make sure that 0 is not a blank
     *        clock face, but instead you want 1 minute to show, the setRange(0, 1000, 60, 1)
     *        will make sure that as values progress from 0 to 1000, getProgress() start from 1
     *        and goes up to 60. Another example could be a BoxProgress with a TextProgress on
     *        top and you want to make sure that "0%" will always show in the box, use
     *        something like setRange(0, 1000, 200, 40) if your box is 200 wide and "0%" is 40
     *        wide.
     *
     * @param min     The minimum input value.
     * @param max     The maximum input value.
     * @param steps   The steps in which to report progress.
     * @param minStep The step which the minimum input value is mapped to.
     *
     * @see setValue, getProgress
     */
    virtual void setRange(int16_t min, int16_t max, uint16_t steps = 0, uint16_t minStep = 0);

    /**
     * @fn virtual void AbstractProgressIndicator::getRange(int16_t& min, int16_t& max, uint16_t& steps, uint16_t& minStep) const;
     *
     * @brief Gets the range.
     *
     *        Gets the range set by setRange().
     *
     * @param [out] min     The minimum input value.
     * @param [out] max     The maximum input value.
     * @param [out] steps   The steps in which to report progress.
     * @param [out] minStep The step which the minimum input value is mapped to.
     */
    virtual void getRange(int16_t& min, int16_t& max, uint16_t& steps, uint16_t& minStep) const;

    /**
     * @fn virtual void AbstractProgressIndicator::getRange(int16_t& min, int16_t& max, uint16_t& steps) const;
     *
     * @brief Gets the range.
     *
     *        Gets the range set by setRange().
     *
     * @param [out] min   The minimum input value.
     * @param [out] max   The maximum input value.
     * @param [out] steps The steps in which to report progress.
     */
    virtual void getRange(int16_t& min, int16_t& max, uint16_t& steps) const;

    /**
     * @fn virtual void AbstractProgressIndicator::getRange(int16_t& min, int16_t& max) const;
     *
     * @brief Gets the range.
     *
     *        Gets the range set by setRange().
     *
     * @param [out] min The minimum input value.
     * @param [out] max The maximum input value.
     */
    virtual void getRange(int16_t& min, int16_t& max) const;

    /**
     * @fn virtual void AbstractProgressIndicator::setValue(const int value);
     *
     * @brief Sets a value.
     *
     *        Sets the current value in the range (min..max) set by setRange(). Values lower
     *        than min are mapped to min, values higher than max are mapped to max.
     *
     * @param value The value.
     */
    virtual void setValue(int value);

    /**
     * @fn virtual int AbstractProgressIndicator::getValue() const;
     *
     * @brief Gets the value.
     *
     *        Gets the current value set by setValue().
     *
     * @return The value.
     */
    virtual int getValue() const;

    /**
     * @fn virtual uint16_t AbstractProgressIndicator::getProgress(const uint16_t range = 100) const;
     *
     * @brief Gets the progress.
     *
     *        Gets the current progress based on the range set by setRange() and the value set by setValue().
     *
     * @param range The range.
     *
     * @return The progress.
     * @see setRange, setValue
     */
    virtual uint16_t getProgress(uint16_t range = 100) const;

protected:
    Image background;                     ///< The background image
    Container progressIndicatorContainer; ///< The container that holds the actual progress indicator
    int16_t rangeMin;                     ///< The range minimum
    int16_t rangeMax;                     ///< The range maximum
    uint16_t currentValue;                ///< The current value
    uint16_t rangeSteps;                  ///< The range steps
    uint16_t rangeStepsMin;               ///< The range steps minimum
};
}

#endif // ABSTRACTPROGRESSINDICATOR_HPP
