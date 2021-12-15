/**
  @page Audio_playback_and_record  Audio Playback and Record application

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Audio/Audio_playback_and_record/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Audio Playback and Record application
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

@par Application Description 

This application shows how:
  - To use the different functionalities of the SAI (Serial Audio Interface) 
    to ensure audio record and playback via ST MEMS microphones (MP34DT01).
  - To use the touch screen to display wave files stored in the USB Key then 
    playback them or record a new wave file via a graphical menu.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

====================
= Application Menu =
====================
1) Explorer Audio File menu:
This menu is used to display wave files stored in the USB Key. Touch the screen 
anywhere, except play or record buttons, to explore the content of the USB key

2) Audio playback menu (Need headphone):
In playback menu, any wave file stored under the USB Key can be opened using the FatFs 
file system and transferred to the internal SRAM using the file system. All the wave
files properties are read from the Wave File Header.
Plug a headphone to ear the sound  /!\ Take care of yours ears. Default volume is 70%.

To play the audio file, a buffer is used between USB and audio codec. This buffer 
is used like a ping-pong buffer to be able to read regularly data from USB to be 
sent to audio codec: 
- Using FatFs driver, a 1st read of AUDIO_BUFFER_SIZE bytes is done
- This buffer is sent to audio BSP which uses DMA mode to transfer data from MCU
to audio codec (16 bits)
- At DMA half transfer ISR, an indication is sent to application to indicate that
half of the buffer has been sent
- At DMA half transfer ISR exit, application can read (AUDIO_BUFFER_SIZE/2) bytes from USB to fill
the 1st part of the buffer
- At DMA transfer complete ISR, DMA transfer is requested to send 1st part of the
buffer
- At DMA transfer complete ISR exit, application can read (AUDIO_BUFFER_SIZE/2) bytes from USB to fill
the 2nd part of the buffer
- When file size is reached, audio codec is stopped and a new play is requested.

A touch screen interface is used to navigate from a file to another (Next and Previous buttons), 
to change volume, pause playback and to quit playback menu by pressing Stop button.

Note: The audio files provided under "/Utilities/Media/Audio" are based on a free 
music download from www.DanoSongs.com website and user can load his own audio 
(*.wav) files in the USB Key root.
 
3) Audio recorder menu:
Two MEMS microphone MP34DT01 mounted on STM32H743I-EVAL are connected
to the DFSDM peripheral via SW2 swith. So be careful to put SW2 swith 
to the DFSDM position before starting the record.
The DFSDM Filters/Cannels are configured to get data from
the MEMS microphones. Once recorded, the data can be played using SAI periphiral.

Data acquisition is performed in 16KHz and 16-bit and DMA is used to transfer 
data from DFSDM peripheral to internal SRAM. In order to avoid data-loss, one buffer
split into two halves is used:

When the first half of the buffer is filled (half transfer complete), the recorded PCM
samples are written to USB Key while the second half of the buffer is being
used to receive data from Audio samples.

When the second half of the buffer is filled (transfer complete), the recorded PCM
samples are written to USB Key while the first half of the buffer is being
used to receive data from Audio samples. This process is repeated as long as the 
application is running.

To avoid data corruption:
- DMA ISR priority must be set at a Lower priority than USB interrupt sources,
  this priority order must be respected when managing other interrupts;

In main loop of the application, when a buffer is full with PCM format it is written
in USB key using the free file system FatFS.

A touch screen interface is used to launch record process, pause record and to quit 
record menu by pressing Stop button.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

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

Audio, DFSDM, SAI, USB, DMA, Playback, Record, MEMS, Microphone, Touch screen, WAV, FatFS, SRAM, audio codec,


@par Directory contents 

  - Audio_playback_and_record/Src/explorer.c           Main explorer functions
  - Audio_playback_and_record/Src/main.c               Main program
  - Audio_playback_and_record/Src/menu.c               Menu file 
  - Audio_playback_and_record/Src/stm32h7xx_it.c       Interrupt handlers
  - Audio_playback_and_record/Src/system_stm32h7xx.c   STM32H7xx system source file
  - Audio_playback_and_record/Src/usbh_conf.c          Main functions to configure USB 	
  - Audio_playback_and_record/Src/usbh_diskio.c        USB Host Disk I/O driver 	
  - Audio_playback_and_record/Src/waveplayer.c         Audio play file
  - Audio_playback_and_record/Src/waverecorder.c       Audio record file
 
  - Audio_playback_and_record/Inc/ffconf.h             FAT file system module configuration file
  - Audio_playback_and_record/Inc/main.h               Main program header file
  - Audio_playback_and_record/Inc/stm32h7xx_hal_conf.h HAL configuration file
  - Audio_playback_and_record/Inc/stm32h7xx_it.h       Interrupt handlers header file
  - Audio_playback_and_record/Inc/usbh_conf.h          USB driver Configuration file
  - Audio_playback_and_record/Inc/usbh_diskio.h        USB Disk I/O driver header file  
  - Audio_playback_and_record/Inc/waveplayer.h         Audio play header file
  - Audio_playback_and_record/Inc/waverecorder.h       Audio record header file
      
      
@par Hardware and Software environment

  - This application runs on STM32H743xx devices.
    
  - This application has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board.   

  - STM32H743I-EVAL Set-up
    - Plug the USB key into the STM32H743I-EVAL board through 'USB micro A-Male 
      to A-Female' cable (HS mode: connector CN14).
    - Plug a headphone on CN17 connector (Headset).
    - Make sure that SW2 switch is in the DFSDM position.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
 

 */
