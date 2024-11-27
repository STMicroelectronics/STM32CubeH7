/**
  @page PWR_Domain3SystemControl example
  
  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_Domain3SystemControl/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of D3 Domain system control in low power mode Example.
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
 
This example shows how to maintain a basic activity of the system in low power mode with D3 Domain 
only by ensuring the communication with the D3SRAM, the BDMA and the I2C4 when the CPU is in Stop mode.

In this example two boards are used to send/receive data using the I2C4 , so there are two examples 
to compile one for each board.
To compile the Domain 3 system control in low power mode example: Uncomment LOW_POWER_BOARD define 
in the main.h file.
To compile the I2C transmit, receive and verify data example: Comment LOW_POWER_BOARD define 
in the main.h file.

First Board:
The first board works in Low Power Mode since the CPU, the D1 Domain, the D2 Domain 
and the D3 Domain are in STOP mode. Only D3 Domain will ensure the communication between 
the D3SRAM, the BDMA and the I2C4 when a chunk is received.
After a system start-up, LED1 is ON to indicate that the system is in RUN mode.
After 3 seconds the system enters the STOP mode, then LED1 is set to Off. The user can then start  
transmission using the second board.
After receiving 5 chunks (20 Bytes) the system wakes-up and send back the received data to the
second board and finally it re-enter to STOP mode.
LED2 will turn ON if an error occurred during reception or transmission process.

Second Board:
The second board works in RUN mode. It is used to transmit chunks of 4 Bytes to the first board,
then receive and verify the received data after sending 5 chunks.
After a system start-up, LED1 is ON to indicate that the system is in RUN mode.
The user has to press the USER button to start data transmission. 
LED1 will remain toggling while no error occurred during the transmission/reception process.
LED2 will turn ON if an error occurred.

When the test is started by pressing the USER button the transmission/reception process between 
the two boards is repeated in an infinite loop.

Board: NUCLEO-H743ZI
SCL Pin: PB.08 (pin D15 on CN7)
SDA Pin: PB.09 (pin D14 on CN7)
   _________________________                       _________________________ 
  |           ______________|                     |______________           |
  |          |I2C4          |                     |        I2C4  |          |
  |          |              |                     |              |          |
  |          |          SCL |_____________________| SCL          |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |          SDA |_____________________| SDA          |          |
  |          |              |                     |              |          |
  |          |______________|                     |______________|          |
  |                         |                     |                         |
  |                      GND|_____________________|GND                      |
  |_STM32_Board 1___________|                     |_STM32_Board 2___________|
		Low Power Board 																


Two identical boards are connected as shown on the picture above.
Board 1: receiving chunks in low power mode, then transmit them.
Board 2: transmit chunks, then receiving and verifying them.

How to run:
  - Compile the program with LOW_POWER_BOARD define, then load it in the first board.
  - Compile the program without LOW_POWER_BOARD define, then load it in the second board.
  - Connect the two boards as shown in the picture and start them.
  - LED1 in the first board will set ON for 3 seconds, then it will turn Off to indicate 
    that the system is enter low power mode.
  - LED1 in the second board will maintain set to indicate that the system is in RUN mode.
  - When LED1 of the first board is Off the user can start the transmission by pressing on the
    USER button in the second board.
  => If the test passed, then LED1 on the second board will toggle each 100 ms to indicate 
     that the transmission and reception processes are in progress and no error occurred.
     If an error occurred during the communication LED2 (Blue LED) of the second board will turn On.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
 - The CPU at 400MHz 
 - The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
   and D3 Domain AHB4  peripherals at 200MHz.
 - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals 
   and D3 Domain APB4 peripherals to  run at 100MHz

The Domain 3 low power example uses:
 - The BDMA channel 7 configured in peripheral to memory mode to receive I2C data and clear the 
   Domain 3 pending source after waking-up from STOP mode when receiving the I2C4_RX request.
 - The BDMA channel 0 configured in memory to peripheral mode to clear the I2C4 address flag 
   when receiving the DMAMUX request 0.
 - The DMAMUX request generator is configured to generate one request (request 0) when receiving 
   the I2C4 wakeup interrupt.

After receiving 5 chunks of data, the BDMA transfer complete interrupt is generated and the system
is waking up from Stop mode to send-back the received data if no error occurred.
Finally the system will restart automatically the reception process and go Back to the STOP mode.

The DMAMUX request generator block is configured using function "HAL_DMAEx_ConfigMuxRequestGenerator" 
with the following parameters :
- SignalID      : set to HAL_DMAMUX2_REQUEST_GEN_I2C4_WKUP which corresponds to the I2c4_it_async signal.
- Polarity      : Set to RISING to use rising edge for DMA request generation.
- RequestNumber : 1 i.e for each received chunk from the I2C4 one DMA request is generated.

The DMA request generator is then enabled using the function “HAL_DMAEx_EnableMuxRequestGenerator”.

The CPU is only used to intercept a DMA transfer error if any and sets the LED2 (Blue LED) to ON in this case
or to tramsmit the received data if no error occurred.

@note The BDMA has access to D3 Domain SRAMs and peripherals only, thus aRxBuffer buffer
      has been placed to the D3SRAM (@0x38000000).
			
@note The low power mode example can not be used in DEBUG mode due to the fact 
      that the Cortex-M7 core is no longer clocked during low power mode 
      so debugging features are disabled.
      
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

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

System, Power, PWR, Stop mode, Domain 3, D3SRAM, BDMA, DMAMUX, I2C, transmit, receive, Interrupt, Wakeup, Low Power, EXTI, Button, Led

@par Directory contents 

  - PWR/PWR_Domain3SystemControl/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_Domain3SystemControl/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_Domain3SystemControl/Inc/main.h                   Header file for main.c
  - PWR/PWR_Domain3SystemControl/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file
  - PWR/PWR_Domain3SystemControl/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_Domain3SystemControl/Src/main.c                   Main program
  - PWR/PWR_Domain3SystemControl/Src/stm32h7xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with NUCLEO-H743ZI board and can be
    easily tailored to any other supported device and development board.
  
  - NUCLEO-H743ZI set-up:
    - Connect a wire between 1st board PB.08 pin (SCL pin D15 on CN7) and 
      2nd board PB.08 pin (SCL pin D15 on CN7).
    - Connect a wire between 1st board PB.09 pin (SDA pin D14 on CN7) and 
      2nd board PB.09 pin (SDA pin D14 on CN7).
    - Connect 1st board GND to 2nd board GND.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example



 */
