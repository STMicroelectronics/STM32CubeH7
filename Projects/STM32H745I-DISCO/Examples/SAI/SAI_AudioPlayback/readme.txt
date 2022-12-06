/**
  @page SAI_AudioPlayback  Description of the SAI audio playback example
  
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    SAI/SAI_AudioPlayback/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the SAI audio playback example.
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

This example shows how to use the SAI to playback audio data coming from two Microphones.
In this example:
- The SAI4 PDM interface receives audio data coming from two Microphones using the
  BDMA in circular mode.
- The audio PDM to PCM library is used to convert audio PDM data to PCM format.
- The SAI1 transmits audio PCM data to the WM8994 Codec for playback using the DMA2 in 
  circular mode.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.

The SystemClock_Config() function is used to set the Flash latency and  to 
configure the system clock :
  - The Cortex-M7 at 400MHz 
  - The Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2  
    peripherals and D3 Domain APB4 peripherals to run at 100MHz.
   
Plug a headphone to hear the sound  /!\ Take care of yours ears. Default volume is 70%.
 
Once started, LED1 is on during playback. 
If an error occurred LED2 is turned ON.

@note Note the BDMA and the DMA run in circular buffer mode and never stop. If you break  
      with the debugger, they will keep running and a noise will be heard.

@note The Domain 3 BDMA has access to  Domain 3 SRAMs and peripherals only,  thus the
      audioPdmBuf buffer has been placed in the D3SRAM (@0x38000000)

@note The PDM to PCM library is used to convert PDM data to PCM format.

@note This example does not use the BSP_AUDIO driver so the MspInit is coded in the 
      stm32h7xx_hal_msp.c.

@par Keywords

Audio, SAI, DMA, Circular mode, conversion, PDM format, PCM format, audio protocol, playback, codec, WM8994, Buffer update, play, headphone

@par Directory contents  

  - SAI/SAI_AudioPlayback/CM7/Src/main.c                  Main program
  - SAI/SAI_AudioPlayback/CM7/Src/system_stm32h7xx.c      STM32H7xx system source file
  - SAI/SAI_AudioPlayback/CM7/Src/stm32h7xx_hal_msp.c     STM32H7xx MSP source file
  - SAI/SAI_AudioPlayback/CM7/Src/stm32h7xx_it.c          Interrupt handlers
  - SAI/SAI_AudioPlayback/CM7/Inc/main.h                  Main program header file
  - SAI/SAI_AudioPlayback/CM7/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - SAI/SAI_AudioPlayback/CM7/Inc/stm32h7xx_it.h          Interrupt handlers header file
	
  - SAI/SAI_AudioPlayback/CM4/Src/main.c                  Main program
  - SAI/SAI_AudioPlayback/CM4/Src/system_stm32h7xx.c      STM32H7xx system source file
  - SAI/SAI_AudioPlayback/CM4/Src/stm32h7xx_hal_msp.c     STM32H7xx MSP source file
  - SAI/SAI_AudioPlayback/CM4/Src/stm32h7xx_it.c          Interrupt handlers
  - SAI/SAI_AudioPlayback/CM4/Inc/main.h                  Main program header file
  - SAI/SAI_AudioPlayback/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - SAI/SAI_AudioPlayback/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file
  
@par Hardware and Software environment 

  - This example runs on STM32H747xx/STM32H745xx devices.
  
  - This example has been tested with STMicroelectronics STM32H745I-DISCO
    boards and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H745I_DISCO_CM7 and STM32H745I_DISCO_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) 
   and CPU2 (Cortex-M4) at once.
 - Run the example


 */
