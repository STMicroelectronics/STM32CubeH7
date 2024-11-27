/**
  @page HSEM_ProcessSync HSEM process Sync example

  @verbatim
  ******************************************************************************
  * @file    HSEM/HSEM_ProcessSync/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the HSEM Process Sync example.
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

How to use a HW semaphore to synchronize 2 process

In this example, HW semaphore (9) is used to synchronize 2 process.
Process 1:
- Sends the message "H E L L O" through the UART.
- Waits 700 ms between each 2 consecutive characters.
Process 2:
- Sends the message "  E V E R Y B O D Y" through the UART.

This projects is configured for STM32H7B3xxQ devices using STM32CubeH7 HAL and running on
STM32H7B3I-EVAL board from STMicroelectronics.

At the beginning of the main program:
- The HAL_Init() function is called to:
  - Reset all the peripherals
  - Initialize the Flash interface and the systick.
- The SystemClock_Config() function is used to configure the system clock:
  - The CPU at 280MHz
  - The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280MHz.
  - The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals at 140MHz.

First semaphore (9) is taken using HAL API HAL_HSEM_Take with process ID  set to "12".
if the semaphore take is obtained a "H E L L O" message is sent to the user hyper terminal.
A delay of 700ms is inserted between each 2 consecutive characters.
once the total message is sent the semaphore is released using HAL API HAL_HSEM_Release.

Note that HAL_HSEM_Take use the 2-Step semaphore take method (refer to the HW Semaphore
description in the reference manual)

Pushing the "Tamper" button will generate an EXTI interrupt.
The corresponding EXTI callback will try to take the same semaphore (9) using
HAL API HAL_HSEM_FastTake.
If the semaphore has been released by first process , then the take is
obtained immediately and It will send the message "  E V E R Y B O D Y" to the user hyper terminal.

If not ,HW semaphore 9 not released yet by the first process , then the HAL_HSEM_FastTake API
will return HAL_ERROR meaning that the semaphore is already taken by another process.
In this case the HAL API HAL_HSEM_ActivateNotification is called to enable receiving
a notification once the semaphore will be released.
Treatment will be postponed until the semaphore is released.
Once done, a HSEM interrupt is generated (semaphore 9), and HAL_HSEM_FreeCallback is asserted
to send the " E V E R Y B O D Y" message.

Note that the API HAL_HSEM_FastTake use the 1-Step semaphore take method (refer to the HW Semaphore
description in the reference manual) where the process ID is assumed to be zero.

As a result both process will be synchronized, second process will send its message
" E V E R Y B O D Y" only when first process has ended sending its message "H E L L O".

Comment the define "USE_HW_SEMAPHORE" in the main.h to disable the HW semaphore
synchronization. In this case when the "Tamper" button is pushed the EXTI
callback will send the " E V E R Y B O D Y" message immediately (even if first message sending is ongoing)
leading to an unorganized message on the user hyper terminal.

@note  LED3 is on in case of error.
       LED1 blinks every 100 ms when the process is complete.

@note Make sure that jumper JP21 is on TX position and JP20 is on RX position
  - Hyper-terminal configuration:
    - Data Length = 7 Bits
    - One Stop Bit
    - Odd parity
    - BaudRate = 9600 baud
    - Flow control: None

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@note It is recommended to enable the cache and maintain its coherence.
      Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
      Please refer to the AN4839 “Level 1 cache on STM32F7 Series”

@par Keywords

Hardware semaphore, Semaphore, HSEM, EXTI, Synchronize, Lock, Unlock, Take, Release, Process, LED

@par Directory contents

  - HSEM/HSEM_ProcessSync/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - HSEM/HSEM_ProcessSync/Inc/stm32h7b3i_eval_conf.h  STM32H7B3I_EVAL board configuration file
  - HSEM/HSEM_ProcessSync/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - HSEM/HSEM_ProcessSync/Inc/main.h                  Header for main.c module

  - HSEM/HSEM_ProcessSync/Src/stm32h7xx_it.c          Interrupt handlers
  - HSEM/HSEM_ProcessSync/Src/main.c                  Main program
  - HSEM/HSEM_ProcessSync/Src/system_stm32h7xx.c      STM32H7xx system source file

@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ devices.

  - This example has been tested with STMicroelectronics STM32H7B3I-EVAL
    board and can be easily tailored to any other supported device
    and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example




 */
