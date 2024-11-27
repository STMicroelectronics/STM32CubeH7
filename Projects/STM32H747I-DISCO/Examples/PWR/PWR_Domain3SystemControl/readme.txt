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
 
This example shows how to maintain a basic activity of the system in low power 
mode with D3 Domain only by ensuring the communication between the D3SRAM, the BDMA 
and the I2C4 when the system is in Stop mode.

In this example two boards are used to send/receive data by using the I2C4 so  
there are two examples to compile one for each board.
To compile the Domain 3 system control in low power mode example: Uncomment 
LOW_POWER_BOARD define in the common.h file.
To compile the I2C transmit, receive and verify data example: Comment 
LOW_POWER_BOARD define in the main.h file.

First Board:
The first board works in Low Power Mode since the CM7 (D1 Domain), the CM4  
(D2 Domain) and the D3 Domain are in STOP mode. Only D3 Domain will ensure the 
communication between the D3SRAM, the BDMA and the I2C4 when a chunk is received.
After a system start-up, LED1 is turned ON to indicate that the CM7 is in RUN mode
and LED2 is turned ON to indicate that the CM4 is in RUN mode.
After 3 seconds LED2 will turn OFF to indicate that CM4 is entering STOP mode.
After 5 seconds LED1 will turn OFF to indicate that CM7 is entering STOP mode.
When CM4 and CM7 enter STOP mode the user can then start transmission by using
the second board.
After receiving 5 chunks (20 Bytes) the CM7 will wake up and send back the 
received data to the second board and finally it will re-enter to STOP mode.
LED3 will turn ON in case of error during receiving or transmission process.

Second Board:
The second board works in RUN mode. It is used to transmit chunks of 4 Bytes 
to the first board then receive and verify the received data after sending 
5 chunks.
After a system start-up, LED1 is turn ON to indicate that the system is in 
RUN mode.
The user has to press the WAKEUP button to start data transmission. 
LED1 will remain toggling while no error occurred during the transmission and 
reception process.
LED3 will turn ON in case of error during the transmission and reception process.

When the test is started by pressing the WAKEUP button the transmission and 
reception process between the two boards is repeated in an infinite loop.

Board: STM32H747I_Discovery
SCL Pin: PD.12 (pin D15 on CN5 connector)
SDA Pin: PD.13 (pin D14 on CN5 connector)
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
  - Compile the program with LOW_POWER_BOARD define, then load it in the first 
    board (CM7 code : target configuration STM32H747I_DISCO_CM7 and CM4 code target configuration STM32H747I_DISCO_CM4).
  - Compile the program without LOW_POWER_BOARD define, then load it in the 
    second board (CM7 code only : target configuration STM32H747I_DISCO_CM7).
  - Connect the two boards as shown in the picture and start them.
  - LED1 in the first board will set ON for 5 seconds, then it will turn Off 
    to indicate that the CM7 core is enter low power mode.
  - LED2 in the first board will set ON for 3 seconds, then it will turn Off 
    to indicate that the CM4 core is enter low power mode.
  - LED1 in the second board will maintain set to indicate that the system is in
    RUN mode.
  - When LED1 and LED2 of the first board are turned Off the user can start the 
    transmission by pressing on the WAKEUP button in the second board.
    
  => If the test passed, then LED1 on the second board will toggle each 100 ms 
     to indicate that the transmission and reception processes are in progress 
     and no error occurred.
     If an error occurred during the communication LED3 (Red LED) of the second 
     board will turn On.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.

The SystemClock_Config() function is used to set the Flash latency and
to configure the system clock :
  - The Cortex-M7 at 400MHz 
  - The Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals 
    and D3 Domain APB4 peripherals to run at 100MHz.

CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) could release CPU2 (Cortex-M4) when needed
by means of HSEM notification or by any D2 Domain wakeup source (SEV,EXTI..).

The above will guarantee that CPU2 (Cortex-M4) code execution starts after system initialization :
(system clock config, external memory configuration..).

After Domain D2 wakeup, if  CPU1 attempts to use any resource from such a domain, 
the access will not be safe until “clock ready flag” of such a domain is set (by hardware). 
The check could be done using this macro : __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY).

The Domain 3 low power example uses:
 - The BDMA channel 7 configured in peripheral to memory mode to receive I2C data 
   and clear the Domain 3 pending source after waking-up from STOP mode when 
   receiving the I2C4_RX request.
 - The BDMA channel 0 configured in memory to peripheral mode to clear the I2C4 
   address flag when receiving the DMAMUX request 0.
 - The DMAMUX request generator is configured to generate one request (request 0) 
   when receiving the I2C4 wakeup interrupt.

After receiving 5 chunks of data, the BDMA transfer complete interrupt is generated 
and the CM7 is waking up from Stop mode to send-back the received data if no 
error occurred.
Finally the CM7 will go Back to the STOP mode and restart automatically 
the reception process.

The DMAMUX request generator block is configured by using  
"HAL_DMAEx_ConfigMuxRequestGenerator" function with the following parameters :
- SignalID      : set to HAL_DMAMUX2_REQUEST_GEN_I2C4_WKUP which corresponds to 
                  the I2c4_it_async signal.
- Polarity      : Set to RISING to use rising edge for DMA request generation.
- RequestNumber : 1 i.e for each received chunk from the I2C4 one DMA request 
                  is generated.

The DMA request generator is then enabled by using the function
"HAL_DMAEx_EnableMuxRequestGenerator"

The CPU is only used to intercept a DMA transfer error if any and sets the 
LED3 (Red LED) to ON in this case or to transmit the received data if no error occurred.

@note The BDMA has access to D3 Domain SRAMs and peripherals only, thus aRxBuffer buffer
      has been placed to the D3SRAM (@0x38000000).
			
@note This example can not be used in DEBUG mode due to the fact that 
      the Cortex-M7 and Cortex-M4 cores are no longer clocked during 
      low power mode so debugging features are disabled.

@Note For the Cortex-M7, if the application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex-M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"
      
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, Power, PWR, multiprocessor, Stop mode, Domain 3, D3SRAM, BDMA, DMAMUX, I2C, transmit, receive, Interrupt, Wakeup, Low Power, EXTI, Button, Led

@par Directory contents 

  - PWR/PWR_Domain3SystemControl/Common/Src/system_stm32h7xx.c    STM32H7xx system configuration source file
  - PWR/PWR_Domain3SystemControl/Common/Inc/Common.h              CM4 - CM7 common items

  - PWR/PWR_Domain3SystemControl/CM7/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_Domain3SystemControl/CM7/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_Domain3SystemControl/CM7/Inc/main.h                   Header file for main.c
  - PWR/PWR_Domain3SystemControl/CM7/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file
  - PWR/PWR_Domain3SystemControl/CM7/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_Domain3SystemControl/CM7/Src/main.c                   Main program
  - PWR/PWR_Domain3SystemControl/CM7/Src/stm32h7xx_hal_msp.c      HAL MSP module

  - PWR/PWR_Domain3SystemControl/CM4/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_Domain3SystemControl/CM4/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_Domain3SystemControl/CM4/Inc/main.h                   Header file for main.c
  - PWR/PWR_Domain3SystemControl/CM4/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file
  - PWR/PWR_Domain3SystemControl/CM4/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_Domain3SystemControl/CM4/Src/main.c                   Main program
  - PWR/PWR_Domain3SystemControl/CM4/Src/stm32h7xx_hal_msp.c      HAL MSP module
  
@par Hardware and Software environment  

  - This example runs on STM32H747xx/STM32H757xx devices.
    
  - This example has been tested with STM32H747I-DISCO board and can be
    easily tailored to any other supported device and development board.
  
  - STM32H747I_Discovery set-up:
    - Connect a wire between 1st board PD.12 pin (SCL pin D15 on CN15) and 
      2nd board PD.12 pin (SCL pin D15 on CN15).
    - Connect a wire between 1st board PD.13 pin (SDA pin D14 on CN15) and 
      2nd board PD.13 pin (SDA pin D14 on CN15).
    - Connect 1st board GND to 2nd board GND.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H747I_DISCO_CM7 and STM32H747I_DISCO_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example



 */
