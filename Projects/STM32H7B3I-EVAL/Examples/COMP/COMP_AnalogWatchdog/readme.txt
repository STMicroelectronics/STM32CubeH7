/**
  @page COMP_AnalogWatchdog COMP Analog Watchdog Example
  
  @verbatim
  ******************************************************************************
  * @file    COMP/COMP_AnalogWatchdog/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the COMP Analog Watchdog Example.
  ******************************************************************************
  *
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

How to use a pair of comparator peripherals to compare a voltage level applied on
a GPIO pin to two thresholds: the internal voltage reference (VREFINT) and a fraction
of the internal voltage reference (VREFINT/4), in interrupt mode.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3XXQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280 MHz.
The APB clock dividers for CD  Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD  Domain APB4 peripherals to run at 280 MHz/2.


  - The upper threshold is set to VREFINT (about 1.2V)
  - The lower threshold is set to VREFINT / 4 (about 0.3V)
      NB: For precise value of VREFINT & VREFINT/4, please refer to datasheet.

LED on STM32H7B3I-EVAL can be used to monitor the voltage level compared to comparators thresholds:

- Voltage above the upper threshold: wake-up from low-power mode stop, LED1 blinks quickly.
- Voltage under the lower threshold: wake-up from low-power mode stop, LED1 blinks slowly (once every 1s).
- Voltage within the 2 thresholds: MCU enters in low-power mode stop, LED1 is turned off.

@note For debug, since this example makes the device enter in low-power stop:
      MCU must be woken-up before further download of program. Either set a voltage out of window thresholds
      or download after release of reset button.


@note Care must be taken when using HAL_Delay(), this function provides 
      accurate delay (in milliseconds) based on variable incremented in SysTick ISR. 
      This implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower) than the 
      peripheral interrupt. Otherwise the caller ISR process will be blocked. 
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set 
      to 1 millisecond to have correct HAL operation.

@par Keywords

Analog, comparator, stop mode, voltage compare, wakeup trigger, comparator interrupt.

@par Directory contents 

  - COMP/COMP_AnalogWatchdog/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - COMP/COMP_AnalogWatchdog/Inc/stm32h7xx_it.h          COMP interrupt handlers header file
  - COMP/COMP_AnalogWatchdog/Inc/main.h                  Header for main.c module
  - COMP/COMP_AnalogWatchdog/Src/stm32h7xx_it.c          COMP interrupt handlers
  - COMP/COMP_AnalogWatchdog/Src/main.c                  Main program
  - COMP/COMP_AnalogWatchdog/Src/stm32h7xx_hal_msp.c     HAL MSP file 
  - COMP/COMP_AnalogWatchdog/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment 

  - This example runs on STM32H7B3xxQ devices.

  - This example has been tested with STM32H7B3I-EVAL board and can be
    easily tailored to any other supported device and development board.

  - STM32H7B3I-EVAL Set-up
    - Use an external power supply and adjust supply voltage (above, within and below thresholds)
    and connect it on PB.00 (connected on pin 28 in CN6)
    - Make sure JP10 is removed.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
