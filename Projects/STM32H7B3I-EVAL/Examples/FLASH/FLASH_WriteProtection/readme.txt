/**
  @page FLASH_WriteProtection FLASH write protection
  
  @verbatim
  ******************************************************************************
  * @file    FLASH/FLASH_WriteProtection/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FLASH write protection example.
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

How to configure and use the FLASH HAL API to enable and disable the write 
protection of the internal Flash memory.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280 MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.

Each time the Tamper push-button is pressed, the program will check the 
write protection status of FLASH_WRP_SECTORS (defined in main.c)
  - If FLASH_WRP_SECTORS are write protected, the write protection will be disabled.
    if protection disable operation is not done correctly then LED2 will
    remain toggling each 100ms. 
    Otherwise, LED1 turns On and LED3 turns Off to indicate that Write protection is correctly disabled.

  - If FLASH_WRP_SECTORS are not write protected, the write protection will be enabled.
    if protection enable operation is not done correctly then LED2 will
    remain toggling each 100ms.
    Otherwise, LED1 turns Off and LED3 turns On to indicate that Write protection is correctly enabled.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds) 
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from 
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower) 
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set 
      to 1 millisecond to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA (or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

Memory, Flash, Write protection, Sector, Program, Erase

@par Directory contents 

  - FLASH/FLASH_WriteProtection/Inc/stm32h7xx_hal_conf.h        HAL Configuration file  
  - FLASH/FLASH_WriteProtection/Inc/stm32h7xx_it.h              Header for stm32h7xx_it.c
  - FLASH/FLASH_WriteProtection/Inc/main.h                      Header for main.c module 
  - FLASH/FLASH_WriteProtection/Inc/stm32h7b3i_eval_conf.h      STM32H7B3I-EVAL board configuration file
  - FLASH/FLASH_WriteProtection/Src/stm32h7xx_it.c              Interrupt handlers
  - FLASH/FLASH_WriteProtection/Src/main.c                      Main program
  - FLASH/FLASH_WriteProtection/Src/system_stm32h7xx.c          STM32H7xx system clock configuration file

@par Hardware and Software environment 
  - This example runs on STM32H7B3xxQ devices.
    
  - This example has been tested with STM32H7B3I-EVAL board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
   

 */
