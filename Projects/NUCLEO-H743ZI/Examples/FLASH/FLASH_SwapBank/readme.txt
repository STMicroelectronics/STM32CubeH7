/**
  @page FLASH_SwapBank  FLASH Swap Bank example
  
  @verbatim
  ******************************************************************************
  * @file    FLASH/FLASH_SwapBank/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FLASH Swap Bank example.
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

Guide through the configuration steps to program internal Flash memory bank 1
and bank 2, and to swap between both of them by mean of the FLASH HAL API.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz. 

Below are the steps to run this example:
1- Choose the FLASH_SwapBank_Bank2 project and generate its binary (ie: FLASH_SwapBank.bin)

2- Configure the FLASH memory at Dual Bank mode using STM32CubeProgrammer (Target -> Options Bytes: 
   "SWAP_BANK" should be checked).
   Then, Load FLASH_SwapBank.bin binary at the bank2 of the flash (at the address 0x08100000) using STM32 
   STM32CubeProgrammer(www.st.com) or any similar tool.   

@note:
 - You have to configure your preferred toolchain in order to generate the binary
   file after compiling the project.
   
3- Choose the FLASH_SwapBank_Bank1 project and run it, this project will be loaded
   in the bank1 of the flash: at the address 0x08000000
   
4- Click the BUTTON_USER button and then the RESET button to swap between the two banks,

- If program is executing from bank1 is selected, LED1 toggles,

- If program is executing from bank2 is selected, LED2 toggles,

- If error occurs, LED3 is turn on.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note After executing this example , the SWAP_BANK option byte will be set , 
Use STM32CubeProgrammer to change this option byte back to "Not swapped" if needed.


@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the �MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
����� If the application needs to use DMA(or other masters) based access or requires more RAM, then �the user has to:
����������� � - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
����������� � - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
�������       - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly�defined to be aligned to L1-CACHE line size (32 bytes). 
�
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
������Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
������Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Memory, FLASH, Swap, Bank, Dual bank, Program, Sector, LED

@par Directory contents 

  - FLASH/FLASH_EraseProgram/Inc/stm32h7xx_hal_conf.h        HAL Configuration file  
  - FLASH/FLASH_EraseProgram/Inc/stm32h7xx_it.h              Header for stm32h7xx_it.c
  - FLASH/FLASH_EraseProgram/Inc/main.h                      Header for main.c module 
  - FLASH/FLASH_EraseProgram/Src/stm32h7xx_it.c              Interrupt handlers
  - FLASH/FLASH_EraseProgram/Src/main.c                      Main program
  - FLASH/FLASH_EraseProgram/Src/system_stm32h7xx.c          STM32H7xx system clock configuration file

@par Hardware and Software environment

  - This example runs on STM32H743I devices.
    
  - This example has been tested with NUCLEO-H743ZI board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
 
@ For STM32CubeIDE:
 One linker script is needed for this example where the FLASH shall be defined at origin: 0x8000000
	This is because The CPU expects all code to be executed from this address.
	The SWAP_BANK bit simply “toggles” which bank is located at @0x8000000.
	To actually load one of the binaries into 0x8100000 you have to setup the debug config to load one bin-file with 0x100000:
	select "Debug configuration" or "Run configuration" in function of the active build FLASH_SwapBanks_Bank1 configuration
   - Double click on “STM32 Cortex-M C/C++ Application”
   - Select  “Startup” >  “Add” > 
   - Select the FLASH_SwapBanks_Bank2 project 
   - Uncheck Perform build option
   - Add 0x100000 to Use download offset hex
   - Click Debug/Run to debug/run the example

 */
