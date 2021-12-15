/**
  @page LPTIM_Timeout LPTIM Timeout example with LSE clock source
  
  @verbatim
  ******************************************************************************
  * @file    LPTIM/LPTIM_Timeout/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LPTIM Timeout example.
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

How to implement, through the HAL LPTIM API, a timeout with the LPTIMER peripheral, to wake up 
the system from a low-power mode.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7A3xxQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280 MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.


The main() function configures the LPTIMER and goes in STOP mode.
In order to start the timer a first trigger is needed on (PG.14)(LPTIM_ETR).
Any successive trigger event on (PG.14) will reset the counter and the timer 
will restart. The timeout value corresponds to the compare value (32768).
If after the first trigger no other trigger occurs within the expected time frame, 
the MCU is woken-up by the compare match event and LED1 toggles.

In this example the LPTIM is clocked by the LSI 

  Timeout = (Compare + 1) / LPTIM_Clock
          = (32767 + 1) / LSI_Clock_Frequency

Once the system is woken up it remains in run mode. The led keeps toggling each time the timer expires.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA (or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

Timer, Low Power Timer, Wake up, Stop mode, LSI, Run mode,

@par Directory contents  

  - LPTIM/LPTIM_Timeout/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - LPTIM/LPTIM_Timeout/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - LPTIM/LPTIM_Timeout/Inc/main.h                  Header for main.c module  
  - LPTIM/LPTIM_Timeout/Inc/stm32h7xx_nucleo_conf.h  STM32H7xx_Nucleo board configuration file
  - LPTIM/LPTIM_Timeout/Src/stm32h7xx_it.c          Interrupt handlers
  - LPTIM/LPTIM_Timeout/Src/main.c                  Main program
  - LPTIM/LPTIM_Timeout/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - LPTIM/LPTIM_Timeout/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H7A3xxQ devices.
    
  - This example has been tested with STMicroelectronics Nucleo-H7A3ZI-Q
    board and can be easily tailored to any other supported device
    and development board.   
	
  - Connect an external trigger (ETR) to PG.14 (pin 12 in CN10 connector ). 
    If the trigger is higher than 1Hz, the counter is regularly reset, the system stays in STOP mode.
    If the trigger is lower than 1Hz, the counter expires and the system is woken up.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred tool chain 
 - Rebuild all files and load your image into target memory
 - Run the example 


 */
