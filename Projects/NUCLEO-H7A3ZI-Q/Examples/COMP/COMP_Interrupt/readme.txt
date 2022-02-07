/**
  @page COMP_Interrupt COMP Interrupt example
  
  @verbatim
  ******************************************************************************
  * @file    COMP/COMP_Interrupt/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the COMP Interrupt example.
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

How to use a comparator peripheral to compare a voltage level applied on a GPIO
pin to the the internal voltage reference (VREFINT), in interrupt mode.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7A3ZITx Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280 MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.

When the comparator input crosses (either rising or falling edges) the internal 
reference voltage VREFINT (1.22V), the comparator generates an interrupt
and system wakes-up from Stop mode.

The System enters Stop mode 5 seconds after the comparator is started and 
after any system wake-up triggered by the comparator interrupt.

Before entering in Stop mode, LED1 toggles 4 times quickly.

In this example, the comparator input plus is connected on pin PB.02.
LED1 is turned on or off depending on comparator output state:
turned on if comparator voltage level on input plus is higher than input minus,
turned-off otherwise.

If LED1 is toggling continuously without any voltage update, it indicates that the system 
generated an error.

@note Care must be taken when using HAL_Delay(), this function provides 
      accurate delay (in milliseconds) based on variable incremented in SysTick ISR. 
      This implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower) than the 
      peripheral interrupt. Otherwise the caller ISR process will be blocked. 
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set 
      to 1 millisecond to have correct HAL operation.
	  
@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

Analog, comparator, stop mode, voltage compare, wakeup trigger, comparator interrupt.

@par Directory contents 

  - COMP/COMP_Interrupt/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - COMP/COMP_Interrupt/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - COMP/COMP_Interrupt/Inc/main.h                  Header for main.c module
  - COMP/COMP_Interrupt/Inc/stm32h7xx_nucleo_conf.h    HAL configuration file  
  - COMP/COMP_Interrupt/Src/stm32h7xx_it.c          Interrupt handlers
  - COMP/COMP_Interrupt/Src/main.c                  Main program
  - COMP/COMP_Interrupt/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - COMP/COMP_Interrupt/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment 

  - This example runs on STM32H7A3xxQ devices.
  - In this example, the clock is set to 280 MHz.

  - This example has been tested with NUCLEO-H7A3ZI-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-H7A3ZI-Q Set-up

    - Use GREEN LED1 connected to PB.00 pin
    - Apply an external variable voltage on PB.02 (connected on pin 15 on CN10) with average voltage 1.22V.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
