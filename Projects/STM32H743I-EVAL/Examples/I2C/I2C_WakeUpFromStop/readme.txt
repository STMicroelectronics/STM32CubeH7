/**
  @page I2C_WakeUpFromStop I2C Two Boards Communication IT Example on Stop Mode
  
  @verbatim
  ******************************************************************************
  * @file    I2C/I2C_WakeUpFromStop/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Wake Up from Stop mode example
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

This example describes how to perform I2C data buffer transmission/reception between 
two boards using an interrupt when the device is in STOP mode.

Board: STM32H743I-EVAL (embeds a STM32H743I device)
SCL Pin: PF1 (CN12)
SDA Pin: PF0 (CN12)

   _________________________                       _________________________ 
  |           ______________|                     |______________           |
  |          |I2C2          |                     |          I2C2|          |
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

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz. When The system is wakeup from stop mode,
system clock is 8 Mhz. The I2C peripheral is directly clocked by HSI.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function.
This later is calling the HAL_I2C_MspInit()function which core is implementing
the configuration of the needed I2C resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change I2C configuration.

The I2C communication is then initiated.
The project is split in two parts the Master Board and the Slave Board
- Master Board
  The HAL_I2C_Master_Receive_IT() and the HAL_I2C_Master_Transmit_IT() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Master mode using interrupt.
- Slave Board
  The HAL_I2C_Slave_Receive_IT() and the HAL_I2C_Slave_Transmit_IT() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Slave mode using interrupt.
The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file:
- Uncomment "#define MASTER_BOARD" to select Master board.
- Comment "#define MASTER_BOARD" to select Slave board.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user presses the Tamper push-button on the Master Board, I2C Master
starts the communication by sending aTxBuffer through HAL_I2C_Master_Transmit_IT() to 
I2C Slave which wakes up from stop mode and receives aRxBuffer through HAL_I2C_Slave_Receive_IT(). 
The second step starts when the user presses the Tamper push-button on the Master Board,
the I2C Slave after wake up from stop mode at address match, sends aTxBuffer through HAL_I2C_Slave_Transmit_IT()
to the I2C Master which receives aRxBuffer through HAL_I2C_Master_Receive_IT().

The end of this two steps are monitored through the HAL_I2C_GetState() function
result.
Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to 
check buffers correctness.  

STM32H743I-EVAL's LEDs can be used to monitor the transfer status on the Master Board :
 - LED1 is ON when the transmission process is complete.
 - LED1 is OFF when the reception process is complete.
 - LED3 is ON when there is an error in transmission/reception process.  

@note I2Cx instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The example need to ensure that the SysTick time base is always set to 1 millisecond
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

Connectivity, I2C, Communication, Transmission, Reception, Master, Slave, Wake up, Low-power, Stop mode, Interrupt

@par Directory contents 

  - I2C/I2C_WakeUpFromStop/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - I2C/I2C_WakeUpFromStop/Inc/stm32h7xx_it.h          I2C interrupt handlers header file
  - I2C/I2C_WakeUpFromStop/Inc/main.h                  Header for main.c module  
  - I2C/I2C_WakeUpFromStop/Src/stm32h7xx_it.c          I2C interrupt handlers
  - I2C/I2C_WakeUpFromStop/Src/main.c                  Main program
  - I2C/I2C_WakeUpFromStop/Src/system_stm32h7xx.c      STM32H7xx system source file
  - I2C/I2C_WakeUpFromStop/Src/stm32h7xx_hal_msp.c     HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with STM32H743I-EVAL board and can be
    easily tailored to any other supported device and development board.    

  - STM32H743I-EVAL Set-up
    - Connect I2C_SCL line of Master board (PF1, PIN12 CN12) to I2C_SCL line of Slave Board (PF1, PIN12 CN12).
    - Connect I2C_SDA line of Master board (PF0, PIN14 CN12) to I2C_SDA line of Slave Board (PF0, PIN14 CN12).

    - Connect GND of Master board to GND of Slave Board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example


 */
