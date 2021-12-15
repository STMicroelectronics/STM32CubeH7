/**
  @page CRYP_AES_GCM  Encrypt and Decrypt data using AES Algo using GCM chaining mode
  
  @verbatim
  ******************************************************************************
  * @file    CRYP/CRYP_AES_GCM/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRYP AES Algorithm using GCM chaining mode example
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

How to use the CRYPTO peripheral to encrypt and decrypt data using AES with
Galois/Counter mode (GCM).

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H735xx Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4 peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

The AES-GCM mode requires:
1. Plain text which will be authenticated and encrypted. Its size must be a
   multiple of 16 bytes. So if the original plain text size is not a multiple
   of 16 bytes, it must be padded.
2. Header that will be authenticated but not encrypted. The header size must be
   multiple of 16 bytes whatever the key size.
3. Initialization vector a value that is used only once within a specified
   context. It must be padded to 16-byte length with LSB set to 2. 
4. Key is the parameter which determines the cipher text. In this example the
   key size is 128 bits =16 bytes but it can be tailored to 192 bits or 256 bits.

The AES-GCM provides:
1. Cipher text which is the encryption result of the Plaint text. In this
   example, the cipher text is available in "EncryptedText".
   Its size is the same as the plain text.
2. Plain text which is the decryption result of the Cipher text. In this
   example, the plain text is available in "DecryptedText".
   Its size is the same as the plain text. 
3. TAG: The Message Authentication Code which is used for both message
   authentication and message tampering detection. It's size is always 16 bytes.

LED1 turns on when Encrypted/Decrypted text and computed Tag are correct
    
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

Security, Cryptography, CRYPT, AES, GCM, DMA, AESGCM, MAC, Cypher, Galois, Counter, USART

@par Directory contents 

  - CRYP/CRYP_AES_GCM/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_AES_GCM/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_AES_GCM/Inc/main.h                  Header for main.c module  
  - CRYP/CRYP_AES_GCM/Src/stm32h7xx_it.c          Interrupt handlers
  - CRYP/CRYP_AES_GCM/Src/main.c                  Main program
  - CRYP/CRYP_AES_GCM/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - CRYP/CRYP_AES_GCM/Src/system_stm32h7xx.c      STM32H7xx system source file

@par Hardware and Software environment

  - This example runs on STM32H735xx devices.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example



 */
