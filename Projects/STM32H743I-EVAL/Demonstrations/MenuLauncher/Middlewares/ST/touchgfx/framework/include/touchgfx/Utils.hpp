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

#ifndef UTILS_HPP
#define UTILS_HPP

#ifdef SIMULATOR
#include <stdarg.h>
#ifndef __linux__
#include <windows.h>
#endif
#include <stdio.h>
#endif
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>

#if defined(SIMULATOR) && !defined(__linux__)

/**
 * @fn void touchgfx_enable_stdio();
 *
 * @brief Ensure there is a console window to print to and read from.
 *
 *        Ensure there is a console window to print to using printf() or std::cout, and read
 *        from using e.g. fgets or std::cin. Alternatively, instead of using printf(), simply
 *        use touchgfx_printf() which will ensure there is a console to write to. After the
 *        first call to touchgfx_printf, it will also be possible to read from stdin.
 *
 * @see touchgfx_printf
 */
void touchgfx_enable_stdio();
#else
#define touchgfx_enable_stdio()
#endif

#ifdef SIMULATOR

/**
 * @fn void touchgfx_printf(const char* format, ...);
 *
 * @brief TouchGFX printf.
 *
 *        TouchGFX printf that will only work on simulators. On Windows systems, a new console
 *        window will be created where the output is printed, on Linux systems, output will be
 *        sent to /var/log/syslog. For target systems, calls to touchgfx_printf() will be
 *        removed and not cause any use of memory.
 *        Unlike printf(), touchfgx_printf() does not return number of characters written.
 *
 * @param format Describes the format to use, see printf().
 * @param ...    Variable arguments providing additional information.
 *
 * @see touchgfx_enable_stdio
 */
void touchgfx_printf(const char* format, ...);
#else
#define touchgfx_printf(format, ...)
#endif

namespace touchgfx
{
/**
 * Provides utility functions.
 */

/**
 * @fn void memset(void* data, uint8_t c, uint32_t size);
 *
 * @brief Simple implementation of the standard memset function.
 *
 *        Simple implementation of the standard memset function.
 *
 * @param [out] data Address of data to set.
 * @param       c    Value to set.
 * @param       size Number of bytes to set.
 */
void memset(void* data, uint8_t c, uint32_t size);

/**
 * @fn RenderingVariant lookupNearestNeighborRenderVariant(const Bitmap& bitmap);
 *
 * @brief Returns the associated nearest neighbor render variant based on the bitmap format.
 *
 *        Returns the associated nearest neighbor render variant based on the bitmap format.
 *
 * @param bitmap The bitmap.
 *
 * @return A RenderingVariant based on the bitmap format.
 */
RenderingVariant lookupNearestNeighborRenderVariant(const Bitmap& bitmap);

/**
 * @fn RenderingVariant lookupBilinearRenderVariant(const Bitmap& bitmap);
 *
 * @brief Returns the associated bilinear render variant based on the bitmap format.
 *
 *        Returns the associated bilinear render variant based on the bitmap format.
 *
 * @param bitmap The bitmap.
 *
 * @return A RenderingVariant based on the bitmap format.
 */
RenderingVariant lookupBilinearRenderVariant(const Bitmap& bitmap);

/**
 * @fn template <class T> T abs(T d)
 *
 * @brief Simple implementation of the standard abs function
 *
 *        Simple implementation of the standard abs function.
 *
 * @tparam T The type on which to perform the abs.
 * @param d The entity on which to perform the abs.
 *
 * @return The absolute (non-negative) value of d.
 */
template <typename T>
T abs(T d)
{
    if (d < 0)
    {
        return -d;
    }
    return d;
}

/**
 * @fn template <typename T> T gcd(T a, T b)
 *
 * @brief Find greatest common divisor
 *
 *        Find greatest common divisor of two given numbers.
 *
 * @tparam T Generic type parameter.
 * @param a The first number.
 * @param b The second number.
 *
 * @return A T.
 */
template <typename T>
T gcd(T a, T b)
{
    for (;;)
    {
        if (a == 0)
        {
            return b;
        }
        b %= a;
        if (b == 0)
        {
            return a;
        }
        a %= b;
    }
}

/**
 * @fn static int32_t clz(int32_t x)
 *
 * @brief Count leading zeros.
 *
 *        Count leading zeros in the binary representation of the given value.
 *
 * @param x The value to count the number of leading zeros in.
 *
 * @return An int32_t.
 */
int32_t clz(int32_t x);

/**
 * @fn static int32_t muldiv(int32_t factor1, int32_t factor2, int32_t divisor, int32_t& remainder)
 *
 * @brief Multiply and divide.
 *
 *        Multiply and divide without causing overflow. Multiplying two large values and
 *        subsequently dividing the result with another large value might cause an overflow in
 *        the intermediate result. The function muldiv() will multiply the two first values and
 *        divide the result by the third value without causing overflow (unless the final
 *        result would overflow). The remainder from the calculation is also returned.
 *
 * @param       factor1   The first factor.
 * @param       factor2   The second factor.
 * @param       divisor   The divisor.
 * @param [out] remainder The remainder.
 *
 * @return An int32_t.
 */
int32_t muldiv(int32_t factor1, int32_t factor2, int32_t divisor, int32_t& remainder);
} // namespace touchgfx

#endif // UTILS_HPP
