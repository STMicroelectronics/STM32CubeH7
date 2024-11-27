/**
  @page CRC_UserDefinedPolynomial User-defined generating polynomial CRC Example
  
  @verbatim
  ******************************************************************************
  * @file    CRC/CRC_UserDefinedPolynomial/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of CRC Example with user-defined generating polynomial.
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

How to configure and use the CRC calculation unit to compute an 8-bit CRC code
for a given data buffer, based on a user-defined generating polynomial. The
peripheral initialization is done using LL unitary service functions for
optimization purposes (performance and size).

The CRC peripheral is configured to work with a user defined polynomial value (8-bit long value). 
In this example, the polynomial is set manually to 0x9B that is X^8 + X^7 + X^4 + X^3 + X + 1.

CPU1 (Cortex-M7)  : At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.

The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz 
  - Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals 
    and D3 Domain APB4 peripherals to run at 100MHz.
    
CPU1 (Cortex-M7) configure the CRC peripheral by HAL_CRC_Init() function.
The latter is calling HAL_CRC_MspInit() function which core is implementing
the configuration of the needed CRC resources according to the used hardware (CLOCK). 
You can update HAL_CRC_Init() input parameters to change the CRC configuration.

The calculated CRC code is stored in uwCRCValue variable.
Once calculated, the CRC value (uwCRCValue) is compared to the CRC expected value (uwExpectedCRCValue).
    
CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.

@note For Cortex-M7, Some code parts (for instance ISR routines, vector table, critical routines )
      can be executed from the ITCM-RAM (64 KB) allowing zero wait state access.
      Which decreases critical task execution time compared to code execution from internal Flash memory. 
      This configuration can be done using '#pragma location = ".itcmram"' to be placed above function declaration, 
      or using the toolchain GUI (file options) to execute a whole source file in the ITCM-RAM.
      For fast data access, critical variables, application RAM, heap, stack could be put as well in the DTCM-RAM (128 KB).

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
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      

STM32H747I-EVAL board's LEDs are used to monitor the example status:
  - LED1 is ON when the correct CRC value is calculated
  - LED3 is ON when there is an error in initialization or if an incorrect CRC value is calculated.

@par Keywords

Security, CRC, CRC Polynomial, IEC 60870-5, hardware CRC, user-defined, generating polynomial

@par Directory contents 
 
  - CRC/CRC_UserDefinedPolynomial/Common/Src/system_stm32h7xx.c       STM32H7xx system configuration file
  
  - CRC/CRC_UserDefinedPolynomial/CM7/Inc/stm32h7xx_hal_conf.h        HAL configuration file for Cortex-M7
  - CRC/CRC_UserDefinedPolynomial/CM7/Inc/stm32h7xx_it.h              Interrupt handlers header file  for Cortex-M7
  - CRC/CRC_UserDefinedPolynomial/CM7/Inc/main.h                      Header for main.c module  for Cortex-M7
  - CRC/CRC_UserDefinedPolynomial/CM7/Src/Src/stm32h7xx_it.c          Interrupt handlers  for Cortex-M7
  - CRC/CRC_UserDefinedPolynomial/CM7/Src/Src/main.c                  Main program for Cortex-M7
  - CRC/CRC_UserDefinedPolynomial/CM7/Src/Src/stm32h7xx_hal_msp.c     HAL MSP module for Cortex-M7

  - CRC/CRC_UserDefinedPolynomial/CM4/Inc/stm32h7xx_hal_conf.h        HAL configuration file for Cortex-M4
  - CRC/CRC_UserDefinedPolynomial/CM4/Inc/stm32h7xx_it.h              Interrupt handlers header file  for Cortex-M4
  - CRC/CRC_UserDefinedPolynomial/CM4/Inc/main.h                      Header for main.c module  for Cortex-M4
  - CRC/CRC_UserDefinedPolynomial/CM4/Src/Src/stm32h7xx_it.c          Interrupt handlers  for Cortex-M4
  - CRC/CRC_UserDefinedPolynomial/CM4/Src/Src/main.c                  Main program for Cortex-M4
     
@par Hardware and Software environment

  - This example runs on STM32H747xx devices.
  - This example has been tested with STM32H747I-EVAL  board with SMPS (SD Converter) power supply config and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H747_EVAL_CM7 and STM32H747_EVAL_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example
 

 */
 
