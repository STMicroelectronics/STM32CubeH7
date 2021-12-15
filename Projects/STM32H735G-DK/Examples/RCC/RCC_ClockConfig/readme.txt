/**
  @page RCC_ClockConfig RCC Clock Config example
  
  @verbatim
  ******************************************************************************
  * @file    RCC/RCC_ClockConfig/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RCC Clock Config example.
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

Configure the system clock (SYSCLK) and modify the clock settings in Run mode, using the RCC HAL API. 

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H735xx Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4 peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

In this example, after startup SYSCLK is configured to the max frequency using the PLL with
HSI as clock source, the User Button (connected to EXTI15_10) will be 
used to change the PLL source: 

- from HSI to CSI
- from CSI to HSE
- from HSE to HSI

Each time the User Button is pressed EXTI15_10 interrupt is generated and in the ISR
the PLL oscillator source is checked using __HAL_RCC_GET_PLL_OSCSOURCE() macro:

- If the HSE oscillator is selected as PLL source, the following steps will be followed to switch 
   the PLL source to HSI oscillator:
     a- Switch the system clock source to HSE to allow modification of the PLL configuration
     b- Enable HSI Oscillator, select it as PLL source and finally activate the PLL
     c- Select the PLL as system clock source and configure the SYSCLK, HCLK, PCLK1, D1PCLK1, D3PCLK1 and PCLK2 clocks dividers
     d- Disable the HSE oscillator (optional, if the HSE is no more needed by the application)

- If the CSI oscillator is selected as PLL source, the following steps will be followed to switch 
   the PLL source to HSE oscillator:
     a- Switch the system clock source to CSI to allow modification of the PLL configuration
     b- Enable HSE Oscillator, select it as PLL source and finally activate the PLL
     c- Select the PLL as system clock source and configure the SYSCLK, HCLK, PCLK1, D1PCLK1, D3PCLK1 and PCLK2 clocks dividers
     d- Disable the CSI oscillator (optional, if the CSI is no more needed by the application)	 


- If the HSI oscillator is selected as PLL source, the following steps will be followed to switch 
   the PLL source to HSE oscillator:
     a- Switch the system clock source to HSI to allow modification of the PLL configuration
     b- Enable HSE Oscillator, select it as PLL source and finally activate the PLL
     c- Select the PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
     d- Disable the HSI oscillator (optional, if the HSI is no more needed by the application)


LED1 is toggled with a timing defined by the HAL_Delay() API.
LED2 is ON when there is an error.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA (or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

RCC, System, Clock Configuration, HSE bypass mode, HSI, System clock, Oscillator, PLL

@par Directory contents 

  - RCC/RCC_ClockConfig/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - RCC/RCC_ClockConfig/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - RCC/RCC_ClockConfig/Inc/main.h                  Header for main.c module
  - RCC/RCC_ClockConfig/Inc/stm32h735g_discovery_conf.h  STM32H735G-DK board configuration file
  - RCC/RCC_ClockConfig/Src/stm32h7xx_it.c          Interrupt handlers
  - RCC/RCC_ClockConfig/Src/main.c                  Main program
  - RCC/RCC_ClockConfig/Src/system_stm32h7xx.c      STM32H7xx system source file

@par Hardware and Software environment

  - This example runs on STM32H735xx devices.
    
  - This example has been tested with STM32H735G-DK 
    board and can be easily tailored to any other supported device
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
