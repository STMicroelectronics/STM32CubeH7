/**
  @page FreeRTOS_Semaphore FreeRTOS Semaphore application
 
  @verbatim
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_Semaphore/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS Semaphore application.
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

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
      Please refer to the AN4839 “Level 1 cache on STM32F7 Series and STM32H7 Series"

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

  - This application runs on STM32H7A3xxQ devices.
    
  - This application has been tested with Nucleo-H7A3ZI-Q board and can be
    easily tailored to any other supported device and development board.
    

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  

 */
