/**
  @page COMP_OutputBlanking COMP output blanking example
  
  @verbatim
  ******************************************************************************
  * @file    COMP/COMP_OutputBlanking/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the COMP output blanking Example
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

How to use the comparator-peripheral output blanking feature. The purpose of the
output blanking feature in motor control is to prevent tripping of the current
regulation upon short current spikes at the beginning of the PWM period.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7A3xxQ Devices :
The CPU at 280MHz
The HCLK for CD Domain AXI and AHB3 peripherals , CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 140MHz.

In this example COMP1 and TIM1 are used:
COMP1 is configured as following:
 - Non Inverting input is connected to PB0 pin 31 in CN11 connector
 - Inverting input is connected to VREFINT (1.22V)
 - Output is available on PE6 pin 20 in CN9 connector
 - Blanking source is connected internally to TIM1 OC5

SystemCoreClock is set to 280 MHz for STM32H7A3xxQ Devices.
TIM1 is configured as following:

   In this example TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2),
   since APB2 prescaler is equal to 2 so :
      TIM1CLK = 2*PCLK2
      PCLK2 = HCLK/2
      => TIM1CLK = HCLK = SystemCoreClock
  
 - TIM1 period is (APB2 clock x 2) / period = (140000000 x 2) / 50000 = 5600 Hz.
 - TIM1 OC5 (internal channel: not available on GPIO) configured in PWM mode
   with a frequency equal to 5600 Hz and high level equal to
   pulse / period = 2000 / 280000000 = 7.1 micro second.
 - Timer channel intended to be connected to an external device (motor, ...) in user application:
   TIM1 CH2 (PA9 pin 21 in CN12 connector) configured in PWM mode with a frequency equal to 5600 Hz
   and duty cycle pulse/period = 100 * (37500 / 50000) =  75%.

Connection needed:
 - either connect COMP1 input (PB0 pin 31 in CN11 connector) to TIM1 CH2 (PA9 pin 21 in CN12 connector).
 - either connect COMP1 input (PB0 pin 31 in CN11 connector) to a signal generator, at the frequency of TIM1 period 
   (to have comparator input signal frequency matching blanking signal frequency. To do this, possibility to use 
   TIM1 CH2 (PA9 pin 21 in CN12 connector) as trigger of signal generator).
 - connect COMP1 output (PE6 pin 20 in CN9 connector) to an osciloscope to view the COMP1 output signal.

Comparator output signal expected:
 - Between t=0us and t=7.1us: Comparator positive input at level high, blanking source at level high,
   therefore comparator output is forced to level low.
   (otherwise, as comparator positive input is higher than negative input, comparator output would have been high.)
 - Between t=7.1us and t=duty cycle pulse: Comparator positive input at level high, blanking source at level low.
   Comparator positive input is higher than negative input, blanking source is low, therefore comparator output is high.
 - Between t=duty cycle pulse and t=period: Comparator positive input at level low, blanking source at level low.
   Comparator negative input is higher than positive input, therefore comparator output is low.

@note Care must be taken when using HAL_Delay(), this function provides 
      accurate delay (in milliseconds) based on variable incremented in SysTick ISR. 
      This implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower) than the 
      peripheral interrupt. Otherwise the caller ISR process will be blocked. 
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set 
      to 1 millisecond to have correct HAL operation.
	  
@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA (or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
      It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
              In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
              Even though the user must manage the cache coherence for read accesses.
              Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
              Please refer to the AN4839 “Level 1 cache on STM32F7 Series and STM32H7 Series

@par Keywords

Analog, comparator, output blanking, voltage compare, timer, timer-triggered, PWM, wave generator.

@par Directory contents 

  - COMP/COMP_OutputBlanking/Inc/stm32h7xx_hal_conf.h   HAL configuration file
  - COMP/COMP_OutputBlanking/Inc/stm32h7xx_it.h         COMP interrupt handlers header file
  - COMP/COMP_OutputBlanking/Inc/main.h                 Header for main.c module
  - COMP/COMP_OutputBlanking/Inc/stm32h7xx_nucleo_conf.h    HAL configuration file  
  - COMP/COMP_OutputBlanking/Src/stm32h7xx_it.c         COMP interrupt handlers
  - COMP/COMP_OutputBlanking/Src/main.c                 Main program
  - COMP/COMP_OutputBlanking/Src/stm32h7xx_hal_msp.c    HAL MSP file
  - COMP/COMP_OutputBlanking/Src/system_stm32h7xx.c     STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H7A3xxQ devices.

  - This example has been tested with NUCLEO-H7A3ZI-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-H7A3ZI-Q Set-up
    - Connect TIM1 CH2 (PA9 pin 21 in CN12 connector) to COMP1 Non Inverting input (PB0 pin 31 in CN11 connector)
      or to the input trigger of a wave generator.
    - connect COMP1 output (PE6 pin 20 in CN9 connector) to an osciloscope to view the COMP1 output signal.
    if you connect TIM1 CH2 to the input trigger of a wave generator :
    - Connect the wave generator output to COMP1 Non Inverting input (PB0 pin 31 in CN11 connector).
    - Configure the wave generator to output a pulse at rising edge event with
      high level greater than VREFINT (1.22V) and pulse duration lower 7.1 micro second
      (as an example you can use 10 micro second).
      You can change the pulse delay and note that the break event is generated
      only when the pulse is triggered after 7.1 micro second

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
