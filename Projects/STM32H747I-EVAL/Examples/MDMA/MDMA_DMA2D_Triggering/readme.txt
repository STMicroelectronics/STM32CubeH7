/**
  @page MDMA_DMA2D_Triggering MDMA with DMA2D Transfer complete flag used as MDMA trigger Example
  
  @verbatim
  ******************************************************************************
  * @file    MDMA/MDMA_DMA2D_Triggering/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the MDMA with DMA2D Transfer complete flag 
  *          used as MDMA trigger Example.
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

This example describes how to use the MDMA with HW trigger set to the DMA2D transfer complete flag.

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

The MDMA channel 0 is then configured in HW request mode with HW transfer request trigger
set to DMA2D Transfer Complete flag. 
The MDMA is initialized to copy the DMA2D destination image to the LCD frame buffer at the bottom-center 
position with 180° rotation and mirror effect.
Each block corresponds to a line of the source/destination image.
The number of blocks corresponds to the number of lines of the image.

The MDMA Request is set to DMA2D Transfer complete flag and the MDMA trigger Mode is set 
to Repeat block mode. As consequence each DMA2D transfer complete event 
(DMA2D has end copying an image to the Top-center of the LCD frame buffer ), the MDMA will start 
a repeat block transfer which correspond of copying the DMA2D destination image with 180° 
and rotation effect to the bottom-center of the LCD frame buffer.
    
The DMA2D is initialized to copy an image from the internal flash to the Top-Center 
of the LCD frame buffer.
In an infinite loop, each time a DMA2D transfer is started to copy a new image. 
As the MDMA Request is set to DMA2D Transfer complete flag each time the DMA2D ends
transferring an image to the Top-center of the LCD frame buffer, the MDMA will use 
this DMA2D destination image to transfer it to the bottom-center of the LCD frame buffer 
with 180° rotation an mirror effect.
A delay of 2sec is inserted after each DMA2D transfer

AS consequence user can check that every 2sec a new image is transferred to the 
Top-Center of the LCD (Using the DMA2D) and the same image with 180° rotation and
mirror effect is transferred to the bottom-center of the LCD (Using the MDMA) 

Note that in order to make the MDMA transfer infinite, a linked list node that have 
the same parameters as the initial transfer is created.
The MDMA linked list is made circular by connecting this node to itself using HAL function 
HAL_MDMA_LinkedList_EnableCircularMode.
The purpose of the linked list node is to create a circular list. the MDMA 
transfer will then loop infinitely. Each DMA2D Transfer complete flag will trig
a Node transfer (a repeat block transfer) that correspond to a new image transfer.

@note the MDMA linked list Nodes addresses must be 64 bits aligned and must be in the AXI address space.
this constraints must be respected when defining the MDMA_LinkNodeTypeDef nodes variables.
As the MDMA Node descriptor "Xfer_Node" is located in the D1 AXI-SRAM which 
is cacheable, it is necessary to clean the data cache after creating and configuring the node 
in order to make sure that the MDMA will load up-to-date data from the linked list node.

STM32H747I Eval board's LEDs can be used to monitor the transfer status:
 - LED4 is ON when the MDMA transfer process is complete.
 - LED3 is ON when there is an error in MDMA or DMA2D transfer process.

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

System, MDMA, DMA, DMA2D, Linked list, Display, Image, LCD, SDRAM, Transfer, Memory to memory, Flash, Frame buffer, Mirror effect, Rotation

@par Directory contents 

  - MDMA/MDMA_DMA2D_Triggering/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration source file 
  
  - MDMA/MDMA_DMA2D_Triggering/CM7/Inc/stm32h7xx_hal_conf.h        HAL configuration file for Cortex-M7
  - MDMA/MDMA_DMA2D_Triggering/CM7/Inc/stm32h7xx_it.h              DMA  and MDMA interrupt handlers header file for Cortex-M7
  - MDMA/MDMA_DMA2D_Triggering/CM7/Inc/main.h                      Header for main.c module for Cortex-M7
  - MDMA/MDMA_DMA2D_Triggering/CM7/Inc/life_augmented_argb8888.h   Image 320x240 in ARGB8888 to display on LCD
  - MDMA/MDMA_DMA2D_Triggering/CM7/Inc/image_320x240_argb8888.h    Image 320x240 in ARGB8888 to display on LCD    
  - MDMA/MDMA_DMA2D_Triggering/CM7/Src/stm32h7xx_it.c              MDMA interrupt handlers for Cortex-M7
  - MDMA/MDMA_DMA2D_Triggering/CM7/Src/main.c                      Main program for Cortex-M7
  - MDMA/MDMA_DMA2D_Triggering/CM7/Src/stm32h7xx_hal_msp.c         HAL MSP module for Cortex-M7

  - MDMA/MDMA_DMA2D_Triggering/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M4
  - MDMA/MDMA_DMA2D_Triggering/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M4
  - MDMA/MDMA_DMA2D_Triggering/CM4/Inc/main.h                  Main configuration file for Cortex-M4
  - MDMA/MDMA_DMA2D_Triggering/CM4/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M4
  - MDMA/MDMA_DMA2D_Triggering/CM4/Src/main.c                  Main program  for Cortex-M4
  - MDMA/MDMA_DMA2D_Triggering/CM4/Src/stm32h7xx_hal_msp.c     HAL MSP module for Cortex-M4  
  
@par Hardware and Software environment

  - This example runs on STM32H747xx device.
    
  - This example has been tested with STM32H747I-EVAL  board with SMPS (SD Converter) power supply config and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H747I_EVAL_CM7 and STM32H747I_EVAL_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example


 */
 
