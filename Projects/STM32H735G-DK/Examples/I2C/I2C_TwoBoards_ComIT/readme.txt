/**
  @page I2C_TwoBoards_ComIT I2C Two Boards Communication IT example
  
  @verbatim
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards_ComIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C Two Boards Communication IT example.
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

How to handle I2C data buffer transmission/reception between two boards, 
using an interrupt.

Board: STM32H735G-DK (embeds a STM32H735IGK6 device)
SCL Pin: PF14 (CN4, D15)
SDA Pin: PF15 (CN4, D14)

   _________________________                       _________________________ 
  |           ______________|                     |______________           |
  |          |I2C4          |                     |          I2C4|          |
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
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 520 MHz.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function.
This later is calling the HAL_I2C_MspInit()function which core is implementing
the configuration of the needed I2C resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change I2C configuration.

The I2C communication is then initiated.
The project is split in two parts: the Master Board and the Slave Board
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
If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.

The user can disable internal pull-up through "#define EXTERNAL_PULL_UP_AVAILABLE"
This help for an integration of this example inside an ecosystem board with external pull-up */

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the User Button on the Master Board,
I2C Master starts the communication by sending aTxBuffer through HAL_I2C_Master_Transmit_IT()
to I2C Slave which receives aRxBuffer through HAL_I2C_Slave_Receive_IT(). 
The second step starts when the user press the User Button on the Master Board,
the I2C Slave sends aTxBuffer through HAL_I2C_Slave_Transmit_IT()
to the I2C Master which receives aRxBuffer through HAL_I2C_Master_Receive_IT().
The end of this two steps are monitored through the HAL_I2C_GetState() function
result.
Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to 
check buffers correctness.  

STM32H735G-DK's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the transmission process is complete.
 - LED1 is OFF when the reception process is complete.
 - LED2 is ON when there is an error in transmission/reception process.  

@note I2Cx instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
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

Connectivity, Communication, I2C, Interrupt, Master, Slave, Transmission, Reception, Fast mode plus

@par Directory contents 

  - I2C/I2C_TwoBoards_ComIT/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - I2C/I2C_TwoBoards_ComIT/Inc/stm32h7xx_it.h          I2C interrupt handlers header file
  - I2C/I2C_TwoBoards_ComIT/Inc/main.h                  Header for main.c module  
  - I2C/I2C_TwoBoards_ComIT/Inc/stm32h735g_discovery_conf.h    HAL configuration file  
  - I2C/I2C_TwoBoards_ComIT/Inc/i2c_timing_utility.h          Header of i2c_timing_utility.c
  - I2C/I2C_TwoBoards_ComIT/Src/i2c_timing_utility.c          Utility to calculate the I2C timing
  - I2C/I2C_TwoBoards_ComIT/Src/stm32h7xx_it.c          I2C interrupt handlers
  - I2C/I2C_TwoBoards_ComIT/Src/main.c                  Main program
  - I2C/I2C_TwoBoards_ComIT/Src/system_stm32h7xx.c      STM32H7xx system source file
  - I2C/I2C_TwoBoards_ComIT/Src/stm32h7xx_hal_msp.c     HAL MSP file    

@par Hardware and Software environment

  - This example runs on STM32H735xx devices.
    
  - This example has been tested with STM32H735G-DK board and can be
    easily tailored to any other supported device and development board.    

  - STM32H735G-DK Set-up
    - Connect I2C_SCL line of Master board (PF14, CN4, D15) to I2C_SCL line of Slave Board (PF14, CN4, D15).
    - Connect I2C_SDA line of Master board (PF15, CN4, D14) to I2C_SDA line of Slave Board (PF15, CN4, D14).
    - Connect GND of Master board to GND of Slave Board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example


 */
