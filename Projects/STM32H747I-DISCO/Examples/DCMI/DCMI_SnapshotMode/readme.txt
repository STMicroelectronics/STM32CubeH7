/**
  @page DCMI_SnapshotMode DCMI Snapshot Mode example
  
  @verbatim
  ******************************************************************************
  * @file    DCMI/DCMI_SnapshotMode/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the STM32H7xx DCMI_SnapshotMode example.
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

How to use the DCMI to interface with a camera module, capture a single image 
in Camera Frame Buffer (320x240 with RGB565 format) and once full frame camera is captured 
display it on the LCD in ARGB8888 format.

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

The Digital camera interface (DCMI) is configured to receive the capture from
the camera module connected to STM32H747I-DISCO board.
A DMA Peripheral to Memory is programmed between DCMI and Camera Frame buffer to receive a 320x240 RGB565
image in camera frame buffer located in the external SDRAM.
When the DCMI frame event callback is raised the picture is transferred to the LCD frame buffer using the DMA2D.   

The camera module is configured to generate QVGA (320x240) image resolution.
At the end of the example , LCD shows the captured image 320x240 at the center and the Green Led (LED1) will starts blinking.
RED LED (LED3) turns On in case of error.

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

@par Keywords

Display, DCMI, Camera, Snapshot, Frame Buffer, LCD, ARGB8888, DMA, RGB565, SDRAM, DMA2D, QQVGA


@par Directory contents 

  - DCMI/DCMI_SnapshotMode/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration source file 

  - DCMI/DCMI_SnapshotMode/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M7
  - DCMI/DCMI_SnapshotMode/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M7
  - DCMI/DCMI_SnapshotMode/Inc/main.h                  Header for main.c module  for Cortex-M7
  - DCMI/DCMI_SnapshotMode/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M7
  - DCMI/DCMI_SnapshotMode/Src/main.c                  Main program for Cortex-M7
  - DCMI/DCMI_SnapshotMode/CM7/Src/stm32h7xx_hal_msp.c     HAL MSP module for Cortex-M7   

  - DCMI/DCMI_SnapshotMode/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M4
  - DCMI/DCMI_SnapshotMode/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M4
  - DCMI/DCMI_SnapshotMode/CM4/Inc/main.h                  Main configuration file for Cortex-M4
  - DCMI/DCMI_SnapshotMode/CM4/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M4
  - DCMI/DCMI_SnapshotMode/CM4/Src/main.c                  Main program  for Cortex-M4
  - DCMI/DCMI_SnapshotMode/CM4/Src/stm32h7xx_hal_msp.c     HAL MSP module for Cortex-M4   

@par Hardware and Software environment

  - This example runs on STM32H747xx device.
    
  - This example has been tested with STM32H747I-DISCO board and can be
    easily tailored to any other supported device and development board.      

  - STM32H747I-DISCO :
       - Connect Camera module to P3 connector.   

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H747I_DISCO_CM7 and STM32H747I_DISCO_CM4): 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example 



 */
