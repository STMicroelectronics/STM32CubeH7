/**
  @page SD_ReadWrite_DMADoubleBuffer Description of the SD_ReadWrite_DMADoubleBuffer example

  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    SD/SD_ReadWrite_SD_ReadWrite_DMADoubleBuffer/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the SD read and write multi buffer with internal DMA example.
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
This example performs some write and read transfers to SD Card with SDMMC IP internal
DMA mode and calculate write and read transfer speed.


At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H735xx Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

Next, a write transfer of 100MB to SD Card is performed to calculate the write speed.
Then with the same parameters, a read transfer of 100MB from SD Card is performed to calculate
read speed.
2 Buffers are used to read and write with DMA Multi Buffer mode.

STM32H735G-DK board's LEDs can be used to monitor the transfer status:
 - LED1 is Toggled when transfer is ongoing (read or write).
 - LED1 is ON when write and read transfer is completed and check buffers are OK.
 - LED RED is ON when there is an error in transfer process or initialization.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Storage, SD, SDMMC, Read Write,Block, sector, DMA, Double Buffer 

@par Directory contents

  - SD/SD_ReadWrite_DMA/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - SD/SD_ReadWrite_DMA/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - SD/SD_ReadWrite_DMA/Inc/main.h                  Header for main.c module
  - SD/SD_ReadWrite_DMA/Inc/stm32h735g_discovery_conf.h  STM32H735G-DK board configuration file
  - SD/SD_ReadWrite_DMA/Src/stm32h7xx_it.c          Interrupt handlers
  - SD/SD_ReadWrite_DMA/Src/main.c                  Main program
  - SD/SD_ReadWrite_DMA/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - SD/SD_ReadWrite_DMA/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H735xx devices.

  - This example has been tested with STM32H735G-DK board and can be
    easily tailored to any other supported device and development board.

  STM32H735G-DK Set-up :	

  - You can display test message on the HyperTerminal,
      * Configure your HyperTerminal with UART configuration:
        - Data Length = 7 Bits
        - One Stop Bit
        - Odd parity
        - BaudRate = 9600 baud
        - Flow control: None


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Plug the SD Card in the STM32H735G-DK
 - Run the example


 */
