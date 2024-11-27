/**
  @page OTFDEC_ExecutingCryptedInstruction Description of OTFDEC Executing Crypetd Instruction example

  @verbatim
  ******************************************************************************
  * @file    OTFDEC_ExecutingCryptedInstruction/readme.txt
  * @author  MCD Application Team
  * @brief   This example describes how to execute an encrypted instruction located on the OCTOSPI external flash using the OTFDEC peripheral.
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
This example shows how to use OTFDEC to decrypt and execute encrypted instruction stored in external NOR flash.
instruction used in this example are already encrypted with the embedded crypto IP standard AES-128 counter mode.
You can also use CRYP for encrypting any other instruction. But it is mandatory to uncomment "#define HAL_CRYP_MODULE_ENABLED", in the stm32h7xx_hal_conf.h
You can refer to OTFDEC_ExcutingAesInstructions example to see how to encrypt with the embedded crypto IP.
The USART1 is used to verify that decryption and executing instruction are done correctly.


This projects is configured for STM32H7B3xxQ devices using STM32CubeH7 HAL and running on
STM32H7B3I-EVAL board from STMicroelectronics.
The example requires to connect the board to an HyperTerminal PC application through USART1 connector.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280 MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.

This example begin with writing encrypted instruction in external NOR flash through OCTOSPI.
These encrypted instruction will be seen like "00" in memory mapped mode (instruction only mode).
Then, OTFDEC will decrypt instruction which will be executed.
The result of executing this instruction (a+b+15) will be displayed on an Hyperterminal through USART1.

@note it has to be considered that keys are entered as big endian format at the level of OTFDEC.

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

Security, Cryptography, OTFDEC, OCTOSPI, CRYP, Cipher, AES, external flash, execute instruction, decryption, NOR flash, HyperTerminal

@par Directory contents

  - Examples/OTFDEC/OTFDEC_Executing-Crypetd_Instruction/Src/main.c                 Main program
  - Examples/OTFDEC/OTFDEC_Executing-Crypetd_Instruction/Src/stm32h7xx_it.c         Interrupt handlers
  - Examples/OTFDEC/OTFDEC_Executing-Crypetd_Instruction/Src/stm32h7xx_hal_msp.c    HAL MSP module
  - Examples/OTFDEC/OTFDEC_Executing-Crypetd_Instruction/Src/hal_octospi_utility.c  Octospi Utility
  - Examples/OTFDEC/OTFDEC_Executing-Crypetd_Instruction/Inc/main.h                 Main program header file
  - Examples/OTFDEC/OTFDEC_Executing-Crypetd_Instruction/Inc/hal_octospi_utility.h  Octospi Utility header file
  - Examples/OTFDEC/OTFDEC_Executing-Crypetd_Instruction/Inc/stm32h7xx_hal_conf.h   HAL Configuration file
  - Examples/OTFDEC/OTFDEC_Executing-Crypetd_Instruction/Inc/stm32h7xx_it.h         Interrupt handlers header file
  - Examples/OTFDEC/OTFDEC_Executing-Crypetd_Instruction/Inc/stm32h7b3i_eval_conf.h STM32H7B3I_EVAL board configuration file
  - Examples/OTFDEC/OTFDEC_Executing-Crypetd_Instruction/Inc/hal_macronix.h         Macronix file
  - Examples/OTFDEC/OTFDEC_Executing-Crypetd_Instruction/Inc/ft5336_conf.h          Specific configuration for the ft5336.c that can be modified by user.
  - Examples/OTFDEC/OTFDEC_Executing-Crypetd_Instruction/Inc/is42s32800j_conf.h     Configurations required for the IS42S32800J SDRAM memory.

@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ devices.

  - This example has been tested with STMicroelectronics STM32H7B3I-EVAL
    board and can be easily tailored to any other supported device
    and development board.

  - Note that the following resistors dedicated for OSPI2 should be fitted :
    R287 - R354 - R319 - R318 - R98 - R89 - R82 - R75 - R76 - R77 - R115 - R257

  - STM32H7B3I-EVAL Set-up
    - Make sure that you have placed the BGA24 OCTOSPI memory module with Macronix MX25LM51245G in the OCSPI2 slot.
    - Make sure that jumpers JP20 and JP21 are fitted.
      Connect the STM32H7B3I-EVAL board through 'USB micro A-Male to A-Male'
      cable to the ST-LINK V3.0 connector to the PC serial port if you want
      to display data on the HyperTerminal.

  - HyperTerminal configuration
      - Word Length = 8 Bits (7 data bit + 1 parity bit) : 
       BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
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
