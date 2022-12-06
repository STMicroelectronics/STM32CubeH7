/**
  @page DFSDM_AudioRecord  Description of the DFSDM audio record example
  
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    DFSDM/DFSDM_AudioRecord/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DFSDM audio record example.
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

This example shows how to use the DFSDM HAL API to perform stereo audio recording.
For this audio record we will use two MP34DT01 digital microphones mounted on the board. 

CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) will release CPU2 (Cortex-M4) when needed
by means of HSEM notification but release could be ensured by any Domain D2 wakeup source (SEV,EXTI..).


CPU1 (Cortex-M7) will configure the DFDSM peripheral while CPU2(Cortex-M4) is in stop mode:

MP34DT01 runs with a clock frequency from 1MHz to 3,25MHz.
This clock frequency has to be delivered by DFSDM. So first channel of DFSDM will be
configured to enable output clock generation, to choose audio clock as an output clock source,
and output clock divider will be set to generate an output clock in MP34DT01 frequency clock range.
Audio clock will be configured to 11.294MHz, so DFSDM output clock divider will be set to 4.

The digital audio outputs from the microphones are coded in PDM (Pulse Density Modulation) and 
interlaced to use the same data wire connected to PD6. When CLK = 0, the left channel is sent
on PD6 and when CLK = 1, the right channel is sent on PD6.

PD6 can be configured as DFSDM_DATIN1 (DFSDM channel 1 external data input).
We will use DFSDM channel 1 to get left channel. DFSDM channel 1 will be configured to take
inputs from external serial inputs and from pins of channel 1. DFSDM channel 1 will also be 
configured to operate in SPI mode with rising edge and with internal clock.
We will use also DFSDM channel 0 to get right channel. DFSDM channel 0 will be configured to take
inputs from external serial inputs and from pins of channel 1. DFSDM channel 0 will also be 
configured to operate in SPI mode with falling edge and with internal clock.

DFSDM filter 0 will be used to perform left channels conversions.
We will use a SINC3 filter with oversampling at 64 and integrator oversampling at 1.
Oversampling is set to 64 in order to have an audio sample frequency at 44.1KHz (11.294MHz/(4*64)).
Regular conversions in fast mode will be performed in DMA mode on DFSDM filter 0 in order to fill
a buffer containing left audio samples at 44.1KHz.
DFSDM filter 1 will be used to perform right channels conversions.
We will use a SINC3 filter with oversampling at 64 and integrator oversampling at 1.
Oversampling is set to 64 in order to have an audio sample frequency at 44.1KHz (11.294MHz/(4*64)).
Regular conversions in fast mode and synchronized with DFSDM filter 0 regular conversions will be 
performed in DMA mode on DFSDM filter 1 in order to fill a buffer containing right audio samples at 44.1KHz.

Playback of the recorded data will be performed on headset using HAL_SAI and WM8994 audio codec.
A circular playback buffer will be filled as soon as recorded data are available. 
When half left and right buffers will be filled, we put first parts of left and right channels data 
on the first half of playback buffer. 
When left and right buffers will be full filled, we put second parts of left and right channels data 
on the second half of playback buffer.

STM32H747I-EVAL board's LEDs are used to monitor the example status:
  - LED3 toggle with a period of one second when  there is an error occurred.


This projects is configured for STM32H747xx devices using STM32CubeH7 HAL and running on 
STM32H747I-EVAL board from STMicroelectronics.

The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz 
  - Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals 
    and D3 Domain APB4 peripherals to run at 100MHz.


@note For Cortex-M7, Some code parts (for instance ISR routines, vector table, critical routines )
      can be executed from the ITCM-RAM (64 KB) allowing zero wait state access.
      Which decreases critical task execution time compared to code execution from internal Flash memory. 
      This configuration can be done using '#pragma location = ".itcmram"' to be placed above function declaration, 
      or using the toolchain GUI (file options) to execute a whole source file in the ITCM-RAM.
      For fast data access, critical variables, application RAM, heap, stack could be put as well in the DTCM-RAM (128 KB).

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
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Audio, DFSDM, Audio record, Digital Microphone, Stereo, MP34DT01, DMA, PDM, Filter, Playback, Audio codec, SAI

@par Directory contents

  - DFSDM/DFSDM_AudioRecord/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration file
  
  - DFSDM/DFSDM_AudioRecord/CM7/Src/main.c                 Main program for Cortex-M7
  - DFSDM/DFSDM_AudioRecord/CM7/Src/stm32h7xx_it.c         Interrupt handlers for Cortex-M7
  - DFSDM/DFSDM_AudioRecord/CM7/Src/stm32h7xx_hal_msp.c    HAL MSP module for Cortex-M7
  - DFSDM/DFSDM_AudioRecord/CM7/Inc/main.h                 Main program header file for Cortex-M7  
  - DFSDM/DFSDM_AudioRecord/CM7/Inc/stm32h7xx_hal_conf.h   HAL Configuration file for Cortex-M7
  - DFSDM/DFSDM_AudioRecord/CM7/Inc/stm32h7xx_it.h         Interrupt handlers header file for Cortex-M7

  - DFSDM/DFSDM_AudioRecord/CM4/Src/main.c                 Main program for Cortex-M4
  - DFSDM/DFSDM_AudioRecord/CM4/Src/stm32h7xx_it.c         Interrupt handlers for Cortex-M4
  - DFSDM/DFSDM_AudioRecord/CM4/Src/stm32h7xx_hal_msp.c    HAL MSP module for Cortex-M4
  - DFSDM/DFSDM_AudioRecord/CM4/Inc/main.h                 Main program header file for Cortex-M4  
  - DFSDM/DFSDM_AudioRecord/CM4/Inc/stm32h7xx_hal_conf.h   HAL Configuration file for Cortex-M4
  - DFSDM/DFSDM_AudioRecord/CM4/Inc/stm32h7xx_it.h         Interrupt handlers header file for Cortex-M4


@par Hardware and Software environment

  - This example runs on STM32H747xx devices.
    
  - This example has been tested with STM32H747I-EVAL  board with SMPS (SD Converter) power supply config and can be
    easily tailored to any other supported device and development board.    

  - STM32H747I-EVAL Set-up :
    - Put the SW2 switch to DFSDM position.
    - Plug a headphone on CN17 connector (Headset).

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H747_EVAL_CM7 and STM32H747_EVAL_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example


 */
