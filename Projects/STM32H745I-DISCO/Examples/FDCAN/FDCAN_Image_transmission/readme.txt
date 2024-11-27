/**
  @page FDCAN_Image_transmission Description of the FDCAN Image transmission example
  
  @verbatim
  ******************************************************************************
  * @file    FDCAN/FDCAN_Image_transmission/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FDCAN_Image_transmission example.
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

This example shows the gain in time obtained by the activation of the Bit Rate
Switching (BRS) feature.
The same image is transmitted two times via FDCAN as follow:
 - first, without Bit Rate Switching, at 1 MBits/s bit rate
 - then, with Bit Rate Switching activated, at 8 MBits/s data bit rate

   ________________________________________________
  |                                    ___________ |
  |                                   |FDCAN1     H|______
  |                                   |    CN11    |      |
  |  _____________________            | connector  |__    |
  | |                     |           |___________L|  |   |
  | |                     |                        |  |   |
  | |                     |               |--| JP7 |  |   |
  | |        LCD          |                        |  |   |
  | |                     |                        |  |   |
  | |                     |                        |  |   |
  | |_____________________|               |--| JP6 |  |   |
  |   _                _               ___________ |  |   |
  |  |_|              (_)             |FDCAN2     L|__|   |
  | LED2              User            |    CN10    |      |
  |                                   | connector  |      |
  |               STM32H745I-DISCO    |___________H|______|
  |________________________________________________|


FDCAN1 and FDCAN2 instances are respectively managed by CPU2 (Cortex-M4) while CPU1 (Cortex-M7).

System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) will release CPU2 (Cortex-M4) when needed
by means of HSEM notification but release could be ensured by any Domain D2 wakeup source (SEV,EXTI..).

The above will guarantee that CPU2 (Cortex-M4) code execution starts after system initialization :
(system clock config, external memory configuration..).

After Domain D2 wakeup, if  CPU1 attempts to use any resource from such a domain, 
the access will not be safe until “clock ready flag” of such a domain is set (by hardware). 
The check could be done using this macro : __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY).

This projects is configured for STM32H745xx devices using STM32CubeH7 HAL and running on 
STM32H745I-DISCO board from STMicroelectronics.

The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz 
  - Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals 
    and D3 Domain APB4 peripherals to run at 100MHz.

Then:
 - Reception filters and Synchronization message are configured.
 - Tx frames are configured to be sent without Bit Rate Switching.
 - FDCAN modules are started.
 - On CPU1 (Cortex-M7) side, main program waits for User push-button to be
   pressed to send Transmission request to CPU2 (Cortex-M4).
 - On CPU2 (Cortex-M4) side, main program waits for Reception of Transmission
   request message to begin sending image data by blocks of 16 messages (maximum
   length of the Tx FIFO).

First step (No BRS):
  When User push-button is pressed, a Transmission request is sent to inform
  that FDCAN2 it is ready to receive data.
  Then, FDCAN1 sends first 16 blocks (messages) of image data and waits for
  acknowledgment of receipt to send next 16 blocks.
  On the FDCAN2 side, every time 16 messages are received and payload retrived,
  the acknowledgment is automatically sent to FDCAN1.
  This process ends when the full image is transferred. It is then displayed on
  the LDC via DMA2D as well as the whole measured communication time.

Second step (BRS activated):
  Tx frames are reconfigured to be sent with Bit Rate Switching activated.
  The same process as step 1 is repeated.

If an error is encountered LED2 is turned ON.

@note The measured communication time includes:
       - Adding messages to Tx FIFO
       - Transmission (propagation time)
       - Retrieving messages from Rx FIFO
       - Sending acknowledgment of receipt every 16 messages

@Note For Cortex-M7, Some code parts (for instance ISR routines, vector table, critical routines )
      can be executed from the ITCM-RAM (64 KB) allowing zero wait state access.
      Which decreases critical task execution time compared to code execution from internal Flash memory. 
      This configuration can be done using '#pragma location = ".itcmram"' to be placed above function declaration, 
      or using the toolchain GUI (file options) to execute a whole source file in the ITCM-RAM.
      For fast data access, critical variables, application RAM, heap, stack could be put as well in the DTCM-RAM (128 KB). 

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

@Note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@Note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

FDCAN,  multiprocessor, Bit Rate Switching, BRS, Image, Transmit, Receive, Timing, Button

@par Directory contents 

  - FDCAN/FDCAN_Image_transmission/Common/Src/system_stm32h7xx.c  STM32H7xx system configuration file
  - FDCAN/FDCAN_Image_transmission/Common/Src/ft5336_conf.h       Configurations file for ft5336
  - FDCAN/FDCAN_Image_transmission/Common/Inc/mt48lc4m32b2_conf.h Configurations file for mt48lc4m32b2

  - FDCAN/FDCAN_Image_transmission/CM7/Src/main.c                 Main program for Cortex-M7
  - FDCAN/FDCAN_Image_transmission/CM7/Src/stm32h7xx_it.c         Interrupt handlers for Cortex-M7
  - FDCAN/FDCAN_Image_transmission/CM7/Src/stm32h7xx_hal_msp.c    HAL MSP module for Cortex-M7
  - FDCAN/FDCAN_Image_transmission/CM7/Inc/main.h                 Main program header file for Cortex-M7  
  - FDCAN/FDCAN_Image_transmission/CM7/Inc/stm32h7xx_hal_conf.h   HAL Configuration file for Cortex-M7
  - FDCAN/FDCAN_Image_transmission/CM7/Inc/stm32h7xx_it.h         Interrupt handlers header file for Cortex-M7
  
  - FDCAN/FDCAN_Image_transmission/CM4/Src/main.c                 Main program for Cortex-M4
  - FDCAN/FDCAN_Image_transmission/CM4/Src/stm32h7xx_it.c         Interrupt handlers for Cortex-M4
  - FDCAN/FDCAN_Image_transmission/CM4/Src/stm32h7xx_hal_msp.c    HAL MSP module for Cortex-M4
  - FDCAN/FDCAN_Image_transmission/CM4/Inc/main.h                 Main program header file for Cortex-M4  
  - FDCAN/FDCAN_Image_transmission/CM4/Inc/stm32h7xx_hal_conf.h   HAL Configuration file for Cortex-M4
  - FDCAN/FDCAN_Image_transmission/CM4/Inc/stm32h7xx_it.h         Interrupt handlers header file for Cortex-M4
  


@par Hardware and Software environment  

  - This example runs on STM32H745xx devices.

  - This example has been tested with STMicroelectronics STM32H745I-DISCO
    boards and can be easily tailored to any other supported device 
    and development board.

  - STM32H745I-DISCO set-up
    - Connect a FDCAN1 CANH (CN11-1) to FDCAN2 CANH (CN10-1)
    - Connect a FDCAN1 CANL (CN11-2) to FDCAN2 CANL (CN10-2)
    - Jumpers JP6 and JP7 => fitted

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H745_DISCO_CM7 and STM32H745_DISCO_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot
   CPU1 (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example


 */
