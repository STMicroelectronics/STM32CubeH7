/**
  @page SAI_AudioPlayback  Description of the SAI audio playback example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    SAI/SAI_AudioPlayback/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SAI audio playback example.
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

This example shows how to use the SAI to playback audio data coming from two Microphones.
In this example:
- The SAI4 PDM interface receives audio data coming from two Microphones using the
  BDMA in circular mode.
- The audio PDM to PCM library is used to convert audio PDM data to PCM format.
- The SAI1 transmits audio PCM data to the WM8994 Codec for playback using the DMA2 in 
  circular mode.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
 - The CPU at 400MHz 
 - The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
   and D3 Domain AHB4  peripherals at 200MHz.
 - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals 
   and D3 Domain APB4 peripherals to  run at 100MHz
   
Plug a headphone to hear the sound  /!\ Take care of yours ears. Default volume is 60%.
 
Once started, LED1 is on during playback. 
If an error occurred LED3 is turned ON.

@note Note the BDMA and the DMA run in circular buffer mode and never stop. If you break  
      with the debugger, they will keep running and a noise will be heard.

@note The Domain 3 BDMA has access to  Domain 3 SRAMs and peripherals only,  thus the
      audioPdmBuf buffer has been placed in the D3SRAM (@0x38000000)

@note The PDM to PCM library is used to convert PDM data to PCM format.

@note This example does not use the BSP_AUDIO driver so the MspInit is coded in the 
      stm32h7xx_hal_msp.c.

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

Audio, SAI, DMA, Circular mode, conversion, PDM format, PCM format, audio protocol, playback, codec, WM8994, Buffer update, play, headphone

@par Directory contents  

  - SAI/SAI_AudioPlayback/Src/main.c                  Main program
  - SAI/SAI_AudioPlayback/Src/system_stm32h7xx.c      STM32H7xx system source file
  - SAI/SAI_AudioPlayback/Src/stm32h7xx_hal_msp.c     STM32H7xx MSP source file
  - SAI/SAI_AudioPlayback/Src/stm32h7xx_it.c          Interrupt handlers
  - SAI/SAI_AudioPlayback/Inc/main.h                  Main program header file
  - SAI/SAI_AudioPlayback/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - SAI/SAI_AudioPlayback/Inc/stm32h7xx_it.h          Interrupt handlers header file
	
@par Hardware and Software environment 

  - This example has been tested with STMicroelectronics STM32H743I-EVAL
    board and can be easily tailored to any other supported device
    and development board.      

  - STM32H743I-EVAL Set-up :
    - Put the SW2 switch to PDM position
    - Plug a headphone on CN17 connector (Headset). 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 


 */
