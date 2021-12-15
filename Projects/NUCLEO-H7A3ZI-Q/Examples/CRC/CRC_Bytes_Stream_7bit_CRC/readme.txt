/**
  @page CRC_Bytes_Stream_7bit_CRC  Bytes Buffers 7-bit CRC Computation Example
  
  @verbatim
  ******************************************************************************
  * @file    CRC/CRC_Bytes_Stream_7bit_CRC/readme.txt 
  * @author  MCD Application Team
  * @brief   7-bit long CRC computation from bytes (8-bit data) buffers.
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

How to configure the CRC using the HAL API. The CRC (cyclic
redundancy check) calculation unit computes 7-bit CRC codes derived from buffers
of 8-bit data (bytes). The user-defined generating polynomial is manually set
to 0x65, that is, X^7 + X^6 + X^5 + X^2 + 1, as used in the Train Communication
Network, IEC 60870-5[17].

At the beginning of the main program, the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is called to configure the system clock for STM32H7A3xxQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280 MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.

The CRC peripheral configuration is ensured by HAL_CRC_Init() function.
The latter is calling HAL_CRC_MspInit() function which core is implementing
the configuration of the needed CRC resources according to the used hardware (CLOCK). 
You can update HAL_CRC_Init() input parameters to change the CRC configuration.

In this example, the user-defined generating polynomial is configured by 
HAL_CRC_Init(). At the same time, it is set that neither input or output data 
must be reversed, the default init value is used and it is specified that input 
data type is byte.

First, a 5-byte long buffer is processed to yield a first CRC.

Next, a second CRC is computed from a 17-byte long buffer. For the latter,
the CRC calculator is not re-initialized and instead the previously computed CRC 
is used as initial value.

Then, a third CRC is computed from a 1-byte long buffer. Again, the CRC calculator 
is not re-initialized, the previously computed CRC is used as initial value.

Finally, a fourth CRC is computed from a 2-byte long buffer. This time, the CRC 
calculator is re-initialized with the IP default value that is 0x7F for a 7-bit CRC.
This is done with a call to HAL_CRC_Calculate() instead of HAL_CRC_Accumulate().  

Each time, the calculated CRC code is stored in uwCRCValue variable.
Once calculated, the CRC value (uwCRCValue) is compared to the CRC expected value (uwExpectedCRCValue_1, 2, 3 or 4).

NUCLEO-H7A3ZI-Q board's LEDs are used to monitor the example status:
  - LED1 is ON when the correct CRC value is calculated
  - LED3 is ON when there is an error in initialization or if an incorrect CRC value is calculated.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
 to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/0x0000000: not cacheable and only accessible
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

Security, CRC, CRC Polynomial, IEC 60870-5

@par Directory contents 
  
  - CRC/CRC_Bytes_Stream_7bit_CRC/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - CRC/CRC_Bytes_Stream_7bit_CRC/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - CRC/CRC_Bytes_Stream_7bit_CRC/Inc/main.h                  Header for main.c module
  - CRC/CRC_Bytes_Stream_7bit_CRC/Inc/stm32h7xx_nucleo_conf.h    HAL configuration file  
  - CRC/CRC_Bytes_Stream_7bit_CRC/Src/stm32h7xx_it.c          Interrupt handlers
  - CRC/CRC_Bytes_Stream_7bit_CRC/Src/main.c                  Main program
  - CRC/CRC_Bytes_Stream_7bit_CRC/Src/stm32h7xx_hal_msp.c     HAL MSP module 
  - CRC/CRC_Bytes_Stream_7bit_CRC/Src/system_stm32h7xx.c      STM32H7xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32H7A3xxQ devices.
  - This example has been tested with NUCLEO-H7A3ZI-Q board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 

 */
 
