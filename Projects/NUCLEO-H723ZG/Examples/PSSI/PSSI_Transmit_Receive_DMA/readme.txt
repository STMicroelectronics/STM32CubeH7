/**
  @page PSSI_Transmit_Receive_DMA example

  @verbatim
  ******************************************************************************
  * @file    PSSI/PSSI_Transmit_Receive_DMA/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the PSSI_Transmit_Receive_DMA example.
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

This example describes how to perform PSSI data buffer transmission/reception
between it as a slave configured on a board and a master Simulated by another Board

This projects is configured for STM32H723xx devices using STM32CubeH7 HAL and running on
NUCLEO-H723ZG board from STMicroelectronics.

Board: NUCLEO-H723ZG

D0  Pin: PC6  (CN7 PIN1)
D1  Pin: PC7 (CN12 PIN19)
D2  Pin: PC8 (CN12 PIN2)
D3  Pin: PC9 (CN12 PIN1)
D4  Pin: PC11 (CN11 PIN2)
D5  Pin: PD3 (CN11 PIN40)
D6  Pin: PB8 (CN12 PIN3)
D7  Pin: PB9 (CN12 PIN5)
Clk Pin: PA6 (CN12 PIN13)
DE  Pin: PA4 (CN7 PIN17)

TIM_PWM: PE9 (CN10 PIN 4)

The PSSI communication is as shown next:
   _________________________                       _________________________
  |           ______________|                     |______________           |
  |          |PSSI          |                     |          PSSI|          |
  |          |              |                     |              |          |
  |          |          D0  |_____________________| D0           |          |
  |          |           .  |_____________________| .            |          |
  |          |           .  |_____________________| .            |          |
  |          |           .  |_____________________| .            |          |
  |          |          D7  |                     | D7           |          |
  |          |          Clk |_____________________| Clk _________|__        |
  |          |          DE  |_____________________| DE           |  |       |
  |          |              |                     |              |  |       |
  |          |              |                     |              |  |       |
  |          |              |                     |       TIM_PWM|__|       |
  |          |              |                     |              |          |
  |          |______________|                     |______________|          |
  |                         |                     |                         |
  |                      GND|_____________________|GND                      |
  |_STM32_Board Slave ______|                     |_STM32_Board Master______|



At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H723xx devices:
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 520 MHz/4.


The PSSI peripheral configuration is ensured by the HAL_PSSI_Init() function.
This later is calling the HAL_PSSI_MspInit()function which core is implementing
the configuration of the needed PSSI resources according to the used hardware (CLOCK,
GPIO, DMA and NVIC). You may update this function to change PSSI configuration.


Transmission_Reception Process : (Slave)

  The HAL_PSSI_Transmit_DMA() and the HAL_PSSI_Receive_DMA() ,functions
  allow respectively the transmission and the reception of a predefined data buffer
  once the clock is ON in the specidfied Pin (PA6).

  1- Transmit the data
  2- Receive the data
  3- Verify that the received data is compliant to the sent one and modified by the "Master"


  NUCLEO-H723ZG board's LEDs can be used to monitor the Transfer status:
  LED1 is On to indicate that the Transmission_Reception process is finished correctly.
  LED2 is On when Transmit is done and receive is ongoing
  LED3 is On there are errors while transmitting /receiving data or the received data is false.


Master Simulation Process :

  We choose :
  The TIM1 that will generate the clock to allow the slave to transmit and receive data
  The PSSI as an IP that will in infinite loop :
   1- Receive the data from the PSSI_Slave which is "Hello from Slave"
   2- Verify that is a request from a Slave
   3- Modify it by adding ": received, from Master"
   4- Transmit "Hello from Slave: received, from Master"

  NUCLEO-H723ZG board's LEDs can be used to monitor the Transfer status:
  LED1 is On for 1s to indicate that the Reception_Transmission process is finished correctly.
  LED2 is On when receive is ongoing
  LED3 is On there are errors while receiving/transmitting data.


The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file:
If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.

@note Some code parts can be executed in the ITCM-RAM (64 KB) which decreases critical task execution time, compared
      to code execution from Flash memory. This feature can be activated using '#pragma location = ".itcmram"' to be
      placed above function declaration, or using the toolchain GUI (file options) to execute a whole source file in
	  the ITCM-RAM.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
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

Transmission, Reception, PSSI, DMA, Slave,Clock

@par Directory contents
   PSSI/PSSI_Transmit_Receive_DMA/Inc/stm32h7xx_hal_conf.h    HAL configuration file
   PSSI/PSSI_Transmit_Receive_DMA/Inc/stm32h7xx_it.h          DMA and PSSI interrupt handlers header file
   PSSI/PSSI_Transmit_Receive_DMA/Inc/main.h                  Header for main.c module
   PSSI/PSSI_Transmit_Receive_DMA/Src/stm32h7xx_it.c          DMA and PSSI interrupt handlers
   PSSI/PSSI_Transmit_Receive_DMA/Src/main.c                  Main program
   PSSI/PSSI_Transmit_Receive_DMA/Src/system_stm32h7xx.c      STM32H7xx system source file
   PSSI/PSSI_Transmit_Receive_DMA/Src/stm32h7xx_hal_msp.c     HAL MSP file




@par Hardware and Software environment

  - This example runs on STM32H723xx devices.

  - This example has been tested with STMicroelectronics NUCLEO-H723ZG
    boards and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-H723ZG  Set-up
    - Connect PSSI Data lines ,clock line and Data Enable line of Master board to PSSI lines of Slave Board (described previously).
    - Connect PSSI clock line of Master board to TIM_PWM Pin of the same board (described previously).
    - Connect GND of Master board to GND of Slave Board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment   "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example




 */
