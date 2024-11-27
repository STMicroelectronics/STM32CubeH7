/**
  @page CEC_DataExchange CEC_DataExchange example
  
  @verbatim
  ******************************************************************************
  * @file    CEC/CEC_DataExchange/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CEC Data Exchange example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

This example shows how to configure and use the CEC peripheral to receive and 
transmit messages.


- Hardware Description

To use this example, two STM32H743I-EVAL boards (called Device_1 and 
Device_2) are loaded with the same software then connected through CEC lines

 /|\  In the firmware file main.h, uncomment the dedicated line to use
/_!_\ the CEC peripheral as STM32 device_1 or STM32 device_2.

@verbatim
*------------------------------------------------------------------------------*
|           STM32H743I-EVAL                         STM32H743I-EVAL            |
|         Device Address :0x01                    Device Address :0x03         |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         | 
|        |     __________     |                 |     __________     |         |
|        |    |   CEC    |____|____CECLine______|____|   CEC    |    |         |
|        |    | Device_1 |    |                 |    | Device_2 |    |         |
|        |    |__________|    |                 |    |__________|    |         |
|        |                    |                 |                    |         |
|        |  O LD1             |                 |  O LD1             |         |
|        |  O LD2    Joystick |                 |  O LD2    Joystick |         |
|        |  O LD3        _    |                 |  O LD3        _    |         |
|        |  O LD4       |_|   |                 |  O LD4       |_|   |         |
|        |                    |                 |                    |         |
|        |             GND O--|-----------------|--O GND             |         |
|        |____________________|                 |____________________|         |
|                                                                              |
|                                                                              |
*------------------------------------------------------------------------------*
@endverbatim


- Software Description

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices:
 - The CPU at 400MHz 
 - The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
 - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 100MHz.

The test unrolls as follows.

On TX side, four possible messages can be transmitted and are indicated as
below on the transmitting board:
 - when Tamper push-button is pressed, LED1 toggles
 - when Joystick Selection push-button is pressed, LED2 toggles
 - when Joystick UP push-button is pressed, LED3 toggles
 - when Joystick DOWN push-button is pressed, LED4 toggles

Accordingly, the following happens on the RX side in case of successful
reception:
 - when Tamper push-button is pressed on TX side, 
     * all RX side LEDs are turned off 
 - when Joystick Selection push-button is pressed on TX side, on RX side
     *  LED1 and LED2 are turned on
     *  LED3 and LED4 are turned off 
 - when Joystick UP push-button is pressed on TX side, 
     *  all RX side LEDs are turned on
 - when Joystick DOWN push-button is pressed on TX side, on RX side 
     * LED1 and LED2 are turned off
     * LED3 and LED4 are turned on    
In case of unsuccessful reception, LED3 is turned on.

Practically, 2 EXTI lines (EXTI15_10 and EXTI9_5) are configured to 
generate an interrupt on each falling or rising edge. 
A specific message is then transmitted by the CEC IP
and a LED connected to a specific MFX GPIO pin is toggled.
    - EXTI9_5 is mapped to MFX used to manage Joystick pins
    - EXTI15_10 is mapped to PC.13

Then, on TX side,
  - when rising edge is detected on EXTI9_5-SEL joystick button, LED2 toggles
  - when falling edge is detected on EXTI15_10 and EXTI line interrupt is detected
    on PC.13, LED1 toggles
  - when falling edge is detected on EXTI9_5-UP joystick button, LED3 toggles
  - when falling edge is detected on EXTI9_5-DOWN joystick button LED4 toggles

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Connectivity, CEC, Transmission, Reception, joystick, Data exchange

@par Directory contents 

  - CEC/CEC_DataExchange/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - CEC/CEC_DataExchange/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - CEC/CEC_DataExchange/Inc/main.h                  Header for main.c module  
  - CEC/CEC_DataExchange/Src/stm32h7xx_it.c          Interrupt handlers
  - CEC/CEC_DataExchange/Src/system_stm32h7xx.c      STM32H7xx system source file
  - CEC/CEC_DataExchange/Src/main.c                  Main program
  - CEC/CEC_DataExchange/Src/stm32h7xx_hal_msp.c     IP hardware resources initialization
  
@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - in main.h, uncomment DEVICE_1 for first board, uncomment DEVICE_2 for second board
 - Rebuild all files and load your image into target memory
 - With a wire, connect GPIOA-GPIO_PIN_15 between the 2 boards
 - Connect the ground of the 2 boards
 - Connect a 27k Ohms pull-up resistor between CEC pin (PA15) and VDD (+3V3)
 - Run the example


 */
