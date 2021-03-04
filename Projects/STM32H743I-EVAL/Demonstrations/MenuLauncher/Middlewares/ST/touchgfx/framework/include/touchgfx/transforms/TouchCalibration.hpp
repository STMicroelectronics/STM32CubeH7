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

#ifndef TOUCHCALIBRATION_HPP
#define TOUCHCALIBRATION_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @class TouchCalibration TouchCalibration.hpp touchgfx/transforms/TouchCalibration.hpp
 *
 * @brief Calibrates a touch coordinate.
 *
 *        Class TouchCalibraiton is responsible for translating coordinates (Point) based on
 *        matrix of calibration values.
 */
class TouchCalibration
{
public:

    /**
     * @fn static void TouchCalibration::setCalibrationMatrix(const Point* ref, const Point* scr);
     *
     * @brief Initializes the calibration matrix based on reference and measured values.
     *
     *        Initializes the calibration matrix based on reference and measured values.
     *
     * @param ref Pointer to array of three reference points.
     * @param scr Pointer to array of three measured points.
     */
    static void setCalibrationMatrix(const Point* ref, const Point* scr);

    /**
     * @fn static void TouchCalibration::translatePoint(Point& p);
     *
     * @brief Translates the specified point using the matrix.
     *
     *        Translates the specified point using the matrix. If matrix has not been
     *        initialized, p is not modified.
     *
     * @param [in,out] p The point to translate.
     */
    static void translatePoint(Point& p);

private:

    /**
     * @fn static int32_t TouchCalibration::muldiv(int32_t factor1, int32_t mls, int32_t mls_result, int32_t mls_remainder, int32_t factor2, int32_t divisor, int32_t& remainder);
     *
     * @brief factor2/divisor without causing overflow.
     *
     *        factor2/divisor without causing overflow.
     *
     * @param factor1            Is the first factor.
     * @param mls                Is the max left shift possible of abs(factor1) without
     *                           overflowing.
     * @param mls_result         is (factor1 &lt;&lt; mls) / divisor.
     * @param mls_remainder      is (factor1 &lt;&lt; mls) % divisor.
     * @param factor2            Is the second factor.
     * @param divisor            Is the divisor.
     * @param [in,out] remainder Is added with the remainder from the division.
     *
     * @return the integer part of the calculation.
     */
    static int32_t muldiv(int32_t factor1, int32_t mls, int32_t mls_result, int32_t mls_remainder, int32_t factor2, int32_t divisor, int32_t& remainder);

    /**
     * @fn static int32_t TouchCalibration::clz(int32_t x);
     *
     * @brief Count leading zero bits (without causing a change of sign).
     *
     *        Count leading zero bits (without causing a change of sign).
     *
     * @param x The x coordinate.
     *
     * @return the number of leading zeros.
     */
    static int32_t clz(int32_t x);

    /**
     * @struct Matrix TouchCalibration.hpp touchgfx/transforms/TouchCalibration.hpp
     *
     * @brief Matrix struct used in the calibration of coordinates returned by touch controllers.
     */
    struct Matrix
    {
        //@{
        /** Variable named according to <a href="http://www.embedded.com/design/system-integration/4023968/How-To-Calibrate-Touch-Screens">calibration technique by Carlos E. Vidales </a> */
        int32_t An, Bn, Cn, Dn, En, Fn, Divider;
        /** Internal helper values to speed up calculation */
        int32_t mlsAn, mlsBn, mlsDn, mlsEn;
        int32_t resAn, resBn, resDn, resEn;
        int32_t remAn, remBn, remDn, remEn;
        //@}
    };

    /**
     * @typedef struct Matrix Matrix
     *
     * @brief Defines an alias representing the matrix.
     */
    typedef struct Matrix Matrix;

    static Matrix matrix; ///< The matrix
};
} // namespace touchgfx
#endif // TOUCHCALIBRATION_HPP
