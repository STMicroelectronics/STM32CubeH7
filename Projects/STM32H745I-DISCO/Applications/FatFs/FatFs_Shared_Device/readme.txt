/**
  @page FatFs_Shared_Device   Concurrent FatFs apps accessing the same eMMC card in read and write access from CM4 and CM7
  @verbatim
  ******************************************************************************
  * @file    FatFs/FatFs_Shared_Device/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FatFs Dual instance application
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

@par Application Description

This application provides a description on how to use STM32Cube™ firmware with FatFs
middleware component as a generic FAT file system module. The objective is to develop an
application making the most of the features offered by FatFs to access the eMMC card.

The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz 
  - Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals 
    and D3 Domain APB4 peripherals to run at 100MHz.

CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) could release CPU2 (Cortex-M4) when needed
by means of HSEM notification or by any D2 Domain wakeup source (SEV,EXTI..).

The above will guarantee that CPU2 (Cortex-M4) code execution starts after system initialization :
(system clock config, external memory configuration..).

After Domain D2 wakeup, if  CPU1 attempts to use any resource from such a domain, 
the access will not be safe until “clock ready flag” of such a domain is set (by hardware). 
The check could be done using this macro : __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY). 
 
 Two FatFs apps run on Cortex M7 and Cortex M4 are accessing the same eMMC
 device in both read and write modes. Below is the detailed scenario.

- the CM7 aquires the HSEM, HSEM_ID_0, creates a Fat filesystem on the eMMC, then
  release the HSEM to wakeup the CM4.
- each Core, the CM7 and the CM4, start its own FatFs Operation as following:
    - Open a file in create/write mode.
    - writes a buffer in the file and close it.
    - reopen the file in read mode
    - read the file content and compare it to the original buffer.
    

It is possible to fine tune needed FatFs features by modifying defines values
in FatFs configuration file “ffconf.h” available under the project includes
directory, in a way to fit the application requirements.

It is possible to monitor the applications progress using a HyperTerminal, as
log messages are printed to show the applications status on both cores.
- HyperTerminal configuration:
    - Data Length = 7 Bits
    - One Stop Bit
    - Odd parity
    - BaudRate = 115200 baud
    - Flow control: None


@note
a second HW semaphore is used sync the UART access between the cores, make sure
to use the same HSEM id.

the mmc diskio driver is also using an internal HSEM, EMMC_HSEM_ID, to sync the concurrent
access on the mmc device. Make sure the this HSEM has a unique ID through all
the CM4 and CM7 project files.

STM32 Disco board's LEDs can be used to monitor the application status:
  - LED1 is toggling when the CM4 application run successfully.
  - LED2 is toggling when the CM7 finishes running the application
    successfully.
  - ON success the content of the created files CM7_FILE and CM4_FILE is
    printed on the HyperTerminal.
  - in case of errors on CM7 or CM4 the LED3 respectively the LED1 is off and
    an error message is printed on the HyperTerminal.

@note 
it is possible to customize the file names and the text buffer to test
with by redefining CM7_FILE, CM7_TEXT, CM4_FILE and CM4_text in main.h files
under the CM7/Inc and CM4/Inc directories.

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

FatFs, SDMMC, eMMC card, File system, FAT Volume, Format, Read, Write, HyperTerminal, multiprocessor

@par Directory contents

  - FatFs_Shared_Device/CM4/Inc/main.h                     CM4 Main program header file 
  - FatFs_Shared_Device/CM4/Inc/stm32h7xx_hal_conf.h       CM4 HAL configuration file
  - FatFs_Shared_Device/CM4/Inc/stm32h7xx_it.h             CM4 Interrupt handlers header file
  - FatFs_Shared_Device/CM4/Src/main.c                     CM4 main program
  - FatFs_Shared_Device/CM4/Src/stm32h7xx_it.c             CM4 Interrupt handlers

  - FatFs_Shared_Device/CM7/Inc/main.h                     CM7 Main program header file
  - FatFs_Shared_Device/CM7/Inc/stm32h7xx_hal_conf.h       CM7 HAL configuration file
  - FatFs_Shared_Device/CM7/Inc/stm32h7xx_it.h             Cortex M7 Interrupt handlers header file
  - FatFs_Shared_Device/CM7/Src/main.c                     CM7 main program
  - FatFs_Shared_Device/CM7/Src/stm32h7xx_it.c             CM7 Interrupt handlers

  - FatFs_Shared_Device/Common/Inc/ffconf.h                Common FatFs configuration file
  - FatFs_Shared_Device/Common/Inc/mmc_diskio.h            FatFs eMMC diskio driver header file
  - FatFs_Shared_Device/Common/Src/mmc_diskio.c            FatFs eMMC diskio driver implementation
  - FatFs_Shared_Device/Common/Src/system_stm32h7xx.c      stm32h7xx system clock configuration file



@par Hardware and Software environment

  - This application runs on STM32H745xx devices.
  - This application has been tested with STMicroelectronics STM32H745I-Disco
    discovery boards. It can be customized to run on other boards using
    differnt storage media.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild CM7 and CM4 applications and load both images into target memory
 - reset the board to get the applications running.


 */
