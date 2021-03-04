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

#ifndef DISPLAYTRANSFORMATION_HPP
#define DISPLAYTRANSFORMATION_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @class DisplayTransformation DisplayTransformation.hpp touchgfx/transforms/DisplayTransformation.hpp
 *
 * @brief Defines transformations from display space to frame buffer space.
 *
 *        Defines transformations from display space to frame buffer space. The display might
 *        be (considered) in portrait mode from 0,0 to 272,480, while the actual frame buffer
 *        is from 0,0 to 480,272. This class handles the transformations.
 */
class DisplayTransformation
{
public:

    /**
     * @fn static void DisplayTransformation::transformDisplayToFrameBuffer(int16_t& x, int16_t& y);
     *
     * @brief Transform x,y from display to frame buffer coordinates.
     *
     *        Transform x,y from display to frame buffer coordinates.
     *
     * @param [in,out] x the x part to translate.
     * @param [in,out] y the y part to translate.
     */
    static void transformDisplayToFrameBuffer(int16_t& x, int16_t& y);

    /**
     * @fn static void DisplayTransformation::transformDisplayToFrameBuffer(float& x, float& y);
     *
     * @brief Transform x,y from display to frame buffer coordinates.
     *
     *        Transform x,y from display to frame buffer coordinates.
     *
     * @param [in,out] x the x part to translate.
     * @param [in,out] y the y part to translate.
     */
    static void transformDisplayToFrameBuffer(float& x, float& y);

    /**
     * @fn static void DisplayTransformation::transformFrameBufferToDisplay(int16_t& x, int16_t& y);
     *
     * @brief Transform x,y from frame buffer to display coordinates.
     *
     *        Transform x,y from frame buffer to display coordinates.
     *
     * @param [in,out] x the x part to translate.
     * @param [in,out] y the y part to translate.
     */
    static void transformFrameBufferToDisplay(int16_t& x, int16_t& y);

    /**
     * @fn static void DisplayTransformation::transformDisplayToFrameBuffer(int16_t& x, int16_t& y, const Rect& in);
     *
     * @brief Transform x,y from coordinates relative to the in rect to frame buffer coordinates.
     *
     *        Transform x,y from coordinates relative to the in rect to frame buffer
     *        coordinates.
     *
     * @param [in,out] x the x part to translate.
     * @param [in,out] y the y part to translate.
     * @param in         the rectangle defining the coordinate space.
     */
    static void transformDisplayToFrameBuffer(int16_t& x, int16_t& y, const Rect& in);

    /**
     * @fn static void DisplayTransformation::transformDisplayToFrameBuffer(float& x, float& y, const Rect& in);
     *
     * @brief Transform x,y from coordinates relative to the in rect to frame buffer coordinates.
     *
     *        Transform x,y from coordinates relative to the in rect to frame buffer
     *        coordinates.
     *
     * @param [in,out] x the x part to translate.
     * @param [in,out] y the y part to translate.
     * @param in         the rectangle defining the coordinate space.
     */
    static void transformDisplayToFrameBuffer(float& x, float& y, const Rect& in);

    /**
     * @fn static void DisplayTransformation::transformDisplayToFrameBuffer(Rect& r);
     *
     * @brief Transform rectangle from display to frame buffer coordinates.
     *
     *        Transform rectangle from display to frame buffer coordinates.
     *
     * @param [in,out] r the rectangle to translate.
     */
    static void transformDisplayToFrameBuffer(Rect& r);

    /**
     * @fn static void DisplayTransformation::transformDisplayToFrameBuffer(Rect& r, const Rect& in);
     *
     * @brief Transform rectangle r from coordinates relative to the in rect to frame buffer
     *        coordinates.
     *
     *        Transform rectangle r from coordinates relative to the in rect to frame buffer
     *        coordinates.
     *
     * @param [in,out] r the rectangle to translate.
     * @param in         the rectangle defining the coordinate space.
     */
    static void transformDisplayToFrameBuffer(Rect& r, const Rect& in);
};
} // namespace touchgfx

#endif // DISPLAYTRANSFORMATION_HPP
