/**
  @page BSP  Example on how to use the BSP drivers

  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    BSP/readme.txt
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

This example provides a description of how to use the different BSP drivers.

The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz
  - Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals
    and D3 Domain APB4 peripherals to run at 100MHz.

CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) could release CPU2 (Cortex-M4) when needed
by means of HSEM notification or by any D2 Domain wakeup source (SEV,EXTI..).

The above will guarantee that CPU2 (Cortex-M4) code execution starts after system initialization :
(system clock config, external memory configuration..).

After Domain D2 wakeup, if  CPU1 attempts to use any resource from such a domain,
the access will not be safe until “clock ready flag” of such a domain is set (by hardware).
The check could be done using this macro : __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY).

This example shows how to use the different functionalities of components
available on the board by switching between all tests using Tamper push-button.

 ** Push the Tamper push-button to start first Test.
Blue Led (LED4) will blink between each test. Press Tamper push-button to start another test:

 ** AUDIO PLAY **
This example shows how to play an audio file using the DMA in circular mode and
how to handle the buffer update.
Plug a headphone to the headset connector (CN17)  to hear the sound.
@Note: Copy file 'audio_sample_tdm.bin' (available in Binary) directly in
       the flash at @0x08080000 using STM32CubeProgammer.

 ** AUDIO RECORD **
This example shows how to record an audio stream from microphones mounted on STM32H747I-EVAL
and play it using the DMA circular mode and how to handle the buffer update.
Plug a headphone to the headset connector (CN17)  to hear the sound.

 ** JOYSTICK **
Use the joystick button to move a pointer inside a rectangle
(up/down/right/left) and change the pointer color(select).

 ** Touchscreen demo1 **
Use touchscreen functionality to select or activate colored circle inside a rectangle.

 ** Touchscreen demo2 **
Use touchscreen functionality to select 2 touch points, their positions will be
captured and displayed on the LCD.

 ** LCD **
This example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

** SD **
This example shows how to erase, write and read the SD card and also
how to detect the presence of the card.

 ** EEPROM **
This example show how to read and write data in EEPROM I2C M24LR64 connected on STM32H747I-EVAL
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
This example shows how to erase, write and read the SDRAM memory in MDMA mode.

 ** QSPI test **
This example provides of how to write, read and buffers compare for external
flash memory using QSPI communication.

** Potentiometer **
This example shows how to get the potentiometer’s voltage level.

@par Keywords

BSP, Board support package, DMA, Audio play, Headphone, Audio record, microphone, Touchscreen, Joystick, LCD, MDMA,
SD Card, SRAM, SDRAM, NOR, EEPROM, QSPI, QUADSPI, Potentiometer, Display, LED, Button

@par Directory contents

  - BSP/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration source file

  - BSP/CM7/Src/main.c                 Main program for cortex-M7
  - BSP/CM7/Src/stm32h7xx_it.c         Interrupt handlers for cortex-M7
  - BSP/CM7/Src/joystick.c             joystick feature
  - BSP/CM7/Src/lcd.c                  LCD drawing features
  - BSP/CM7/Src/eeprom.c               EEPROM Read/Write features
  - BSP/CM7/Src/audio_play.c           Play audio
  - BSP/CM7/Src/audio_record.c         Record and Play audio
  - BSP/CM7/Src/sram.c                 SRAM Read/Write features
  - BSP/CM7/Src/sdram.c                SDRAM Read/Write features
  - BSP/CM7/Src/sdram_dma.c            SDRAM Read/Write DMA mode
  - BSP/CM7/Src/nor.c                  NOR Read/Write features
  - BSP/CM7/Src/qspi.c                 QUADSPI Read/Write features
  - BSP/CM7/Src/potentiometer.c        potentiometer features
  - BSP/CM7/Src/sd.c                   SD Erase/Read/Write features
  - BSP/CM7/Src/touchscreen.c          touchscreen features
  - BSP/CM7/Inc/main.h                 Main program header file for cortex-M7
  - BSP/CM7/Inc/stm32h7xx_hal_conf.h   HAL Configuration file for cortex-M7
  - BSP/CM7/Inc/stm32h7xx_it.h         Interrupt handlers header file for cortex-M7
  - BSP/CM7/Inc/stlogo.h               Image used for BSP example

  - BSP/CM4/Src/main.c                 Main program for cortex-M4
  - BSP/CM4/Src/stm32h7xx_it.c         Interrupt handlers for cortex-M4
  - BSP/CM4/Inc/main.h                 Main program header file for cortex-M4
  - BSP/CM4/Inc/stm32h7xx_hal_conf.h   HAL Configuration file for cortex-M4
  - BSP/CM4/Inc/stm32h7xx_it.h         Interrupt handlers header file for cortex-M4

@par Hardware and Software environment

  - This example runs on STM32H747xx device.

  - This example has been tested with STM32H747I-EVAL  board with SMPS (SD Converter) power supply config and can be
    easily tailored to any other supported device and development board.

  - STM32H747I-EVAL Set-up :
   For NOR Module:
    - Make sure that JP13 is fitted.
   For SD module:
    - Use a UHS-I usD card (Ultra High Speed usD Card)

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - For each target configuration (STM32H747I_EVAL_CM7 and STM32H747I_EVAL_CM4) :
     - Rebuild all files
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example


 */
