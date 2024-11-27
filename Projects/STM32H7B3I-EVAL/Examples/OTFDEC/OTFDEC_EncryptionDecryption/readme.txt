/**
  @page OTFDEC_EncryptionDecryption Description of OTFDEC Encryption Decryption example

  @verbatim
  ******************************************************************************
  * @file    OTFDEC_EncryptionDecryption/readme.txt
  * @author  MCD Application Team
  * @brief   This example describes how to encrypt a binary image with the OTFDEC proprietary protection layer over the standard AES in counter mode,
  * @brief   load it in external OSPI memory and execute it using the OTFDEC peripheral to decrypt it on the fly
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

This example shows how to use OTFDEC to encrypt, decrypt and execute PI calculation algorithm stored in external NOR flash
The USART1 is used to verify that decryption and executing instruction are done correctly.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
The CPU at 280MHz
The HCLK for CD Domain AXI and AHB3 peripherals , CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 140MHz.

In this example a code computing PI value has been compiled to be executing from external NOR flash, the hex file has been converted to an array file in ospi1_code.c
The OTFDEC is used to encrypt the binary image using the OTFDEC RSS services resetAndGenerateKeys and resetAndEncrypt. After each call of the OTFDEC service, a system reset is triggered
First, security mode is enable and keys are generated for each OTFDEC region and stored in flash memory at the key context address 0x08010000 (RSS service resetAndGenerateKeys)
The RSS creates a PCROP area at the context address 0x08011000 and the first 16KB of DTCM RAM is protected. A system reset is then triggered
After device rebooted, code to be encrypted is loaded into AXI SRAM address 0x24000000, max size 1024KB, call RSS service resetAndEncrypt to encrypt the binary image to be stored
at the same AXI SRAM address. A system reset is then triggered
After device rebooted, the encrypted code is then flashed to the OSPI1 external flash memory
The secure mode is disable and PCROP area removed
The external NOR flash is configured in memory map mode, the OTFDEC used region is configured with the generated keys of the selected region, and required parameters to decrypt the code
The user application jumps in the external memory and execute the encrypted code which is decrypted on the fly thanks to the OTFDEC
The result of the PI computation is displayed on serial terminal via the UART1 interface

@note The user application data located in DTCM RAM after the first 16KB (0x20004000) to make sure to let this memory area reserved for RSS

@note Make sure Security Option Byte is disable and key context address area (sector 8 at address 0x08010000) erased before launching this example
      STM32 CubeProgrammer can be used to check the Security Option Byte and erase the key context area

@note Some code parts can be executed in the ITCM-RAM (64 KB) which decrease critical task execution time, compared
      to code execution from Flash memory. This feature can be activated using '#pragma location = ".itcmram"' to be
      placed above function declaration, or using the toolchain GUI (file options) to execute a whole source file in
	  the ITCM-RAM.

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
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
      It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
              In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
              Even though the user must manage the cache coherence for read accesses.
              Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
              Please refer to the AN4839 “Level 1 cache on STM32H7 Series”

@par Keywords

Security, Cryptography, OTFDEC, OCTOSPI, CRYP, Cipher, external flash, NOR, execute instruction, decryption, encryption, HyperTerminal

@par Directory contents

  - OTFDEC_OTFDEC_Encryption/Src/main.c                 Main program
  - OTFDEC_OTFDEC_Encryption/Src/stm32h7xx_it.c         Interrupt handlers
  - OTFDEC_OTFDEC_Encryption/Src/stm32h7xx_hal_msp.c    HAL MSP module
  - OTFDEC_OTFDEC_Encryption/Inc/ospi1_code.c           PI calculation algorithm converted in OSPI_Buffer array format from the generated hex file
  - OTFDEC_OTFDEC_Encryption/Src/system_stm32f7xx.c     STM32H7xx system clock configuration file
  - OTFDEC_OTFDEC_Encryption/Inc/main.h                 Main program header file
  - OTFDEC_OTFDEC_Encryption/Inc/stm32h7xx_hal_conf.h   HAL Configuration file
  - OTFDEC_OTFDEC_Encryption/Inc/stm32h7xx_it.h         Interrupt handlers header file
  - OTFDEC_OTFDEC_Encryption/Inc/macronix.h             Macronix header file
  - OTFDEC_OTFDEC_Encryption/Inc/ospi1_code.h           OSPI_Buffer array external declaration
  - OTFDEC_OTFDEC_Encryption/Inc/RSS.h                  RSS header file
  - OTFDEC_OTFDEC_Encryption/Inc/RSS_OTFD.h             RSS OTFDEC header file

@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ devices.

  - This example has been tested with STMicroelectronics STM32H7B3I-EVAL
    board and can be easily tailored to any other supported device
    and development board.

  - STM32H7B3I-EVAL Set-up
    - Make sure that JP4 and JP23 are fitted on OCSPI1 Position.
    - Make sure that jumpers JP20 and JP21 are fitted.

    - Connect the board to an HyperTerminal PC application through USART1 via the STLINK V3 USB to UART connector CN9
      UART configured as follows:
      - Word Length = 8 Bits (7 data bit + 1 parity bit) BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
      - Stop Bit    = One Stop bit
      - Parity      = ODD parity
      - BaudRate    = 115200 baud
      - Hardware flow control disabled (RTS and CTS signals)

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example




 */
