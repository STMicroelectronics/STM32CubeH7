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

#ifndef ATOMIC_HPP
#define ATOMIC_HPP

/**
 * Defines a atomic write on supported platforms
 */

#if defined(WIN32) || defined(_WIN32)

#include <windows.h>
typedef LONG atomic_t;

/**
 * @fn inline void atomic_set(atomic_t& target, atomic_t value)
 *
 * @brief Makes a atomic write of value to target.
 *
 *        Makes a atomic write of value to target.
 *
 * @param [out] target The value to write to.
 * @param value        The value to write.
 */
inline void atomic_set(atomic_t& target, atomic_t value)
{
    InterlockedExchange(&target, value);
}

#elif defined(__GNUC__)

#include <csignal>
typedef sig_atomic_t atomic_t;

/**
 * @fn inline void atomic_set(atomic_t& target, atomic_t value)
 *
 * @brief Makes a atomic write of value to target.
 *
 *        Makes a atomic write of value to target.
 *
 * @param [out] target The value to write to.
 * @param value        The value to write.
 */
inline void atomic_set(atomic_t& target, atomic_t value)
{
    __sync_synchronize();
    target = value;
}

#elif defined(__IAR_SYSTEMS_ICC__)

typedef unsigned long atomic_t;

/**
 * @fn inline void atomic_set(atomic_t& target, atomic_t value)
 *
 * @brief Makes a atomic write of value to target.
 *
 *        Makes a atomic write of value to target.
 *
 * @note Assume that 32 bit writes are atomic.
 *
 * @param [out] target The value to write to.
 * @param value        The value to write.
 */
inline void atomic_set(atomic_t& target, atomic_t value)
{
    target = value;
}
#elif defined(__ARMCC_VERSION)

typedef unsigned long atomic_t;

/**
 * @fn inline void atomic_set(atomic_t& target, atomic_t value)
 *
 * @brief Makes a atomic write of value to target.
 *
 *        Makes a atomic write of value to target.
 *
 * @param [out] target The value to write to.
 * @param value        The value to write.
 */
inline void atomic_set(atomic_t& target, atomic_t value)
{
    target = value;
}
#else

#error "Compiler/platform not supported"

#endif

#endif // ATOMIC_HPP
