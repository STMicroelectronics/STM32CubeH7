/**
  @page CRYP_AESGCM  AES GCM  Example
  
  @verbatim
  ******************************************************************************
  * @file    CRYP/CRYP_AES_CCM/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRYP AES Galois/counter mode (GCM)encryption
  *          decryption mode and TAG generation.
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


How to use the CRYPTO peripheral to encrypt/decrypt data(Plaintext/Ciphertext) using 
AES Galois/counter mode (GCM)and generate the authentication TAG .

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H753xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.


The AES-GCM mode requires:
1. Plaintext/Ciphertext: which will be authenticated and encrypted/decrypted. Its
   size must be a multiple of 4 words. if it's not, which is the case in this 
   example, it must be padded either manually by user ( put input padded Size 
   equal to 16 in this example)or automatically by the driver( in this case put
   real input Size here 15).
   in our example, the size is set to 15, so the padding is done by the driver. 
       
2. HeaderMessage: authenticated header (also knows as Additional Authentication Data AAD)
   this part of the message is only authenticated, not encrypted. 
   HeaderSize is the size of header buffer in word. should be a multiple of 4 words.
   if it's not, which is the case in this example, it must be padded exactly like Plaintext.
   in our example, the headersize is set to 5, so the padding is done by the driver.
    
3. InitVector or the Initial Counter Block (ICB)composed of IV and counter.

4. Key: is the parameter which determines the cipher text. In this example the key
   size is 128 bits = 4 words but it can be tailored to 192 bits and 256 bits.

CRYP peripheral must be initialized once from the beginning, then for each operation of 
encryption/decryption, only configuration should be made if needed.
 
The AES-GCM encryption and TAG generation provides :
1. Encryptedtext: which is the encryption result of Plaintext, it is compared to Ciphertext.
2. TAG: the Message Authentication Code generated after the encryption and it's compared
   to ExpectedTAG.

The AES-GCM decryption and TAG generation provides :
1. Decryptedtext: which is the Decryption result of Ciphertext, it is compared to Plaintext.
2. TAG: the Message Authentication Code generated after the encryption and it's compared
   to ExpectedTAG.
   
STM32H743I-EVAL LEDs are used to monitor the encryption/decryption and TAG generation  status:
 - LED1(GREEN) is ON when encryption/decryption and TAG generation is right.
 - LED3(RED) is ON when encryption or decryption or TAG generation is wrong.
 
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

Security, CRYP, GCM , NIST Special Publication 800-38d, hardware CRYP, 

@par Directory contents 
  
  - CRYP/CRYP_AESGCM/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_AESGCM/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_AESGCM/Inc/main.h                  Header for main.c module
  - CRYP/CRYP_AESGCM/Src/stm32h7xx_it.c          Interrupt handlers
  - CRYP/CRYP_AESGCM/Src/main.c                  Main program
  - CRYP/CRYP_AESGCM/Src/stm32h7xx_hal_msp.c     HAL MSP module 
  - CRYP/CRYP_AESGCM/Src/system_stm32h7xx.c      STM32H7xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32H753xx devices.
  
  - This example has been tested with an STMicroelectronics STM32H753I-EVAL
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 

 */ 
