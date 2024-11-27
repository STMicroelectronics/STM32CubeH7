/**
  @page MDMA_LinkedList_ColorsComp MDMA linked list for colors decomposing example description
  
  @verbatim
  ******************************************************************************
  * @file    MDMA/MDMA_LinkedList_ColorsComp/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the MDMA Linked List Colors Comp example.
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

  This example demonstrates how to use the MDMA in linked list mode to extract Red/Green and blue
  colors of an ARGB8888 image, resize each sub image (with a decimation factor /2) and 
  display the result Red/green/blue decimated sub-images on the LCD.

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
 
  After LCD initialization, the LCD layer 0 is configured for display. 
  
  The MDMA is configured in linked list mode as follow : 
  
  Node0 which corresponds to the MDMA initialization parameters is used to transfer the original image
  to the top center of the display screen.
  
  other linked list nodes are created using function "HAL_MDMA_LinkedList_CreateNode" then connected to each other using
  function "HAL_MDMA_LinkedList_AddNode"
  
  Node0_1,Node0_2 and Node0_3 are used to reset the 3 sub-images display area with default ARGB8888 color
  0xFF000000 which corresponds to black.
  
  Node1, Node2 and Node3 are respectively used to extract Red, Green and blue components from 
  the original image and to copy the result   to the display areas previously reset 
  by respectively Node0_1,Node0_2 and Node0_3.
  
  Note that the nodes are configured also to subsample the original image by 2 horizontally and vertically.
  The nodes are configured as follow :
  - Source and destination data size set to byte : to copy one color component (Red, green or blue) only from the source image
  - Source Data increment is set to double word (64 bits) : in order to jump one 32 bits word over 2.
  - Destination data increment is set to Word (32bits)
    knowing that an ARGB8888 pixel is defined on one word (32bits), this allows to decimate horizontally the original image by 2.
  - The source block address offset is set Image_width * bytes_per_pixels = Image_width * 4. This allows
    to jump one line over 2 from the source image allowing to decimate vertically the original image by 2.
  
  
  Note : The parameter "pPrevNode" of function "HAL_MDMA_LinkedList_AddNode"
  is set to zero in order to connect the given node to the end of the linked list.
      
  The transfer is started using function "HAL_MDMA_Start_IT". The parameters source and destination addresses
  correspond to the Node0 source and destination. for all other nodes, the source and destination are passed
  during the node creation (function HAL_MDMA_LinkedList_CreateNode).   
  
  When the MDMA transfer ends the LCD will display the original image on (Top) then 
  3 images that correspond to Red/Green and Blue components decimated(resized) by 2 vertically and horizontally.
  

 The STM32H747I DISCO board's LED can be used to monitor the transfer status:
  - LED4 is ON when the transfer ends without error.
  - LED3 is ON when if a transfer error occurs.

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
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, MDMA, Linked List, Display, LCD, SW Request, Image, Colors, Extract, Resize, Decimate, ARGB8888, Pixel

@par Directory contents

  - MDMA/MDMA_LinkedList_ColorsComp/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration source file 
  
  - MDMA/MDMA_LinkedList_ColorsComp/CM7/Inc/stm32h7xx_hal_conf.h  HAL configuration file for Cortex-M7
  - MDMA/MDMA_LinkedList_ColorsComp/CM7/Inc/stm32h7xx_it.h        Interrupt handlers header file for Cortex-M7
  - MDMA/MDMA_LinkedList_ColorsComp/CM7/Inc/main.h                Main configuration file for Cortex-M7
  - MDMA/MDMA_LinkedList_ColorsComp/CM7/Src/stm32h7xx_it.c        Interrupt handlers for Cortex-M7
  - MDMA/MDMA_LinkedList_ColorsComp/CM7/Src/main.c                Main program for Cortex-M7	
  - MDMA/MDMA_LinkedList_ColorsComp/CM7/image_320x240_argb8888.h  Image of size 320x240 and format ARGB8888 to be displayed on LCD

  - MDMA/MDMA_LinkedList_ColorsComp/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M4
  - MDMA/MDMA_LinkedList_ColorsComp/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M4
  - MDMA/MDMA_LinkedList_ColorsComp/CM4/Inc/main.h                  Main configuration file for Cortex-M4
  - MDMA/MDMA_LinkedList_ColorsComp/CM4/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M4
  - MDMA/MDMA_LinkedList_ColorsComp/CM4/Src/main.c                  Main program  for Cortex-M4
  - MDMA/MDMA_LinkedList_ColorsComp/CM4/Src/stm32h7xx_hal_msp.c     HAL MSP module for Cortex-M4  
  
@par Hardware and Software environment  

  - This example runs on STM32H747xx device.
  
  - This example has been tested with STM32H747I-DISCO  board with SMPS (SD Converter) power supply config and can be
    easily tailored to any other supported device and development board.    

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H747I_DISCO_CM7 and STM32H747I_DISCO_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example
                           

 */
                                   
