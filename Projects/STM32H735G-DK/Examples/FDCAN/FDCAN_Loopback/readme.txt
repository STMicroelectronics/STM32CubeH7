/**
  @page FDCAN_Loopback FDCAN Loopback example

  @verbatim
  ******************************************************************************
  * @file    FDCAN/FDCAN_Loopback/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FDCAN_Loopback ewample.
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

This example shows how to configure the FDCAN to operate in loopback mode.

This projects is configured for STM32H735xx devices using STM32CubeH7 HAL and running on
STM32H735G-DK board from STMicroelectronics.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H735xx Devices:
 - The CPU at 520 MHz
 - The HCLK for D1 Domain AXI peripherals, D2 Domain AHB peripherals and D3 Domain AHB  peripherals at 260 MHz.
 - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 130 MHz.

Then:
 - FDCAN module is configured to operate in external loopback mode.
 - FDCAN module is configured to receive:
    - messages with pre-defined standard ID to Rx Buffer 0
    - messages with pre-defined extended ID to Rx FIFO 1
 - FDCAN module is started.
 - The following messages are then sent:
    - one standard ID message matching Rx Buffer 0 filter
    - two extended ID messages matching Rx FIFO 1 filter

When User button is pressed:
 - main program checks that the three messages are received as expected
 - if the result is OK, LED1 start blinking

If at any time of the process an error is encountered LED2 is turned ON.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Connectivity, Communication, FDCAN, CAN-FD, Bus, Transmission, Reception, Loopback, Frame

@par Directory contents 

  - FDCAN/FDCAN_Loopback/Inc/stm32h7xx_hal_conf.h            HAL configuration file
  - FDCAN/FDCAN_Loopback/Inc/stm32h7xx_it.h                  Interrupt handlers header file
  - FDCAN/FDCAN_Loopback/Inc/main.h                          Header for main.c module  
  - FDCAN/FDCAN_Loopback/Inc/stm32h735g_discovery_conf.h     STM32H735G-DK BSP configuration file
  
  - FDCAN/FDCAN_Loopback/Src/stm32h7xx_it.c                  Interrupt handlers
  - FDCAN/FDCAN_Loopback/Src/main.c                          Main program
  - FDCAN/FDCAN_Loopback/Src/stm32h7xx_hal_msp.c             HAL MSP module
  - FDCAN/FDCAN_Loopback/Src/system_stm32h7xx.c              stm32h7xx system source file

@par Hardware and Software environment 

  - This example runs on STM32H735xx devices.

  - This example has been tested with an STM32H735G-DK board embedding a 
    STM32H735xx device and can be easily tailored to any other supported device 
    and development board.

  - STM32H735G-DK set-up
    - Jumpers JP4, JP5 and JP6 => fitted

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
