/**
  @page FreeRTOS_SemaphoreFromISR FreeRTOS semaphore from ISR application
 
  @verbatim
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_SemaphoreFromISR/readme.txt
  * @author  MCD Application Team 
  * @brief   Description of the FreeRTOS semaphore from ISR application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description

How to use semaphore from ISR with CMSIS RTOS API.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals and initialize the Flash interface.
The HAL initializes the TIM6 to generate an interrupt each 1ms, it will be used as timebase for the HAL drivers.
This application uses FreeRTOS, the RTOS initializes the systick to generate an interrupt each 1ms. 
The systick is then used for FreeRTOS timebase.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

This application creates a thread that toggle a LED through semaphore given from ISR.
 
Each time the user pushes the USER push-button of the NUCLEO-H743ZI board the semaphore 
is given to the SemaphoreTest thread to toggle the LED1.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in HAL time base ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set accordingly to the 
      OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".


@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

RTOS, FreeRTOS, Threading, Semaphore, Priorities, Button, LED, Interrupt

@par Directory contents

    - FreeRTOS/FreeRTOS_SemaphoreFromISR/Src/main.c                Main program
    - FreeRTOS/FreeRTOS_SemaphoreFromISR/Src/stm32h7xx_it.c        Interrupt handlers
    - FreeRTOS/FreeRTOS_SemaphoreFromISR/Src/system_stm32h7xx.c    STM32H7xx system clock configuration file
    - FreeRTOS/FreeRTOS_SemaphoreFromISR/Inc/main.h                Main program header file
    - FreeRTOS/FreeRTOS_SemaphoreFromISR/Inc/stm32h7xx_hal_conf.h  HAL Library Configuration file
    - FreeRTOS/FreeRTOS_SemaphoreFromISR/Inc/stm32h7xx_it.h        Interrupt handlers header file
    - FreeRTOS/FreeRTOS_SemaphoreFromISR/Inc/FreeRTOSConfig.h      FreeRTOS Configuration file

@par Hardware and Software environment

  - This example runs on STM32H743xx device.
    
  - This example has been tested with NUCLEO-H743ZI board and can be
    easily tailored to any other supported device and development board.
    

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
 

 */
