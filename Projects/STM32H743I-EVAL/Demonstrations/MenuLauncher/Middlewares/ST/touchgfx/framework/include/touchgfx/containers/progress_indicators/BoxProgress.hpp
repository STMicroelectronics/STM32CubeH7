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

#ifndef BOXPROGRESS_HPP
#define BOXPROGRESS_HPP

#include <touchgfx/containers/progress_indicators/AbstractDirectionProgress.hpp>
#include <touchgfx/widgets/Box.hpp>

namespace touchgfx
{
/**
 * @class BoxProgress BoxProgress.hpp touchgfx/containers/progress_indicators/BoxProgress.hpp
 *
 * @brief A box progress.
 *
 *        A Box progress which shows the current progress using a simple Box. It is possible to
 *        set the color and the alpha of the box. It is also possible to control in what
 *        direction the box will progress (up, down, to the left or to the right).
 *
 * @see Box
 */
class BoxProgress : public AbstractDirectionProgress
{
public:

    /**
     * @fn BoxProgress::BoxProgress();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    BoxProgress();

    /**
     * @fn virtual BoxProgress::~BoxProgress();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~BoxProgress();

    /**
     * @fn virtual void BoxProgress::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);
     *
     * @brief Sets the position and dimension of the box progress indicator.
     *
     *        Sets the position and dimension of the box progress indicator relative to the
     *        background image.
     *
     * @param x      The x coordinate.
     * @param y      The y coordinate.
     * @param width  The width of the box progress indicator.
     * @param height The height of the box progress indicator.
     */
    virtual void setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);

    /**
     * @fn virtual void BoxProgress::setColor(colortype color);
     *
     * @brief Sets the color.
     *
     *        Sets the color of the Box.
     *
     * @param color The color.
     *
     * @see Box
     */
    virtual void setColor(colortype color);

    /**
     * @fn virtual colortype BoxProgress::getColor() const;
     *
     * @brief Gets the color.
     *
     *        Gets the color of the Box.
     *
     * @return The color.
     *
     * @see Box
     */
    virtual colortype getColor() const;

    /**
     * @fn virtual void BoxProgress::setAlpha(uint8_t alpha);
     *
     * @brief Sets the alpha.
     *
     *        Sets the alpha of the Box.
     *
     * @param alpha The alpha.
     *
     * @see Box
     */
    virtual void setAlpha(uint8_t alpha);

    /**
     * @fn virtual uint8_t BoxProgress::getAlpha() const;
     *
     * @brief Gets the alpha.
     *
     *        Gets the alpha of the Box.
     *
     * @return The alpha.
     *
     * @see Box
     */
    virtual uint8_t getAlpha() const;

    virtual void setValue(int value);

protected:
    Box box;    ///< The box
};
}

#endif // BOXPROGRESS_HPP
