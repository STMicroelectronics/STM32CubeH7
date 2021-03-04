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

#ifndef CONFIG_HPP
#define CONFIG_HPP

#define STR(X) STR_I(X)
#define STR_I(X) #X

/**
 * Section placements of data.
 */
#ifdef SIMULATOR
#define LOCATION_PRAGMA(name)
#define LOCATION_ATTRIBUTE(name)
#else
#ifdef __GNUC__
#define LOCATION_PRAGMA(name)
#define LOCATION_ATTRIBUTE(name) __attribute__ ((section (STR(name)))) __attribute__ ((aligned(4)))
#elif defined __ICCARM__
#define LOCATION_PRAGMA(name) _Pragma(STR(location=name))
#define LOCATION_ATTRIBUTE(name)
#elif defined(__ARMCC_VERSION)
#define LOCATION_PRAGMA(name)
#define LOCATION_ATTRIBUTE(name) __attribute__ ((section (name))) __attribute__ ((aligned(4)))
#endif
#endif

/**
 * Macros for backwards compatibility with TouchGFX 4.11 (and older) applications
 */
#define FONT_GLYPH_LOCATION_FLASH_PRAGMA LOCATION_PRAGMA("FontFlashSection")
#define FONT_GLYPH_LOCATION_FLASH_ATTRIBUTE LOCATION_ATTRIBUTE("FontFlashSection")

#define FONT_TABLE_LOCATION_FLASH_PRAGMA LOCATION_PRAGMA("FontFlashSection")
#define FONT_TABLE_LOCATION_FLASH_ATTRIBUTE LOCATION_ATTRIBUTE("FontFlashSection")

#define FONT_KERNING_LOCATION_FLASH_PRAGMA LOCATION_PRAGMA("FontFlashSection")
#define FONT_KERNING_LOCATION_FLASH_ATTRIBUTE LOCATION_ATTRIBUTE("FontFlashSection")

#define TEXT_LOCATION_FLASH_PRAGMA LOCATION_PRAGMA("TextFlashSection")
#define TEXT_LOCATION_FLASH_ATTRIBUTE LOCATION_ATTRIBUTE("TextFlashSection")

#define LOCATION_EXTFLASH_PRAGMA LOCATION_PRAGMA("ExtFlashSection")
#define LOCATION_EXTFLASH_ATTRIBUTE LOCATION_ATTRIBUTE("ExtFlashSection")

/**
 * To force inline of time critical functions
 */
#ifdef SIMULATOR
#define FORCE_INLINE_FUNCTION inline
#else
#ifdef __GNUC__
#define FORCE_INLINE_FUNCTION __attribute__((always_inline)) inline
#elif defined __ICCARM__
#define FORCE_INLINE_FUNCTION _Pragma("inline=forced")
#pragma diag_suppress=Pe236
#elif defined(__ARMCC_VERSION)
#define FORCE_INLINE_FUNCTION __forceinline
#endif
#endif

/**
 * To be able to use __restrict__ on the supported platform. The IAR compiler does not support this
 */
#ifdef __GNUC__
#define RESTRICT __restrict__
#else
#define RESTRICT
#endif // __GNUC__

/**
 * Use KEEP to make sure the compiler does not remove this
 */
#ifdef __ICCARM__
#define KEEP __root
#else
#define KEEP
#endif

#endif // CONFIG_HPP
