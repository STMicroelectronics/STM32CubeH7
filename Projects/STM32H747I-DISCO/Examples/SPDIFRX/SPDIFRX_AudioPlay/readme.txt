/**
  @page SPDIFRX_AudioPlay SPDIFRX Audio Play example
  
  @verbatim
  ******************************************************************************
  * @file    SPDIFRX/SPDIFRX_AudioPlay/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the SPDIFRX Audio Play example.
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

This example shows how to use the SPDIFRX HAL APIs to receive audio data then play
them through the codec by using the SAI interface.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.

The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz 
  - The Cortex-M4 at 200MHz.
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

In this example two boards are used to send and receive audio data by using the 
SPDIF RX/TX interfaces:
  - SPDIFRX STM32H747I_Discovery Board: This is the mainly used board for our example.
    It is used to receive SPDIF audio data via the SPDIFRX interface.
    Once started, LED1 is toggling.
  - SPDIFTX STM32F769I_Discovery Board: This board is used to send SPDIF audio data
    via the SPDIFTX interface. Once started, LED2 is toggling.
    To use this board the user must load two binary files 
    in the flash memory:
      1- audio.bin file: exist under Bin directory. This file contains the audio 
         data to send. Copy this file directly in the flash at @0x08080000 by 
         using ST-Link utility.
      2- SPDIFTX_STM32F769I_Discovery.hex file: exist under Bin directory. 
         This file represent the SPDIFTX example in hex format. Copy this file  
         directly in the flash at @0x08000000 by using ST-Link utility.
         
To start the example:
  - First connect the two boards via a RCA cable.
  - Second plug a headset on CN11 connector of the SPDIFRX STM32H747I_Discovery 
    board to ear the sound.
  - Third reset the SPDIFTX board.
  - Finally reset the SPDIFRX board.
The audio data will be played in loop.

SPDIFTX Board (STM32F769I_Discovery): SPDIF RCA output connector CN8
SPDIFRX Board (STM32H747I_Discovery): SPDIF RCA input connector CN3

In STM32F769I_Discovery two LEDs are used to monitor the data transfer's status:
  - LED2 (GREEN) toggle during buffer transmission.
  - LED1 (RED) is ON when a communication or an initialization error occurs.
  
In STM32H747I_Discovery two LEDs are used to monitor the data transfer's status:
  - LED1 (GREEN) toggle during buffer reception.
  - LED3 (RED) is ON when a communication or an initialization error occurs.
   _________________________                       _________________________ 
  |           ______________|                     |______________           |
  |          |SPDIFTX       |                     |       SPDIFRX|          |
  |          |              |                     |              |          |
  |          |           TX |_____________________| RX           |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |______________|                     |______________|          |
  |                         |                     |                         |
  |                      GND|_____________________|GND                      |
  |_STM32F769I_Discovery____|                     |_STM32H747I_Discovery____|


@par Keywords

Audio, SPDIFTX, SPDIFRX, IEC-60958, IEC-61937, SAI, Codec.

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

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 


  - SPDIFRX/SPDIFRX_AudioPlay/Common/Src/system_stm32h7xx.c   STM32H7xx system configuration source file 
  
  - SPDIFRX/SPDIFRX_AudioPlay/CM7/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - SPDIFRX/SPDIFRX_AudioPlay/CM7/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - SPDIFRX/SPDIFRX_AudioPlay/CM7/Inc/main.h                  Header for main.c module  
  - SPDIFRX/SPDIFRX_AudioPlay/CM7/Src/stm32h7xx_it.c          Interrupt handlers
  - SPDIFRX/SPDIFRX_AudioPlay/CM7/Src/main.c                  Main program
  - SPDIFRX/SPDIFRX_AudioPlay/CM7/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - SPDIFRX/SPDIFRX_AudioPlay/CM7/Src/system_stm32h7xx.c      STM32H7xx system source file
  
  - SPDIFRX/SPDIFRX_AudioPlay/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - SPDIFRX/SPDIFRX_AudioPlay/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - SPDIFRX/SPDIFRX_AudioPlay/CM4/Inc/main.h                  Header for main.c module  
  - SPDIFRX/SPDIFRX_AudioPlay/CM4/Src/stm32h7xx_it.c          Interrupt handlers
  - SPDIFRX/SPDIFRX_AudioPlay/CM4/Src/main.c                  Main program
  - SPDIFRX/SPDIFRX_AudioPlay/CM4/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - SPDIFRX/SPDIFRX_AudioPlay/CM4/Src/system_stm32h7xx.c      STM32H7xx system source file

@par Hardware and Software environment

  - This example runs on STM32H747xx/STM32H757xx devices.
    
  - This example has been tested with STM32H747I-DISCO board and can be
    easily tailored to any other supported device and development board.
    
  - STM32H747I_Discovery Set-up :
    - Connect SPDIFRX (CN3) to the SPDIFTX (CN8) of the STM32F769I_Discovery board
      with a RCA cable.    
    - Plug a headset on CN11 connector.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H747I_DISCO_CM7 and STM32H747I_DISCO_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example


 */
