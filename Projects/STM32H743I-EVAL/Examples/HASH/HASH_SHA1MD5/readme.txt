/**
  @page HASH_SHA1MD5  HASH digest calculation using SHA1 and MD5 example
  
  @verbatim
  ******************************************************************************
  * @file    HASH/HASH_SHA1MD5/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the digest calculation using SHA1 and MD5 example.
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

How to use the HASH peripheral to hash data with SHA-1 and MD5 algorithms.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H753xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

For this example, DMA is not used for data transfer, the CPU is using HASH peripheral in
polling mode.

The SHA-1 message digest result is a 160-bit long data and the MD5 message digest result 
is a 128-bit long data.

The expected HASH digests (for SHA1 and MD5) are already computed using an online
HASH tool. Those values are compared to those computed by the HASH peripheral.

In case there is a mismatch the red LED is turned ON (LED3).
In case the SHA1 digest is computed correctly the green LED (LED1) is turned ON.
In case the MD5 digest is computed correctly the blue LED (LED4) is turned ON.

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

System, Security, HASH, SHA1, MD5, digest, 

@par Directory contents 

  - HASH/HASH_SHA1MD5/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - HASH/HASH_SHA1MD5/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - HASH/HASH_SHA1MD5/Inc/main.h                  Header for main.c module  
  - HASH/HASH_SHA1MD5/Src/stm32h7xx_it.c          Interrupt handlers
  - HASH/HASH_SHA1MD5/Src/main.c                  Main program
  - HASH/HASH_SHA1MD5/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - HASH/HASH_SHA1MD5/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

    - This example runs on STM32H753xx devices.
  
  - This example has been tested with STM32H753I-EVAL board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 



 */
 
