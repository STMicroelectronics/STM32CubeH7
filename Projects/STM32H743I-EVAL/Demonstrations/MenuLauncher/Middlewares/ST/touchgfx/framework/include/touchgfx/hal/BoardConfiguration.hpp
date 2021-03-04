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

#ifndef BOARDCONFIGURATION_HPP
#define BOARDCONFIGURATION_HPP

namespace touchgfx
{
/**
 * @fn void hw_init();
 *
 * @brief Function to perform generic hardware initialization of the board.
 *
 *        Function to perform generic hardware initialization of the board. This function
 *        prototype is only provided as a convention.
 */
void hw_init();

/**
 * @fn void touchgfx_init();
 *
 * @brief Function to perform touchgfx initialization.
 *
 *        Function to perform touchgfx initialization. This function prototype is only provided
 *        as a convention.
 */
void touchgfx_init();
} // namespace touchgfx

#endif // BOARDCONFIGURATION_HPP
