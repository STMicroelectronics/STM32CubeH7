/**
  @page FreeRTOS_AMP_RTOS_BareMetal FreeRTOS Baremetal Multicore processing application

  @verbatim
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_AMP_RTOS_BareMetal/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS AMP RTOS BareMetal application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

This example shows how to use FreeRTOS's message buffers to pass data from one core to another.

The core that sends the data is the Cortex-M7 referred to as core 1.
The core that receives the data is the  Cortex-M4 referred to as core 2.
The task implemented by prvCore1Task() runs on core 1. Core 2 is running in Baremetal (without RTOS).

prvCore1Task() task sends messages via message buffers to the application running on Core 2 then delay for 500 ms.

Core to core communication via message buffer requires the message buffers
to be at an address known to both cores within shared memory (D3_SRAM is used as shared memory).

The Core 2 will check the received message from Core 1 and notify the user by the following:
  - Toggle LED1 if the received message is correct
  - Turn On LED2 if the received message is wrong and enter an empty infinite loop

The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz
  - Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals
    and D3 Domain APB4 peripherals to run at 100MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set accordingly to the
      OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications
on STM32Cube with RTOS".


@Note For the Cortex-M7, if the application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex-M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

RTOS, FreeRTOS, multiprocessor, Threading, AMP, Asymmetric Multi Processing, Baremetal, Dual core, Message, Task, Shared memory


@par Directory contents


    - FreeRTOS/FreeRTOS_AMP_RTOS_BareMetal/Common/Src/system_stm32h7xx.c  STM32H7xx system configuration source file

    - FreeRTOS/FreeRTOS_AMP_RTOS_BareMetal/CM7/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M7
    - FreeRTOS/FreeRTOS_AMP_RTOS_BareMetal/CM7/Inc/FreeRTOSConfig.h        FreeRTOS Configuration file
    - FreeRTOS/FreeRTOS_AMP_RTOS_BareMetal/CM7/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M7
    - FreeRTOS/FreeRTOS_AMP_RTOS_BareMetal/CM7/Inc/main.h                  Header for main.c module for Cortex-M7
    - FreeRTOS/FreeRTOS_AMP_RTOS_BareMetal/CM7/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M7
    - FreeRTOS/FreeRTOS_AMP_RTOS_BareMetal/CM7/Src/main.c                  Main program for Cortex-M7

    - FreeRTOS/FreeRTOS_AMP_RTOS_BareMetal/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M4
    - FreeRTOS/FreeRTOS_AMP_RTOS_BareMetal/CM4/Inc/FreeRTOSConfig.h        FreeRTOS Configuration file
    - FreeRTOS/FreeRTOS_AMP_RTOS_BareMetal/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M4
    - FreeRTOS/FreeRTOS_AMP_RTOS_BareMetal/CM4/Inc/main.h                  Header for main.c module for Cortex-M4
    - FreeRTOS/FreeRTOS_AMP_RTOS_BareMetal/CM4/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M4
    - FreeRTOS/FreeRTOS_AMP_RTOS_BareMetal/CM4/Src/main.c                  Main program for Cortex-M4


@par Hardware and Software environment

  - This example runs on STM32H745xx/STM32H755xx devices.

  - This example has been tested with NUCLEO-H745ZI-Q board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your 2 images into target memory for each core.
 - Reset the Board so the 2 CPUs start running their respective code.



 */
