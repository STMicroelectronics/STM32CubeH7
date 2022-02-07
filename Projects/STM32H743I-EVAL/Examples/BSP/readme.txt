/**
  @page BSP  Example on how to use the BSP drivers

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    BSP/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
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

@par Example Description

This example provides a description of how to use the different BSP drivers.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

This example shows how to use the different functionalities of components
available on the board by switching between all tests using Tamper push-button.

 ** Push the Tamper push-button to start first Test.
Green Led (LED1) will blink between each test. Press Tamper push-button to start another test:

 ** AUDIO PLAY **
This example shows how to play an audio file using the DMA in circular mode and
how to handle the buffer update.
Plug a headphone to the headset connector (CN17)  to hear the sound.
@Note: Copy file 'audio_sample_tdm.bin' (available in Binary) directly in
       the flash at @0x08080000 using ST-Link utility.

 ** AUDIO RECORD **
This example shows how to record an audio stream from microphones mounted on STM32H743I-EVAL
and play it using the DMA circular mode and how to handle the buffer update.
Plug a headphone to the headset connector (CN17)  to hear the sound.

 ** JOYSTICK **
Use the joystick button to move a pointer inside a rectangle
(up/down/right/left) and change the pointer color(select).

 ** Touchscreen **
Use touchscreen functionality to select or activate colored circle inside a rectangle.
A calibration is needed when using MB1046.

 ** LCD **
This example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

** SD **
This example shows how to erase, write and read the SD card and also
how to detect the presence of the card.

 ** EEPROM **
This example show how to read and write data in EEPROM I2C M24LR64 connected on STM32H743I-EVAL
   * The I2C RF EEPROM memory (M24LR64) is available through the connector CN4

 ** NOR **
This example shows how to erase, write and read the NOR memory.

 ** SRAM **
This example shows how to erase, write and read the SRAM memory.

 ** SRAM DMA **
This example shows how to erase, write and read the SRAM memory in DMA mode.

 ** SDRAM **
This example shows how to erase, write and read the SDRAM memory.

 ** SDRAM DMA **
This example shows how to erase, write and read the SDRAM memory in DMA mode.

 ** QSPI test **
This example provides of how to write, read and buffers compare for external
flash memory using QSPI communication.

** Potentiometer **
This example shows how to get the potentiometer’s voltage level.

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

BSP, Board support package, DMA, Audio play, Headphone, Audio record, microphone, Touchscreen, Joystick, LCD, LTDC, DMA2D, MDMA,
SD Card, NOR, SRAM, SDRAM, EEPROM, Potentiometer, LOG, QSPI, QUADSPI, HyperRAM, Display, LED, Button

@par Directory contents

  - BSP/Src/main.c                 Main program
  - BSP/Src/system_stm32h7xx.c     STM32H7xx system clock configuration file
  - BSP/Src/stm32h7xx_it.c         Interrupt handlers
  - BSP/Src/joystick.c             joystick feature
  - BSP/Src/lcd.c                  LCD drawing features
  - BSP/Src/log.c                  LCD Log firmware functions
  - BSP/Src/eeprom.c               EEPROM Read/Write features
  - BSP/Src/audio_play.c           Play audio
  - BSP/Src/audio_record.c         Record and Play audio
  - BSP/Src/sram.c                 SRAM Read/Write features
  - BSP/Src/sdram.c                SDRAM Read/Write features
  - BSP/Src/sdram_dma.c            SDRAM Read/Write DMA mode
  - BSP/Src/nor.c                  NOR Read/Write features
  - BSP/Src/qspi.c                 QUADSPI Read/Write features
  - BSP/Src/potentiometer.c        potentiometer features
  - BSP/Src/sd.c                   SD Erase/Read/Write features
  - BSP/Src/touchscreen.c          touchscreen features
  - BSP/Src/ts_calibration.c       calibration of the touchscreen
  - BSP/Inc/main.h                 Main program header file
  - BSP/Inc/stm32h7xx_hal_conf.h   HAL Configuration file
  - BSP/Inc/stm32h7xx_it.h         Interrupt handlers header file
  - BSP/Inc/stlogo.h               Image used for BSP example

@par Hardware and Software environment

  - This example runs on STM32H743xx device.

  - This example has been tested with STMicroelectronics STM32H743I-EVAL
    evaluation boards and can be easily tailored to any other supported device
    and development board.

  - STM32H743I-EVAL Set-up :
   if using MB1063 (AMPIRE 640x480 LCD ) :
   - Make sure that JP10 is set to PSU position.
   - Make sure that the board is powered using an external 5v power supply through CN10.
   For NOR Module:
    - Make sure that JP13 is fitted.
   For SD module:
    - Use a UHS-I usD card (Ultra High Speed usD Card)

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
