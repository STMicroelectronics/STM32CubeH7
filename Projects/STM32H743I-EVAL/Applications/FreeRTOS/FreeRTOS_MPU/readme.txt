/**
  @page FreeRTOS_MPU FreeRTOS MPU application
 
  @verbatim
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_MPU/readme.txt
  * @author  MCD Application Team  
  * @brief   Description of the FreeRTOS MPU application.
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

This application aims to describe how to use the MPU feature of FreeRTOS.

This application implements five tasks in both User mode and Privileged mode, 
and using both the xTaskCreate() and xTaskCreateRestricted() API functions. 

Two tasks are created using xTaskCreate() 'UserModeTask' and 'PrivilegedModeTask'
run in user mode and privileged mode respectively they aims to test the difference
between two modes. these tasks execute once then delete themselves.

Three tasks are created using xTaskCreateRestricted():
  - 'Test1Task': sends I'm alive message each 100 ms 
  - 'Test2Task': sends I'm alive message each 100 ms
  - 'CheckTask': tests the MPU configured memory regions,
                 controls the execution of the two previous tasks,
                 increments ReadWriteArray[0] when it receives I'm alive message from task1
                 increments ReadWriteArray[1] when it receives I'm alive message from task2
                 increments ReadWriteArray[2], sets ReadWriteArray[0] and ReadWriteArray[1] to 0
                 when it receives Reset Cycle message from Application hook task		 
                 

'Test1Task' is created in privileged mode and 'Test2Task' is created in user mode,
this will demonstrate how to use global variable in different modes

the reset cycle message will be sent from the application tick hook each 5 seconds 

A queue is used to demonstrate passing data between protected/restricted tasks 
as well as passing data between an interrupt and protected/restricted task.


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

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the �MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
����� If the application needs to use DMA(or other masters) based access or requires more RAM, then �the user has to:
����������� � - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
����������� � - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
�������       - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly�defined to be aligned to L1-CACHE line size (32 bytes). 
�
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
������Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
������Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

RTOS, FreeRTOS, MPU

@par Directory contents
    - FreeRTOS/FreeRTOS_MPU/Src/main.c                       Main program
    - FreeRTOS/FreeRTOS_MPU/Src/stm32h7xx_hal_timebase_tim.c HAL timebase file
    - FreeRTOS/FreeRTOS_MPU/Src/stm32h7xx_it.c               Interrupt handlers
    - FreeRTOS/FreeRTOS_MPU/Src/system_stm32h7xx.c           STM32H7xx system clock configuration file
    - FreeRTOS/FreeRTOS_MPU/Inc/main.h                       Main program header file
    - FreeRTOS/FreeRTOS_MPU/Inc/stm32h7xx_hal_conf.h         HAL Library Configuration file
    - FreeRTOS/FreeRTOS_MPU/Inc/stm32h7xx_it.h               Interrupt handlers header file
    - FreeRTOS/FreeRTOS_MPU/Inc/FreeRTOSConfig.h             FreeRTOS Configuration file

@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board.
    

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  

 */
