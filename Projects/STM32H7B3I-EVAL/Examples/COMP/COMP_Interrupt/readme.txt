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

This example shows how to configure the COMP peripheral to compare the external
voltage applied on a specific pin with the Internal Voltage Reference.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7b3xx Devices :
- The CPU at 280MHz
- The HCLK for CD Domain AXI and AHB3 peripherals , CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280MHz.
- The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 140MHz.

When the comparator input crosses (rising edge) the internal 
reference voltage VREFINT (1.22V), the comparator generates an interrupt
and exit from STOP mode.

The System enters STOP mode 5 seconds with LED2 off after the comparator is started and 
after any system wake-up triggered by the comparator interrupt.

In this example, the comparator input is connected on the pin PB.00 the user 
shall apply a voltage on and each time the comparator input crosses VREFINT,MCU is awake,
system clock is reconfigured and LED2 is ON.
If LED3 is toggling continuously without any voltage update, it indicates that the system 
generated an error.

@note In case of noise on voltage applied on COMP_INPUT, The MCU
may be awake at a voltage level with an uncertainty of tens of mV.

@note Care must be taken when using HAL_Delay(), this function provides 
      accurate delay (in milliseconds) based on variable incremented in SysTick ISR. 
      This implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower) than the 
      peripheral interrupt. Otherwise the caller ISR process will be blocked. 
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set 
      to 1 millisecond to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the �MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
����� If the application needs to use DMA(or other masters) based access or requires more RAM, then �the user has to:
����������� � - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
����������� � - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
�������       - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly�defined to be aligned to L1-CACHE line size (32 bytes). 
�
@Note It is recommended to enable the cache and maintain its coherence.
��������������Please refer to the AN4838 �Managing memory protection unit (MPU) in STM32 MCUs�
��������������Please refer to the AN4839 �Level 1 cache on STM32F7 Series�

@par Keywords

comparator, analog, stop mode, voltage compare, wakeup trigger, comparator interrupt.

@par Directory contents 

  - COMP/COMP_Interrupt/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - COMP/COMP_Interrupt/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - COMP/COMP_Interrupt/Inc/main.h                  Header for main.c module
  - COMP/COMP_Interrupt/Inc/stm32h7b3i_eval_conf.h  STM32H7B3I_EVAL board configuration file
  - COMP/COMP_Interrupt/Inc/mx25lm51245g_conf.h     MX25LM51245G QSPI memory Description file
  - COMP/COMP_Interrupt/Src/stm32h7xx_it.c          Interrupt handlers
  - COMP/COMP_Interrupt/Src/main.c                  Main program
  - COMP/COMP_Interrupt/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - COMP/COMP_Interrupt/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment 

  - This example runs on STM32H7b3xx devices.

  - This example has been tested with STM32H7B3I-EVAL  board and can be
    easily tailored to any other supported device and development board.

  - STM32H7B3I-EVAL  Set-up

     - Apply an external variable voltage on PB.0 with average voltage 1.22V.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
