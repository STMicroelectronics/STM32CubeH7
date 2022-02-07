/**
  @page SD_ReadWrite_DMA_HS  Description of the SD_ReadWrite_DMA_HS example
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
  */

@par Example Description
This example shows how to support DMA mode for micro SD Card.
This example performs some write and read transfers to UHS-I micro SD Card with SDMMC IP internal
DMA mode and calculate write and read transfer speed.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
The CPU at 280MHz
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 140MHz.

The transfer clock is set at 90MHz (SDMMC_CLK / Clock_DIV * 2) are configured within the
SD Handle init parameters.
After initialization, the driver check SD Card capability and switch to Ultra High Speed mode if
supported. Next, a write transfer of 100MB to SD Card is performed to calculate the write speed.
Then with the same parameters, a read transfer of 100MB from SD Card is performed to calculate
read speed.
The transfers of write and read are performed with the SDMMC IP internal DMA,
no configuration by user is needed for this DMA.
The operation of writing and reading is performed in infinity loop to measure the
SD card performance.


STM32H7B3I-EVAL board's LEDs can be used to monitor the transfer status:
 - LED_Orange is Toggle when transfer is on going (read or write).
 - LED_Green is ON when write and read transfer is completed and check buffers are OK.
 - LED_Red is ON when there is an error in transfer process or initialization.


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
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
              Please refer to the AN4839 “Level 1 cache on STM32F7 Series and STM32H7 Series”

@par Keywords

Storage, SD, SDMMC, Read Write, Block, sector, DMA, High Speed, HS, DelayBlock

@par Directory contents

  - SD/SD_ReadWrite_DMA_HS/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - SD/SD_ReadWrite_DMA_HS/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - SD/SD_ReadWrite_DMA_HS/Inc/main.h                  Header for main.c module
  - SD/SD_ReadWrite_DMA_HS/Src/stm32h7xx_it.c          Interrupt handlers
  - SD/SD_ReadWrite_DMA_HS/Src/main.c                  Main program
  - SD/SD_ReadWrite_DMA_HS/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - SD/SD_ReadWrite_DMA_HS/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ devices.

  - This example has been tested with STM32H7B3I-EVAL board and can be
    easily tailored to any other supported device and development board.
  - An SD card is need to be plugged to the uSD2 connector in Eval Board.

  - You can display test message  on the HyperTerminal,
      * You need to connect a null-modem female/female RS232 cable between the DB9 connector (CN9)
        (USART) and PC serial port.
      * Configure your HyperTerminal with UART configuration:
        - Data Length = 7 Bits
        - One Stop Bit
        - Parity none
        - BaudRate = 9600 baud
        - Flow control: None

  - Make sure that jumpers are placed in the right position :
          * JP1 in SDIO2 position
          * JP4 in SDIO2 position
          * JP2 in SDIO2 position
	      * JP13 fitted
	      * JP17 in SDIO2 position
	      * JP23 in SDIO2 position
		
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Plug a MicroSD Card in Eval board
 - Run the example


 */
