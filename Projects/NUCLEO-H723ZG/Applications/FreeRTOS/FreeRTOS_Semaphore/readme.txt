/**
  @page FreeRTOS_Semaphore FreeRTOS Semaphore example

  @verbatim
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_Semaphore/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS Semaphore example.
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

@par Application Description

How to use semaphores with CMSIS RTOS API.

This application creates two threads that toggle LEDs through a shared semaphore,
as following:

The first thread which have the higher priority obtains the semaphore and
toggle the LED1 each 200 ms. After 5 seconds it releases the semaphore and
suspends itself.

The low priority thread can execute now, it obtains the semaphore and
resume execution of the first thread, as it has the higher priority
the first thread will try to obtain the semaphore but it fails because
the semaphore is already taken by the low priority thread, which will
toggle the LED2 each 200 ms for 5 seconds before releasing the semaphore
to begin a new cycle

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in HAL time base ISR.
      This implies that if HAL_Delay() is called from a peripheral ISR process, then
      the HAL time base interrupt must have higher priority (numerically lower) than
      the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority()
      function.

@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set accordingly to the
      OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications
on STM32Cube with RTOS".

@par Keywords

RTOS, FreeRTOS, Threading, Semaphore, Priorities

@par Directory contents
    - FreeRTOS/FreeRTOS_Semaphore/Src/main.c                       Main program
    - FreeRTOS/FreeRTOS_Semaphore/Src/stm32h7xx_hal_timebase_tim.c HAL timebase file
    - FreeRTOS/FreeRTOS_Semaphore/Src/stm32h7xx_it.c               Interrupt handlers
    - FreeRTOS/FreeRTOS_Semaphore/Src/stm32h7xx_hal_msp.c          MSP Initialization file
    - FreeRTOS/FreeRTOS_Semaphore/Src/system_stm32h7xx.c           STM32H7xx system clock configuration file
    - FreeRTOS/FreeRTOS_Semaphore/Inc/main.h                       Main program header file
    - FreeRTOS/FreeRTOS_Semaphore/Inc/stm32h7xx_hal_conf.h         HAL Library Configuration file
    - FreeRTOS/FreeRTOS_Semaphore/Inc/stm32h7xx_it.h               Interrupt handlers header file
    - FreeRTOS/FreeRTOS_Semaphore/Inc/FreeRTOSConfig.h             FreeRTOS Configuration file
    - FreeRTOS/FreeRTOS_Semaphore/Inc/stm32h7xx_nucleo_conf.h    BSP configuration file

@par Hardware and Software environment

  - This application runs on STM32H723xx devices.

  - This application has been tested with NUCLEO-H723ZG board and can be
    easily tailored to any other supported device and development board.


@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
