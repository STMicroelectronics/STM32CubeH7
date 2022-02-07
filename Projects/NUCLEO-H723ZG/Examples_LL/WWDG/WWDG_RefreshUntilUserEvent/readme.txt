/**
  @page WWDG_RefreshUntilUserEvent WWDG example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/WWDG/WWDG_RefreshUntilUserEvent/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the WWDG_RefreshUntilUserEvent example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

Configuration of the WWDG to periodically update the counter and 
generate an MCU WWDG reset when a user button is pressed. The peripheral initialization 
uses the LL unitary service functions for optimization purposes (performance and size).

Example Configuration:
Configure the WWDG (Window, Prescaler & Counter) and enable it.
Refresh the WWDG downcounter in the main loop - LED1 is blinking fastly & continuously

Example Execution:
When User push-button is pressed, the Downcounter automatic refresh mechanism is disabled and thus, reset occurs.
After a reset when re-entering the main function, RCC WWDG Reset Flag is checked and if we are back from a WWDG reset the LED1 is switched ON.

A new User push-button press re-activates the WWDG.

@par Keywords

System, WWDG, Downcounter, MCU Reset, Timeout, Software fault

@par Directory contents 

  - WWDG/WWDG_RefreshUntilUserEvent/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - WWDG/WWDG_RefreshUntilUserEvent/Inc/main.h                  Header for main.c module
  - WWDG/WWDG_RefreshUntilUserEvent/Inc/stm32_assert.h          Template file to include assert_failed function
  - WWDG/WWDG_RefreshUntilUserEvent/Src/stm32h7xx_it.c          Interrupt handlers
  - WWDG/WWDG_RefreshUntilUserEvent/Src/main.c                  Main program
  - WWDG/WWDG_RefreshUntilUserEvent/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H723xx devices.
    
  - This example has been tested with NUCLEO-H723ZG board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
