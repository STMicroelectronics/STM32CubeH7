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

#ifndef CWRUTIL_HPP
#define CWRUTIL_HPP

#include <touchgfx/canvas_widget_renderer/Rasterizer.hpp>

namespace touchgfx
{
/**
 * @struct CWRUtil CWRUtil.hpp touchgfx/widgets/canvas/CWRUtil.hpp
 *
 * @brief Helper classes and functions for CanvasWidget.
 *
 *        Helper classes and functions for CanvasWidget. A handful of utility functions can be
 *        found here. These include helper functions for converting between float, int and
 *        Q5/Q10/Q15 format. There are also functions for calculating sin() and cos() in
 *        integers with a high number of bits reserved for fraction. Having sin() and cos() pre-
 *        calculated in this way allows very fast drawing of circles without the need for
 *        floating point arithmetic.
 *
 *        Using Q5, numbers from -1024.00000 to +1024.96875 with a precision of 1/32 = 0.03125
 *        can be represented.
 *
 * @see http://en.wikipedia.org/wiki/Q_%28number_format%29
 * @see Widget
 */

struct CWRUtil
{
    class Q10;
    class Q15;

    /**
     * @class Q5 CWRUtil.hpp touchgfx/widgets/canvas/CWRUtil.hpp
     *
     * @brief Defines a number with 5 bits reserved for fraction.
     *
     *        Defines a number with 5 bits reserved for the fractional part of the decimal
     *        number. Q5 implements some simple arithmetic operations, most yielding a Q5
     *        number and some yielding a Q10 number as a result. Other operations also work
     *        with Q15 numbers.
     *
     * @see Q10
     * @see Q15
     * @see http://en.wikipedia.org/wiki/Q_%28number_format%29
     * @see http://en.wikipedia.org/wiki/Fixed-point_arithmetic
     */
    class Q5
    {
    public:

        /**
         * @fn Q5::Q5()
         *
         * @brief Default constructor.
         */
        Q5() : v(0) { }

        /**
         * @fn explicit Q5::Q5(int i)
         *
         * @brief Constructor from integer.
         *
         *        Constructor from integer. No conversion is done - the integer is assumed to
         *        already be in Q5 format.
         *
         * @param i Integer pre-formattet in Q5 format.
         */
        explicit Q5(int i) : v(i) { }

        /**
         * @fn Q5::Q5(const Q10 q10) : v(int(q10) / Rasterizer::POLY_BASE_SIZE)
         *
         * @brief Constructor from Q10.
         *
         *        Constructor from Q10. The Q10 is shifted down to convert it to Q5.
         *
         * @param q10 The Q10 value to convert to a Q5 value.
         *
         * @see Q10
         */
        Q5(const Q10 q10) : v(int(q10) / Rasterizer::POLY_BASE_SIZE) { }

        /**
         * @fn operator int() const
         *
         * @brief Gets the Q5 as an integer without conversion.
         *
         *        Gets the Q5 as an integer without conversion.
         *
         * @return The unconverted Q5 value.
         */
        operator int() const
        {
            return v;
        }

        /**
         * @fn Q5 Q5::operator-() const
         *
         * @brief Negation operator.
         *
         *        Negation operator.
         *
         * @return The negative value of this.
         */
        Q5 operator-() const
        {
            return Q5(-v);
        }

        /**
         * @fn Q5 Q5::operator+(const Q5& q5) const
         *
         * @brief Addition operator.
         *
         *        Addition operator.
         *
         * @param q5 The Q5 to add to this.
         *
         * @return The result of the operation.
         */
        Q5 operator+(const Q5& q5) const
        {
            return Q5(v + q5.v);
        }

        /**
         * @fn Q5 Q5::operator-(const Q5& q5) const
         *
         * @brief Subtraction operator.
         *
         *        Subtraction operator.
         *
         * @param q5 The Q5 to subtract from this.
         *
         * @return The result of the operation.
         */
        Q5 operator-(const Q5& q5) const
        {
            return Q5(v - q5.v);
        }

        /**
         * @fn Q10 Q5::operator*(const Q5& q5) const
         *
         * @brief Multiplication operator.
         *
         *        Multiplication operator. The result is a Q10, not a Q5, for increased
         *        precision.
         *
         * @param q5 The Q5 to multiply this with.
         *
         * @return The result of the operation.
         *
         * @see Q10
         */
        Q10 operator*(const Q5& q5) const
        {
            return Q10(v * q5.v);
        }

        /**
         * @fn Q5 Q5::operator*(const Q15& q15) const
         *
         * @brief Multiplication operator.
         *
         *        Multiplication operator. Often used in relation with sine and cosine
         *        calculation which are pre-calculated as Q15. As the result is needed as a Q5,
         *        this operator multiplies with the given Q15 and converts the result to a Q5.
         *
         * @param q15 The Q15 to multiply this with.
         *
         * @return The result of the operation.
         *
         * @see Q15
         */
        Q5 operator*(const Q15& q15) const
        {
            int32_t remainder;
            return Q5(muldiv(v, int(q15), Rasterizer::POLY_BASE_SIZE * Rasterizer::POLY_BASE_SIZE * Rasterizer::POLY_BASE_SIZE, remainder));
        }

        /**
         * @fn Q5 Q5::operator*(const int i) const
         *
         * @brief Multiplication operator.
         *
         *        Multiplication operator.
         *
         * @param i The integer to multiply this with.
         *
         * @return The result of the operation.
         */
        Q5 operator*(const int i) const
        {
            return Q5(v * i);
        }

        /**
         * @fn Q5 Q5::operator/(const int i) const
         *
         * @brief Division operator.
         *
         *        Division operator.
         *
         * @param i The integer to divide this by.
         *
         * @return The result of the operation.
         */
        Q5 operator/(const int i) const
        {
            return Q5(v / i);
        }

        /**
         * @fn Q5 Q5::operator/(const Q5 q5) const
         *
         * @brief Division operator.
         *
         *        Division operator. Internally this Q5 is converted to Q10 before the division
         *        to increased precision.
         *
         * @param q5 The Q5 to divide this by.
         *
         * @return The result of the operation.
         *
         * @see Q10
         */
        Q5 operator/(const Q5 q5) const
        {
            return Q5(int(v) * Rasterizer::POLY_BASE_SIZE / q5.v);
        }

        /**
         * @fn template <typename T> T Q5::to() const
         *
         * @brief Converts the Q5 value to an int or a float.
         *
         *        Convert the Q5 value to an integer by removing the 5 bits used for the
         *        fraction, or to a floating point value by dividing by 32, depending on the
         *        type specified as T.
         *
         * @tparam T Either int or float.
         *
         * @return Q5 value as a type T.
         */
        template <typename T>
        T to() const
        {
            return (T)v / (T)Rasterizer::POLY_BASE_SIZE;
        }

    private:
        int32_t v;
    };

    /**
     * @class Q10 CWRUtil.hpp touchgfx/widgets/canvas/CWRUtil.hpp
     *
     * @brief Defines a number with 10 bits reserved for fraction.
     *
     *        Defines a number with 10 bits reserved for the fractional part of the decimal
     *        number. Q10 implements some simple arithmetic operations, most yielding a Q10
     *        number and some yielding a Q5 number or a Q15 number as a result.
     *
     *        Q5*Q5=Q10, Q10/Q5=Q5, ...
     *
     * @see Q5
     * @see Q15
     * @see http://en.wikipedia.org/wiki/Q_%28number_format%29
     * @see http://en.wikipedia.org/wiki/Fixed-point_arithmetic
     */
    class Q10
    {
    public:

        /**
         * @fn Q10::Q10()
         *
         * @brief Default constructor.
         *
         *        Default constructor.
         */
        Q10() : v(0) { }

        /**
         * @fn explicit Q10::Q10(int i)
         *
         * @brief Constructor from integer.
         *
         *        Constructor from integer. No conversion is done - the integer is assumed to
         *        already be in Q10 format.
         *
         * @param i int pre-formattet in Q10 format.
         */
        explicit Q10(int i) : v(i) { }

        /**
         * @fn operator int() const
         *
         * @brief Gets the Q10 as an integer without conversion.
         *
         *        Gets the Q10 as an integer without conversion.
         *
         * @return The unconverted Q10 value.
         */
        operator int() const
        {
            return v;
        }

        /**
         * @fn Q10 Q10::operator-() const
         *
         * @brief Negation operator.
         *
         *        Negation operator.
         *
         * @return The negative value of this.
         */
        Q10 operator-() const
        {
            return Q10(-v);
        }

        /**
         * @fn Q10 Q10::operator+(const Q10& q10) const
         *
         * @brief Addition operator.
         *
         *        Addition operator.
         *
         * @param q10 The Q10 to add to this.
         *
         * @return The result of the operation.
         */
        Q10 operator+(const Q10& q10) const
        {
            return Q10(v + q10.v);
        }

        /**
         * @fn Q15 Q10::operator*(const Q5& q5) const
         *
         * @brief Multiplication operator.
         *
         *        Multiplication operator. The result is a Q15, not a Q10, for increased
         *        precision.
         *
         * @param q5 The Q5 to multiply this with.
         *
         * @return The result of the operation.
         */
        Q15 operator*(const Q5& q5) const
        {
            return Q15(v * int(q5));
        }

        /**
         * @fn Q5 Q10::operator/(const Q5& q5) const
         *
         * @brief Division operator.
         *
         *        Division operator.
         *
         * @param q5 The Q5 to divide this by.
         *
         * @return The result of the operation.
         */
        Q5 operator/(const Q5& q5) const
        {
            return Q5(v / int(q5));
        }

        /**
         * @fn template <typename T> T Q10::to() const
         *
         * @brief Converts the Q10 value to an int or a float.
         *
         *        Convert the Q10 value to an integer by removing the 10 bits used for the
         *        fraction, or to a floating point value by dividing by 32 * 32, depending on the
         *        type specified as T.
         *
         * @tparam T Either int or float.
         *
         * @return Q10 value as a type T.
         */
        template <typename T>
        T to() const
        {
            return (T)v / (T)(Rasterizer::POLY_BASE_SIZE * Rasterizer::POLY_BASE_SIZE);
        }

    private:
        int32_t v;
    };

    /**
     * @class Q15 CWRUtil.hpp touchgfx/widgets/canvas/CWRUtil.hpp
     *
     * @brief Defines a number with 15 bits reserved for fraction.
     *
     *        Defines a number with 15 bits reserved for the fractional part of the decimal
     *        number. Q15 is only used for sine/cosine and for intermediate calculations when
     *        multiplying.
     *
     *        Q5*Q5=Q10, Q10/Q5=Q5, ...
     *
     * @see Q5
     * @see Q10
     * @see http://en.wikipedia.org/wiki/Q_%28number_format%29
     * @see http://en.wikipedia.org/wiki/Fixed-point_arithmetic
     */
    class Q15
    {
    public:

        /**
         * @fn explicit Q15::Q15(int i)
         *
         * @brief Constructor from integer. No conversion is done - the integer is assumed to
         *        already be in Q15 format.
         *
         *        Constructor from integer. No conversion is done - the integer is assumed to
         *        already be in Q15 format.
         *
         * @param i int pre-formattet in Q15 format.
         */
        explicit Q15(int i) : v(i) { }

        /**
         * @fn operator int() const
         *
         * @brief Gets the Q15 as an integer without conversion.
         *
         *        Gets the Q15 as an integer without conversion.
         *
         * @return The unconverted Q15 value.
         */
        operator int() const
        {
            return v;
        }

        /**
         * @fn Q15 Q15::operator-() const
         *
         * @brief Negation operator.
         *
         *        Negation operator.
         *
         * @return The negative value of this.
         */
        Q15 operator-() const
        {
            return Q15(-v);
        }

        /**
        * @fn Q15 Q15::operator+(const Q15& q15) const
        *
        * @brief Addition operator.
        *
        *        Addition operator.
        *
        * @param q15 The Q10 to add to this.
        *
        * @return The result of the operation.
        */
        Q15 operator+(const Q15& q15) const
        {
            return Q15(v + q15.v);
        }

        /**
        * @fn Q10 Q15::operator/(const Q5& q5) const
        *
        * @brief Q5 / Q5 which requires the result of a Q15 / Q5 to be calculated.
        *
        *        Q5 / Q5 which requires the result of a Q15 / Q5 to be calculated.
        *
        * @param q5 The Q5 to divide this by.
        *
        * @return The result of the operation.
        */
        Q10 operator/(const Q5& q5) const
        {
            return Q10(v / int(q5));
        }

    private:
        int32_t v;
    };

    /**
     * @fn FORCE_INLINE_FUNCTION static Q5 toQ5(Q5 value)
     *
     * @brief Convert a Q5 to itself.
     *
     *        Convert a Q5 to itself. Allows toQ5 to be called with a variable that is already Q5.
     *
     * @param value the Q5.
     *
     * @return the value passed.
     */
    FORCE_INLINE_FUNCTION
    static Q5 toQ5(Q5 value)
    {
        return value;
    }

    /**
     * @fn template <typename T> FORCE_INLINE_FUNCTION static Q5 toQ5(T value)
     *
     * @brief Convert an integer to a fixed point number.
     *
     *        Convert an integer to a fixed point number. This is done by shifting the integer
     *        value 5 places to the left, or multiplying the floating point value by (1 &lt;&lt;
     *        5)
     *
     * @tparam T Should be either int or float.
     * @param value the integer to convert.
     *
     * @return the converted integer.
     */
#ifdef __ICCARM__
    FORCE_INLINE_FUNCTION
#endif
    template <typename T>
#ifndef  __ICCARM__
    FORCE_INLINE_FUNCTION
#endif
    static Q5 toQ5(T value)
    {
        return Q5(int(value * Rasterizer::POLY_BASE_SIZE));
    }

    /**
     * @fn template <typename T> FORCE_INLINE_FUNCTION static Q10 toQ10(T value)
     *
     * @brief Convert an integer to a fixed point number.
     *
     *        Convert an integer to a fixed point number. This is done by shifting the integer
     *        value 10 places to the left, or multiplying the floating point value by (1 &lt;
     *        &lt;
     *        510.
     *
     * @tparam T Should be either int or float.
     * @param value the integer to convert.
     *
     * @return the converted integer.
     */
#ifdef __ICCARM__
    FORCE_INLINE_FUNCTION
#endif
    template <typename T>
#ifndef  __ICCARM__
    FORCE_INLINE_FUNCTION
#endif
    static Q10 toQ10(T value)
    {
        return Q10(int(value * Rasterizer::POLY_BASE_SIZE * Rasterizer::POLY_BASE_SIZE));
    }

    /**
     * @fn static Q15 sine(int i)
     *
     * @brief Find the value of sin(i) with 15 bits precision.
     *
     *        Find the value of sin(i) with 15 bits precision. The returned value can be
     *        converted to a floating point number and divided by (1&lt;&lt;15) to get the
     *        rounded value of of sin(i). By using this function, a complete circle can be
     *        drawn without the need for using floating point math.
     *
     * @param i the angle in degrees. The angle follows the angles of the clock, 0 being straight
     *          up and 90 being 3 o'clock.
     *
     * @return the value of sin(i) with 15 bits precision on the fractional part.
     */
    static Q15 sine(int i)
    {
        const static uint16_t sineTable[91] =
        {
            0x0000, 0x023C, 0x0478, 0x06B3, 0x08EE, 0x0B28, 0x0D61, 0x0F99, 0x11D0, 0x1406,
            0x163A, 0x186C, 0x1A9D, 0x1CCB, 0x1EF7, 0x2121, 0x2348, 0x256C, 0x278E, 0x29AC,
            0x2BC7, 0x2DDF, 0x2FF3, 0x3203, 0x3410, 0x3618, 0x381D, 0x3A1C, 0x3C18, 0x3E0E,
            0x4000, 0x41ED, 0x43D4, 0x45B7, 0x4794, 0x496B, 0x4B3D, 0x4D08, 0x4ECE, 0x508E,
            0x5247, 0x53FA, 0x55A6, 0x574C, 0x58EB, 0x5A82, 0x5C13, 0x5D9D, 0x5F1F, 0x609A,
            0x620E, 0x637A, 0x64DE, 0x663A, 0x678E, 0x68DA, 0x6A1E, 0x6B5A, 0x6C8D, 0x6DB8,
            0x6EDA, 0x6FF4, 0x7104, 0x720D, 0x730C, 0x7402, 0x74EF, 0x75D3, 0x76AE, 0x7780,
            0x7848, 0x7907, 0x79BC, 0x7A68, 0x7B0B, 0x7BA3, 0x7C33, 0x7CB8, 0x7D34, 0x7DA6,
            0x7E0E, 0x7E6D, 0x7EC1, 0x7F0C, 0x7F4C, 0x7F83, 0x7FB0, 0x7FD3, 0x7FEC, 0x7FFB, 0x8000
        };
        i = ((i % 360) + 360) % 360;
        if (i <= 90)
        {
            return Q15(sineTable[i]);
        }
        if (i <= 180)
        {
            return Q15(sineTable[180 - i]);
        }
        if (i <= 270)
        {
            return Q15(-int32_t(sineTable[i - 180]));
        }
        return Q15(-int32_t(sineTable[360 - i]));
    }

    /**
     * @fn static Q15 sine(Q5 i)
     *
     * @brief Find the value of sin(i) with 15 bits precision.
     *
     *        Find the value of sin(i) with 15 bits precision. The returned value can be converted
     *        to a floating point number and divided by (1&lt;&lt;15) to get the rounded value of
     *        of sin(i). By using this function, a complete circle can be drawn without the need
     *        for using floating point math.
     *
     *        If the given degree is not an integer, the value is approximated by interpolation
     *        between sin(floor(i))
     *        and sin(ceil(i)).
     *
     * @param i the angle in degrees. The angle follows the angles of the clock, 0 being straight up
     *          and 90 being 3 o'clock.
     *
     * @return the value of sin(i) with 15 bits precision on the fractional part.
     */
    static Q15 sine(Q5 i)
    {
        Q5 _360 = toQ5<int>(360);
        i = Q5(((i % _360) + _360) % _360);
        int16_t fraction = i % Rasterizer::POLY_BASE_SIZE;
        Q15 sineLow = sine(i.to<int>());
        if (fraction == 0)
        {
            return sineLow;
        }
        Q15 sineHigh = sine(i.to<int>() + 1);
        int32_t remainder;
        return Q15(muldiv(int(sineHigh - sineLow), fraction, Rasterizer::POLY_BASE_SIZE, remainder)) + sineLow;
    }

    /**
     * @fn static Q15 cosine(int i)
     *
     * @brief Find the value of cos(i) with 15 bits precision.
     *
     *        Find the value of cos(i) with 15 bits precision using the fact that cos(i)=sin(90-
     *        i).
     *
     * @param i the angle in degrees. The angle follows the angles of the clock, 0 being straight
     *          up and 90 being 3 o'clock.
     *
     * @return the value of cos(i) with 15 bits precision on the fractional part.
     *
     * @see sine()
     */
    static Q15 cosine(int i)
    {
        return sine(90 - i);
    }

    /**
     * @fn static Q15 cosine(Q5 i)
     *
     * @brief Find the value of cos(i) with 15 bits precision.
     *
     *        Find the value of cos(i) with 15 bits precision using the fact that cos(i)=sin(90-i).
     *
     * @param i the angle in degrees. The angle follows the angles of the clock, 0 being straight up
     *          and 90 being 3 o'clock.
     *
     * @return the value of cos(i) with 15 bits precision on the fractional part.
     *
     * @see sine()
     */
    static Q15 cosine(Q5 i)
    {
        return sine(CWRUtil::toQ5<int>(90) - i);
    }

    /**
     * @fn static int8_t arcsine(Q10 q10)
     *
     * @brief Gets the arcsine of the given fraction (given as Q10).
     *
     *        Gets the arcsine of the given fraction (given as Q10). The function is most
     *        precise for angles 0-45. To limit memory requirements, values above sqrt(1/2) is
     *        calculated as 90-arcsine(sqrt(1-q10^2)). Internally.
     *
     * @param q10 The 10.
     *
     * @return An int8_t.
     */
    static int8_t arcsine(Q10 q10)
    {
        const static uint8_t arcsineTable[91] =
        {
            0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 6, 6,
            7, 7, 8, 8, 9, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13,
            14, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20,
            21, 21, 22, 22, 23, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27,
            28, 28, 29, 29, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35,
            36, 36, 37, 38, 38, 39, 39, 40, 40, 41, 42, 42, 43, 43, 44, 45
        };
        if (q10 < 0)
        {
            return -arcsine(-q10);
        }
        if (q10 > toQ10<int>(1))
        {
            return 0;    // error
        }
        // sqrt(1/2) as Q10 is "724" so if q10>724 calculate 90-arcsine(sqrt(1-q10^2))
        if (int(q10) > 724)
        {
            return 90 - arcsine(Q10(isqrt((1 << (Rasterizer::POLY_BASE_SHIFT * 4)) - int(q10) * int(q10))));
        }
        int q7 = (int(q10) + 3) >> 3; // Round Q10 to nearest Q7
        return arcsineTable[q7];
    }

    /**
     * @fn template <typename T> static int angle(T x, T y)
     *
     * @brief Find angle of a coordinate.
     *
     * @tparam T Generic type parameter (int or float).
     * @param x The x coordinate.
     * @param y The y coordinate.
     *
     * @return The angle of the coordinate.
     */
    template <typename T>
    static int angle(T x, T y)
    {
        Q5 dist;
        return angle(toQ5<T>(x), toQ5<T>(y), dist);
    }

    /**
     * @fn template <typename T> static int angle(T x, T y, T& d)
     *
     * @brief Find angle of a coordinate.
     *
     * @tparam T Generic type parameter (int or float).
     * @param x       The x coordinate.
     * @param y       The y coordinate.
     * @param [out] d The distance from (0,0) to (x,y).
     *
     * @return The angle of the coordinate.
     */
    template <typename T>
    static int angle(T x, T y, T& d)
    {
        Q5 dist;
        int a = angle(toQ5<T>(x), toQ5<T>(y), dist);
        d = dist.to<T>();
        return a;
    }

    /**
     * @fn static int angle(Q5 x, Q5 y)
     *
     * @brief Find angle of a coordinate.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     *
     * @return The angle of the coordinate.
     */
    static int angle(Q5 x, Q5 y)
    {
        Q5 dist;
        return angle(x, y, dist);
    }

    /**
     * @fn static int angle(Q5 x, Q5 y, Q5& d)
     *
     * @brief Find the angle of the coordinate (x, y).
     *
     * @param x       The x coordinate.
     * @param y       The y coordinate.
     * @param [out] d The distance from (0,0) to (x,y).
     *
     * @return The angle.
     */
    static int angle(Q5 x, Q5 y, Q5& d)
    {
        // Map to quadrant 1
        if (x >= 0)
        {
            if (y >= 0)
            {
                return 90 + _angle(x, y, d);
            }
            else // y < 0
            {
                return 90 - _angle(x, -y, d);
            }
        }
        // x < 0
        if (y >= 0)
        {
            return 270 - _angle(-x, y, d);
        }
        // y < 0
        return 270 + _angle(-x, -y, d);
    }

    /**
     * @fn static Q5 sqrtQ10(Q10 value)
     *
     * @brief Find the square root of the given value.
     *
     * @param value The value to find the square root of.
     *
     * @return The square root of the given value.
     */
    static Q5 sqrtQ10(Q10 value)
    {
        return Q5(isqrt(uint32_t(int(value))));
    }

    /**
     * @fn static Q5 muldivQ5(Q5 factor1, Q5 factor2, Q5 divisor)
     *
     * @brief Multiply two Q5's and divide by a Q5 without overflowing the multiplication.
     *
     *        Multiply two Q5's and divide by a Q5 without overflowing the multiplication (assuming
     *        that the final result can be stored in a Q5).
     *
     * @param factor1 The first factor.
     * @param factor2 The second factor.
     * @param divisor The divisor.
     *
     * @return factor1 * factor2 / divisor.
     */
    static Q5 muldivQ5(Q5 factor1, Q5 factor2, Q5 divisor)
    {
        int32_t remainder;
        return Q5(muldiv(int(factor1), int(factor2), int(divisor), remainder));
    }

    /**
     * @fn static Q5 muldivQ10(Q10 factor1, Q10 factor2, Q10 divisor)
     *
     * @brief Multiply two Q5's and divide by a Q5 without overflowing the multiplication.
     *
     *        Multiply two Q5's and divide by a Q5 without overflowing the multiplication (assuming
     *        that the final result can be stored in a Q5).
     *
     * @param factor1 The first factor.
     * @param factor2 The second factor.
     * @param divisor The divisor.
     *
     * @return factor1 * factor2 / divisor.
     */
    static Q5 muldivQ10(Q10 factor1, Q10 factor2, Q10 divisor)
    {
        int32_t remainder;
        return Q5(muldiv(int(factor1), int(factor2), int(divisor), remainder) / Rasterizer::POLY_BASE_SIZE);
    }

    /**
     * @fn static Q5 mulQ5(Q5 factor1, Q5 factor2)
     *
     * @brief Multiply two Q5's returning a new Q5.
     *
     *        Multiply two Q5's returning a new Q5 without overflowing.
     *
     * @param factor1 The first factor.
     * @param factor2 The second factor.
     *
     * @return factor1 * factor2.
     */
    static Q5 mulQ5(Q5 factor1, Q5 factor2)
    {
        return muldivQ5(factor1, factor2, CWRUtil::toQ5<int>(1));
    }

    /**
     * @fn static Q5 mulQ5(Q5 factor1, Q10 factor2)
     *
     * @brief Multiply one Q5 by a Q10 returning a new Q5.
     *
     *        Multiply one Q5 by a Q10 returning a new Q5 without overflowing.
     *
     * @param factor1 The first factor.
     * @param factor2 The second factor.
     *
     * @return factor1 * factor2.
     */
    static Q5 mulQ5(Q5 factor1, Q10 factor2)
    {
        return muldivQ10(Q10(int(factor1) * Rasterizer::POLY_BASE_SIZE), factor2, CWRUtil::toQ10<int>(1));
    }

private:
    static int _angle(Q5 x, Q5 y, Q5& d)
    {
        assert(x >= 0 && y >= 0);
        // The sine table from 0-45 is more precise, so swap x/y to get the angle in that range
        if (x < y)
        {
            return 90 - _angle(y, x, d);
        }

        Q5 _1 = toQ5<int>(1); // Used to convert Q5->Q10->Q15
        d = sqrtQ10(x * x + y * y);
        if (d == 0)
        {
            return 0;    // Error
        }
        int32_t remainder;
        Q10 dy = Q10(muldiv(int(y), int(_1 * _1), int(d), remainder));
        return arcsine(dy);
    }

    static uint32_t isqrt(uint32_t n)
    {
        uint32_t root = 0, bit, trial;
        bit = (n >= 0x10000) ? 1 << 30 : 1 << 14;
        do
        {
            trial = root + bit;
            if (n >= trial)
            {
                n -= trial;
                root = trial + bit;
            }
            root >>= 1;
            bit >>= 2;
        }
        while (bit);
        return root;
    }
};
} // namespace touchgfx

#endif // CWRUTIL_HPP
