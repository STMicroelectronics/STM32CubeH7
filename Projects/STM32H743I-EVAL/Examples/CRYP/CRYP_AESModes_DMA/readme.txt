/**
  @page CRYP_AESModes_DMA  AES ECB in DMA mode  Example
  
  @verbatim
  ******************************************************************************
  * @file    CRYP/CRYP_AES_CCM/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRYP AES ECB encryption/decryption in DMA mode
  *          
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


How to use the CRYPTO peripheral to encrypt/decrypt data(Plaintext/Ciphertext) using AES
ECB algorithm  in DMA mode with swapping.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H753xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.


The AES ECB requires:
1. Plaintext     : which will be encrypted with ECB algorithm & 128bits key.    
   Ciphertext    : which will be decrypted with ECB algorithm & 128bits key.
   Data Byte swapping (DataType 8):
   Plaintext_8   : which will be encrypted with ECB algorithm & 128bits key.    
   Ciphertext_8  : which will be decrypted with ECB algorithm & 128bits key.  
   Data half-word swapping (DataType 16):
   Plaintext_16  : which will be encrypted with ECB algorithm & 128bits key.    
   Ciphertext_16 : which will be decrypted with ECB algorithm & 128bits key.
   Data bit swapping (DataType 1):
   Plaintext_1   : which will be encrypted with ECB algorithm & 128bits key.    
   Ciphertext_1  : which will be decrypted with ECB algorithm & 128bits key.
   
2. Key: is the parameter which determines the Ciphertext. In this example 128 bits
   key is used

CRYP peripheral must be initialized once from the beginning, then for each 
operation of encryption/decryption, only configuration should be made if needed.
 
The AES-ECB encryption/decryption in DMA mode provide :
   Encryptedtext: which is the encryption result of Plaintext, it is compared
   to Ciphertext.
   Decryptedtext: which is the Decryption result of Ciphertext,it is compared
   to Plaintext.
1. No swapping, Encryptedtext compared to Ciphertext. then Decryptedtext compared to Plaintext.
2. Byte swapping, Encryptedtext compared to Ciphertext_8. then Decryptedtext compared to Plaintext_8.
3. half-word swapping, Encryptedtext compared to Ciphertext_16. then Decryptedtext compared to Plaintext_16.
4. Bit swapping, Encryptedtext compared to Ciphertext_1. then Decryptedtext compared to Plaintext_1.

STM32H743I-EVAL LEDs are used to monitor the encryption/decryption status:
 - LED1(GREEN) is ON when encryption/decryption and TAG generation is right.
 - LED3(RED) is ON when encryption or decryption or TAG generation is wrong.
 
@note CPU Data Cache maintenance :
      It is recommended to clean the CPU Data cache before starting the 
       DMA transfer. As the source buffer may be prepared by the CPU, this 
       guarantees that the source buffer (if located in the D1 AXI-SRAM which is
       cashable) will be up to date before starting the transfer.

      It is recommended to invalidate the CPU Data cache after the DMA transfer. 
      As the destination buffer may be used by the CPU, this guarantees Up-to-date
      data when CPU access to the destination buffer located in the D1 AXI-SRAM
      (which is cashable).	 
      
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

Security, CRYP, AES, ECB, NIST FIPS publication 197, hardware CRYP, 

@par Directory contents 
  
  - CRYP/CRYP_AESModes_DMA/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_AESModes_DMA/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_AESModes_DMA/Inc/main.h                  Header for main.c module
  - CRYP/CRYP_AESModes_DMA/Src/stm32h7xx_it.c          Interrupt handlers
  - CRYP/CRYP_AESModes_DMA/Src/main.c                  Main program
  - CRYP/CRYP_AESModes_DMA/Src/stm32h7xx_hal_msp.c     HAL MSP module 
  - CRYP/CRYP_AESModes_DMA/Src/system_stm32h7xx.c      STM32H7xx system source file

     
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
 
