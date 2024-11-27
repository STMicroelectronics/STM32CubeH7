/**
  @page SD_ReadWrite_IT  Description of the SD_ReadWrite_IT example

  @verbatim
  ******************************************************************************
  * @file    SD/SD_ReadWrite_IT/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the SD read and write buffer with IT mode example.
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
This example performs some write and read transfers to SD Card with SDMMC IP IT
mode and calculate write and read transfer speed.

@note This example works with Normal SD Card.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3XXQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280 MHz.
The APB clock dividers for CD  Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD  Domain APB4 peripherals to run at 280 MHz/2.

The transfer clock is set at 25Mhz (SDMMC_CLK / Clock_DIV * 2) and configured within the
SD Handle init parameters.

Next, a write transfer of 100MB to SD Card is performed to calculate the write speed.
Then with the same parameters, a read transfer of 100MB from SD Card is performed to calculate
read speed.
The transfers of write and read are performed with the IT mode.
The operation of writing and reading is performed in infinity loop to measure the
SD card performance.

STM32H7B3I-EVAL  board's LEDs can be used to monitor the transfer status:
 - LED4 is Toggled when transfer is ongoing (read or write).
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
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).
 
@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
              Please refer to the AN4839 “Level 1 cache on STM32F7 Series and STM32H7”

@par Keywords

Storage, SD, SDMMC, Read Write, Block, sector, interrupt

@par Directory contents

  - SD/SD_ReadWrite_IT/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - SD/SD_ReadWrite_IT/Inc/main.h                  Header for main.c module
  - SD/SD_ReadWrite_IT/Inc/stm32h7b3i_eval_conf.h  STM32H7B3I-EVAL board configuration file
  - SD/SD_ReadWrite_IT/Src/stm32h7xx_it.c          Interrupt handlers
  - SD/SD_ReadWrite_IT/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - SD/SD_ReadWrite_IT/Src/main.c                  Main program
  - SD/SD_ReadWrite_IT/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - SD/SD_ReadWrite_IT/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ devices.

  - This example has been tested with STM32H7B3I-EVAL  board and can be
    easily tailored to any other supported device and development board.

   STM32H7B3I-EVAL  Set-up :
  - Connect the uSD Card to the Micro SD connector (CN28).
  - Make sure JP12 and JP15 are on SDIO1 position.

  - You can display test message on the HyperTerminal,
      * You need to connect the STM32H7B3I-EVAL  board through 'USB micro A-Male to A-Male'
        cable to the ST-LINK V3.0 connector to the PC serial port.
      * Make sure that jumper JP20 and JP21 are fitted.
      * Configure your HyperTerminal with UART configuration:
        - Data Length = 7 Bits
        - One Stop Bit
        - Odd parity
        - BaudRate = 9600 baud
        - Flow control: None


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files (Optimize for Size: -Os) and load your image into target memory
 - Plug the SD Card in the STM32H7B3I-EVAL 
 - Run the example


 */
