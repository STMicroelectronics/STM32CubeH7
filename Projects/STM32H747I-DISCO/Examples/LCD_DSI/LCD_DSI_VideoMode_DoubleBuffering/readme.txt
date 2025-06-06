/**
  @page LCD_DSI_VideoMode_DoubleBuffering : LCD DSI examples in DSI mode Video Burst

  @verbatim
  ******************************************************************************
  * @file    LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LCD DSI in mode video burst example.
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

This example provides a description of how to use the embedded LCD DSI controller 
(using IPs LTDC and DSI Host) to drive the KoD LCD mounted on board or a HDMI monitor
Connected through the DSI-HDMI bridge board MB1232.A.

The goal of this example is to display a QVGA landscape (320x240) images on LCD glass
or the HDMI monitor in continuous mode (video burst) using two buffers one for display and one for draw.
The example can automatically detect if the KoD LCD is connected or if the DSI-HDMI 
bridge is connected to the DSI connector (CN15), if no DSI slave is connected the RED LED will turn On.

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

Layer0 is initialized to display a brief description of the example on the LCD/HDMI screen. It will be
used for images display also.

Line Event callback (LTDC) is used to swap front and back buffer upon drawing 
complete refresh (V-SYNC). Each image is displayed for two seconds. 
'back_buffer' pointer is used to select image to draw in back buffer,
while 'front_buffer' pointer is used to select image to display in front buffer.

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

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note: The STM32H747I-DISCO board uses an LCD display in landscape mode with a 45° tearing effect
       limitation during the transition between the first and second images. To avoid this limitation,
       we suggest using an adapted DSI command mode with a left/right refresh technique

@par Keywords

Graphics, Display, LCD, DMA2D, DSI, MIPI Alliance, Video mode, Double buffering, LTDC, QVGA, ARGB8888, SDRAM


@par Directory contents

  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration source file 
  
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/CM7/Inc/stm32h7xx_hal_conf.h          HAL configuration file for Cortex-M7
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/CM7/Inc/stm32h7xx_it.h                Interrupt handlers header file for Cortex-M7
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/CM7/Inc/main.h                        Header for main.c module for Cortex-M7
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/CM7/Inc/life_augmented_argb8888.h     Image 320x240 in ARGB8888 to display on LCD
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/CM7/Inc/image_320x240_argb8888.h      Image 320x240 in ARGB8888 to display on LCD   
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/CM7/Src/stm32h7xx_it.c                Interrupt handlers for Cortex-M7
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/CM7/Src/main.c                        Main program for Cortex-M7
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/CM7/Src/stm32h7xx_hal_msp.c           HAL MSP file for Cortex-M7

  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M4
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M4
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/CM4/Inc/main.h                  Main configuration file for Cortex-M4
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/CM4/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M4
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/CM4/Src/main.c                  Main program  for Cortex-M4
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/CM4/Src/stm32h7xx_hal_msp.c     HAL MSP module for Cortex-M4  

@par Hardware and Software environment

  - This example runs on STM32H747xx/STM32H757xx devices.

  - This example has been tested with STMicroelectronics STM32H747I-DISCO 
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H747I_DISCO_CM4 first then STM32H747I_DISCO_CM7) : 
     - Rebuild all files 
     - Load images into target memory
 - Mount either the KoD LCD panel (Board MB1166 ) or the DSI-HDMI bridge(Board MB1232 )
   to the DSI connector (CN15). 
 - if using the DSI-HDMI bridge then connect a HDMI monitor using a HDMI cable to the 
   MB1232 board.  
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example



 */
