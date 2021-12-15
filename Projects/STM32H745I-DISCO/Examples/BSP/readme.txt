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
available on the board by switching between all tests using key button. 

 ** Push the User button to start first Test.  
Green Led (LED1) will blink between each test.Press user key to start another test:

 ** Touchscreen demo1 **
Use touchscreen functionality to select or activate colored circle inside a rectangle.

 ** Touchscreen demo2 **
Use touchscreen functionality to select 2 touch points, their positions will be
captured and displayed on the LCD.

 ** LCD **
This example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.


 ** AUDIO PLAY **
This example shows how to play an audio file using the DMA circular mode and 
how to handle the buffer update.
Plug a headphone to hear the sound 
@Note: Copy file 'audio_sample_tdm.bin' (available in Binary) directly in 
       the flash at @0x08080000 using STM32CubeProgrammer utility

 ** AUDIO RECORD **
This example shows how to record an audio stream from microphnes mounted on STM32H745I-DISCO
and play it using the DMA circular mode and how to handle the buffer update.
Plug a headphone to hear the sound.  

** MMC **
This example shows how to erase, write and read the MMC. 

 ** SDRAM **
This example shows how to erase, write and read the SDRAM memory.

 ** SDRAM DMA **
This example shows how to erase, write and read the SDRAM memory in DMA mode.

 ** QSPI test **
This example provides of how to write, read and buffers compare for external 
flash memory using QSPI communication.

@Note For the Cortex-M7, if the application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex-M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

BSP, Board support package, DMA, Audio play, Headphone, Audio record, microphone, Touchscreen, LCD, LTDC, DMA2D, MDMA,
MMC card, SRAM, SDRAM, QSPI, QUADSPI, Display, LED, Button

@par Directory contents 

  - BSP/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration source file 

  - BSP/CM7/Inc/stm32h7xx_hal_conf.h   HAL Configuration file for Cortex-M7
  - BSP/CM7/Inc/stm32h7xx_it.h         Interrupt handlers header file for Cortex-M7
  - BSP/CM7/Inc/main.h                 Main program header file for Cortex-M7
  - BSP/CM7/Inc/stlogo.h               Image used for BSP example

  - BSP/CM7/Src/stm32h7xx_it.c         Interrupt handlers for Cortex-M7  
  - BSP/CM7/Src/main.c                 Main program for Cortex-M7
  - BSP/CM7/Src/stm32h7xx_hal_msp.c    HAL MSP module for Cortex-M7   
  - BSP/CM7/Src/lcd.c                  LCD drawing features
  - BSP/CM7/Src/log.c                  LCD Log firmware functions
  - BSP/CM7/Src/audio_play.c           Play audio 
  - BSP/CM7/Src/audio_record.c         Record and Play audio
  - BSP/CM7/Src/sdram.c                SDRAM Read/Write features
  - BSP/CM7/Src/sdram_dma.c            SDRAM Read/Write DMA mode
  - BSP/CM7/Src/qspi.c                 QUADSPI Read/Write features   

  - BSP/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M4
  - BSP/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M4
  - BSP/CM4/Inc/main.h                  Main configuration file for Cortex-M4
  - BSP/CM4/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M4
  - BSP/CM4/Src/main.c                  Main program  for Cortex-M4
  - BSP/CM4/Src/stm32h7xx_hal_msp.c     HAL MSP module for Cortex-M4 
  
@par Hardware and Software environment  

  - This example runs on STM32H745xx device.
    
  - This example has been tested with STMicroelectronics STM32H745I-DISCO 
    and can be easily tailored to any other supported device and development board.
    
 
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H745I_DISCO_CM7 and STM32H745I_DISCO_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - Run the example


 */
