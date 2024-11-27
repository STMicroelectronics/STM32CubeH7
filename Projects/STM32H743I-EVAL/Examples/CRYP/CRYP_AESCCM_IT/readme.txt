/**
  @page CRYP_AESCCM_IT  AES CCM  Example using IT
  
  @verbatim
  ******************************************************************************
  * @file    CRYP/CRYP_AES_CCM_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRYP AES CCM encryption/decryption and TAG 
  *          generation.
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


How to use the CRYPTO peripheral to encrypt/decrypt data(Plaintext/Ciphertext) in
interrupt mode using AES with Combined Cipher Machine (CCM) then generate the authentication TAG .

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H753xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.


The AES-CCM mode requires:
1. Plaintext/Ciphertext: which will be authenticated and encrypted/decrypted. 
   It's size must be a multiple of 4 words. if it's not it must be padded.
    
2. B1 block or Header ( also named associated data): that will be authenticated 
   but not encrypted.

3. B0 block : first block B0 is formatted by Flags, Nonce and the octet length
   of Plaintext.

4. Key: is the parameter which determines the cipher text. In this example the key
   size is 192 bits = 6 words.

CRYP peripheral must be initialized once from the beginning, then for each operation of 
encryption/decryption, only configuration(HAL_CRYP_SetConfig) should be made if needed.
 
The AES-CCM encryption and TAG generation provides :
1. Encryptedtext: which is the encryption result of Plaintext, it is compared to Ciphertext.
2. TAG: the Message Authentication Code generated after the encryption and it's compared
   to ExpectedTAG.

The AES-CCM decryption and TAG generation provides :
1. Decryptedtext: which is the Decryption result of Ciphertext, it is compared to Plaintext.
2. TAG: the Message Authentication Code generated after the encryption and it's compared
   to ExpectedTAG.

 The encryption/decryption is performed in non-blocking mode using the HAL function
 "HAL_CRYP_Encrypt_IT" and "HAL_CRYP_Decrypt_IT". when compute is complete the
 output FIFO transfer completed callbacks set variable  "CrypCompleteDetected" 
 to 1 then result is compared to the expected buffer. 
   
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

Security, CRYP, CCM , NIST Special Publication 800-38C, hardware CRYP, 

@par Directory contents 
  
  - CRYP/CRYP_AESCCM_IT/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_AESCCM_IT/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_AESCCM_IT/Inc/main.h                  Header for main.c module
  - CRYP/CRYP_AESCCM_IT/Src/stm32h7xx_it.c          Interrupt handlers
  - CRYP/CRYP_AESCCM_IT/Src/main.c                  Main program
  - CRYP/CRYP_AESCCM_IT/Src/stm32h7xx_hal_msp.c     HAL MSP module 
  - CRYP/CRYP_AESCCM_IT/Src/system_stm32h7xx.c      STM32H7xx system source file

     
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
 
