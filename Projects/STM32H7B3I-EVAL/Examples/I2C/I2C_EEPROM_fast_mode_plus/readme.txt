/**
  @page I2C_EEPROM_fast_mode_plus I2C EEPROM fast mode plus example
  
  @verbatim
  ******************************************************************************
  * @file    I2C/I2C_EEPROM_fast_mode_plus/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C EEPROM fast mode plus example.
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

How to handle I2C data buffer transmission/reception with DMA. In the example, 
the device communicates with an I2C EEPROM memory.

SCL Pin: PH.4
SDA Pin: PH.5
   __________________________________________________________________________                        
  |           ______________                        ______________           |
  |          | I2C2         |                      |    I2C_EEPROM|          |
  |          |              |                      |              |          |
  |          |           SCL|______________________|CLOCK         |          |
  |          |              |                      |              |          |
  |          |              |                      |              |          |
  |          |              |                      |              |          |
  |          |           SDA|______________________|DATA          |          |
  |          |              |                      |              |          |
  |          |______________|                      |______________|          |
  |                                                                          |
  |__________________________________________________________________________|
      STM32H7B3I-EVAL

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3XXQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280 MHz.
The APB clock dividers for CD  Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD  Domain APB4 peripherals to run at 280 MHz/2.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function.
This later is calling the HAL_I2C_MspInit()function which core is implementing
the configuration of the needed I2C resources according to the used hardware (CLOCK, 
GPIO, DMA and NVIC). You may update this function to change I2C configuration.

The I2C/EEPROM communication is then initiated.
The HAL_I2C_Mem_Read_DMA() and the HAL_I2C_Mem_Write_DMA() functions allow respectively 
the reception of Data from EEPROM and the transmission of a predefined data 
buffer.

For this example the TxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step the I2C writes the aTxBuffer by group of 4 bytes (EEPROM M24LR64
Page size) using HAL_I2C_Mem_Write_DMA() then read back the data through aRxBuffer
using HAL_I2C_Mem_Read_DMA(). 
The end of this two steps are monitored through the Rx (respectively TX) Transfer completed callback.
Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to 
check buffers correctness.  

STM32 Eval board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the transmission process is complete.
 - LED3 is ON when the reception process is complete.
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
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

Connectivity, Memory, EEPROM, I2C, SCL, SDA, Fast mode plus, DMA, Read, Write

@par Directory contents 

  - I2C/I2C_EEPROM_fast_mode_plus/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - I2C/I2C_EEPROM_fast_mode_plus/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - I2C/I2C_EEPROM_fast_mode_plus/Inc/main.h                  Header for main.c module  
  - I2C/I2C_EEPROM_fast_mode_plus/Src/stm32h7xx_it.c          Interrupt handlers
  - I2C/I2C_EEPROM_fast_mode_plus/Inc/stm32h7b3i_eval_conf.h  STM32H7B3I-EVAL board configuration file
  - I2C/I2C_EEPROM_fast_mode_plus/Src/main.c                  Main program
  - I2C/I2C_EEPROM_fast_mode_plus/Src/system_stm32h7xx.c      STM32H7xx system source file
  - I2C/I2C_EEPROM_fast_mode_plus/Src/stm32h7xx_hal_msp.c     HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ devices.
    
  - This example has been tested with STM32H7B3I-EVAL board and can be
    easily tailored to any other supported device and development board.    

   - Make sure the ANT7-M24LR-A eeprom is connected to CN24 of the STM32H7B3I-EVAL board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 



 */
