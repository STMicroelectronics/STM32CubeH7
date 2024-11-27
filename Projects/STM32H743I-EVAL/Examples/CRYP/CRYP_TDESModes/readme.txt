/**
  @page CRYP_TDESModes  TDES ECB/CBC Example
  
  @verbatim
  ******************************************************************************
  * @file    CRYP/CRYP_TDESModes/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRYP TDES encryption/decryption mode
  *          with ECB and CBC algorithm.
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
TDES ECB and CBC algorithm.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H753xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.


The TDES mode requires:
1. Plaintext : which will be encrypted with several algorithm & keys.    
   Ciphertext_ECB,Ciphertext_CBC : which will be decrypted respectively with ECB
   and CBC algorithm. 

3. InitVector or the Initialization Vector used only for CBC.

4. Key: is the parameter which determines the Ciphertext. In this example key
   size is 128 bits.

CRYP peripheral must be initialized (HAL_CRYP_Init) once from the beginning with
user parameter, then for each operation of encryption/decryption, only configuration
(HAL_CRYP_SetConfig) should be made if needed.
Before setting new configuration via HAL_CRYP_SetConfig function, we should get
the current parameters of CRYP via HAL_CRYP_GetConfig function.
 
The TDES-ECB encryption/decryption in polling mode provide :
1. Encryptedtext: which is the encryption result of Plaintext, it is compared to Ciphertext_ECB.
2. Decryptedtext: which is the Decryption result of Ciphertext_ECB,it is compared to Plaintext.

The TDES-CBC encryption/decryption in polling mode provide :
1. Encryptedtext: which is the encryption result of Plaintext, it is compared to Ciphertext_CBC.
2. Decryptedtext: which is the Decryption result of Ciphertext_CBC,it is compared to Plaintext.
   
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

Security, CRYP, TDES, ECB, CBC, hardware CRYP, 

@par Directory contents 
  
  - CRYP/CRYP_TDESModes/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_TDESModes/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_TDESModes/Inc/main.h                  Header for main.c module
  - CRYP/CRYP_TDESModes/Src/stm32h7xx_it.c          Interrupt handlers
  - CRYP/CRYP_TDESModes/Src/main.c                  Main program
  - CRYP/CRYP_TDESModes/Src/stm32h7xx_hal_msp.c     HAL MSP module 
  - CRYP/CRYP_TDESModes/Src/system_stm32h7xx.c      STM32H7xx system source file

     
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
 
