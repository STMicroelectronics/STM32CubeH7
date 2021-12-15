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
The SystemClock_Config() function is used to configure the system clock for STM32H7A3xxQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280 MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.

In this example, after startup SYSCLK is configured to the max frequency using the PLL with
HSE as clock source, the User push-button (connected to EXTI15_10) will be 
used to change the PLL source: 

- from HSI to CSI
- from CSI to HSE
- from HSE to HSI

 @note In HSE bypass mode, input clock will come from the MCO from
       ST_LINK MCU. This frequency cannot be changed, and it is fixed
       at 8 MHz. To use MCO from ST_LINK you need to check the Board
       User Manual to know how to connect the MCO pin to the STM32 device.
        
Each time the User push-button is pressed EXTI15_10 interrupt is generated and in the ISR
the PLL oscillator source is checked using __HAL_RCC_GET_PLL_OSCSOURCE() macro:

- If the HSE bypass oscillator is selected as PLL source, the following steps will be followed to switch 
   the PLL source to HSI oscillator:
     a- Switch the system clock source to HSE bypass to allow modification of the PLL configuration
     b- Enable HSI Oscillator, select it as PLL source and finally activate the PLL
     c- Select the PLL as system clock source and configure the SYSCLK, HCLK, PCLK1, D1PCLK1, D3PCLK1 and PCLK2 clocks dividers
     d- Disable the HSE bypass oscillator (optional, if the HSE bypass is no more needed by the application)

- If the CSI oscillator is selected as PLL source, the following steps will be followed to switch 
   the PLL source to HSE oscillator:
     a- Switch the system clock source to CSI to allow modification of the PLL configuration
     b- Enable HSE Oscillator, select it as PLL source and finally activate the PLL
     c- Select the PLL as system clock source and configure the SYSCLK, HCLK, PCLK1, D1PCLK1, D3PCLK1 and PCLK2 clocks dividers
     d- Disable the CSI oscillator (optional, if the CSI is no more needed by the application)	 


- If the HSI oscillator is selected as PLL source, the following steps will be followed to switch 
   the PLL source to HSE bypass oscillator:
     a- Switch the system clock source to HSI to allow modification of the PLL configuration
     b- Enable HSE bypass Oscillator, select it as PLL source and finally activate the PLL
     c- Select the PLL as system clock source and configure the SYSCLK, HCLK, PCLK1, D1PCLK1, D3PCLK1 and PCLK2 clocks dividers
     d- Disable the HSI oscillator (optional, if the HSI is no more needed by the application)

In this example the SYSCLK /4 is outputted on the MCO2 pin(PC.09).
 => PC.09 connected to pin 4 on CN8 connector


LED1 is toggled with a timing defined by the HAL_Delay() API.
LED3 is ON when there is an error.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

RCC, System, Clock Configuration, HSE bypass mode, HSI, System clock, Oscillator, PLL

@par Directory contents 

  - RCC/RCC_ClockConfig/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - RCC/RCC_ClockConfig/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - RCC/RCC_ClockConfig/Inc/main.h                  Header for main.c module
  - RCC/RCC_ClockConfig/Inc/stm32h7xx_nucleo_conf.h  STM32H7xx_Nucleo board configuration file
  - RCC/RCC_ClockConfig/Src/stm32h7xx_it.c          Interrupt handlers
  - RCC/RCC_ClockConfig/Src/main.c                  Main program
  - RCC/RCC_ClockConfig/Src/system_stm32h7xx.c      STM32H7xx system source file

@par Hardware and Software environment

  - This example runs on STM32H7A3xxQ devices.
    
  - This example has been tested with NUCLEO-H7A3ZI-Q 
    board and can be easily tailored to any other supported device
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
