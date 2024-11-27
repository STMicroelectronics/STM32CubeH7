/**
  @page BSP  Description of the BSP example

  @verbatim
  ******************************************************************************
  * @file    Examples/BSP/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
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

This projects is configured for STM32H7B3xxQ devices using STM32CubeH7 HAL and running on
STM32H7B3I-DK board from STMicroelectronics. It provides a description of how to use
the different BSP drivers.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
The CPU at 280MHz
The HCLK for CD Domain AXI and AHB3 peripherals , CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 140MHz.

This example shows how to use the different functionalities of components
available on the board by switching between all tests using User button.

The program starts by turning ON GREEN, YELLOW, ORANGE and RED LEDs.

 ** Push the User button to start first Test.

 ** LCD TEST **
This example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

 ** TOUCHSCREEN DEMO1 **
Use touchscreen functionality to select or activate colored circle inside a rectangle.

 ** TOUCHSCREEN DEMO2 **
Use Multi-Touch functionality to draw 2 touch points, their positions will be
captured and displayed on the LCD.

 ** TOUCHSCREEN DEMO3 **
Use touchscreen interrupt mode detection functionality to draw a rectangle with
a random color.

 ** OCTO-SPI TEST **
Erase, Write and read Octo-SPI memory in:
- SPI or OPI modes
- STR or DTR transfer rate
- Indirect or Memory Mapped read

 ** AUDIO PLAY **
This example shows how to play an audio file using the DMA circular mode and
how to handle the buffer update.
Plug a headphone to CN5 to hear the sound
@Note:
   + Comment #define USE_SAI_INSTANCE in audio_play.c file

 ** AUDIO LOOPBACK SINGLE-BUFFERING **
This example shows how to record and play in loopback an audio stream using DFSDM
for recording and SAI for playing. The data are processed internaly within BSP
audio drivers and returned to user in PCM filtered format.
The DMA is configured in circular mode to play audio stream.

Plug a headphone to CN5 to hear the sound

 ** AUDIO LOOPBACK MULTI-BUFFERING **
This example shows how to record and play in loopback an audio stream using DFSDM
for recording and SAI for playing. The data are processed at the application level
based on acquisition transfer completion status returned from BSP audio drivers.
The data are shifted to remove information data (the 8 LSB of each word) then a
saturation is applied to get final PCM filtered format.
The DMA is configured in circular mode to play audio stream.

Plug a headphone to CN5 to hear the sound

** SD POLLING TEST **
This example shows how to erase, write and read the SD1 card in polling mode and
also how to detect the presence of the card in IT mode.

** SD DMA TEST **
This example shows how to erase, write and read the SD1 card in DMA mode and
also how to detect the presence of the card in IT mode.

** SD IT TEST **
This example shows how to erase, write and read the SD1 card in IT mode and
also how to detect the presence of the card in IT mode.

 ** SDRAM TEST **
This example shows how to erase, write and read the SDRAM memory.

 ** EEPROM TEST **
This example show how to read and write data in EEPROM I2C M24LR64 connected on STM32H7B3I-DK
Plug an I2C RF EEPROM memory (M24LR64) on CN16 connector

@note Some code parts can be executed in the ITCM-RAM (64 KB) which decrease critical task execution time, compared
      to code execution from Flash memory. This feature can be activated using '#pragma location = ".itcmram"' to be
      placed above function declaration, or using the toolchain GUI (file options) to execute a whole source file in
	  the ITCM-RAM.

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
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case, it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

BSP, Board support package, DMA, Polling, Interruption, Audio play, Headphone, Audio record, SAI, DFSDM, microphone, Touchscreen,
LCD, LTDC, SD card, EEPROM, SDRAM, OSPI, OCTOSPI, Display, LED, Button

@par Directory contents

  - BSP/Src/main.c                         Main program
  - BSP/Src/system_stm32h7xx.c             STM32H7xx system clock configuration file
  - BSP/Src/stm32h7xx_it.c                 Interrupt handlers
  - BSP/Src/lcd.c                          LCD drawing features
  - BSP/Src/sd.c                           SD features
  - BSP/Src/eeprom.c                       EEPROM Read/Write features
  - BSP/Src/audio_play.c                   Play audio test
  - BSP/Src/sdram.c                        SDRAM Read/Write features
  - BSP/Src/octospi.c                      OCTOSPI Read/Write features
  - BSP/Src/touchscreen.c                  Touch Screen features
  - BSP/Src/audio_loopback.c               Audio record & play features
  - BSP/Inc/main.h                         Main program header file
  - BSP/Inc/stm32f7xx_it.h                 Interrupt handlers header file
  - BSP/Inc/stm32h7b3i_discovery_conf.h    STM32H7B3I-DK BSP configuration file
  - BSP/Inc/stm32h7xx_hal_conf.h           HAL Configuration file
  - BSP/Inc/is42s16800j_conf.h             IS42S16800J SDRAM memory configuration file
  - BSP/Inc/ft5336_conf.h                  FT5336 TouchScreen configuration file
  - BSP/Inc/mx25lm51245g_conf.h            MX25LM51245G OSPI memory configuration file
  - BSP/Inc/stlogo.h                       Image used for BSP example
  - BSP/Inc/audio_16khz_wav.h              Audio stream used for audio play example

@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ devices.

  - This example has been tested with STMicroelectronics STM32H7B3I-DK rev B
    board and can be easily tailored to any other supported device
    and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example




 */
