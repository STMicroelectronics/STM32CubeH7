/**
  @page PWR_D2ON_D1OFF PWR Power D2 domain ON and D1 domain OFF Example
  
  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_D2ON_D1OFF/readme.txt
  * @author  MCD Application Team
  * @brief   Description of PWR_D2ON_D1OFF example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

This example shows how to run the system with only D2 domain in Run mode 
while D1 domain is in Standby mode.

The SystemClock_Config() function is used to set the Flash latency and  to 
configure the system clock :
  - The Cortex-M7 at 400MHz 
  - The Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2  
    peripherals and D3 Domain APB4 peripherals to run at 100MHz.

In this example CM4 boot option should be deactivated by using ST LINK Utility to
guarantee that CM4 will not boot automatically.
To deactivate CM4 boot go to STM32CubeProgrammer->Option_Bytes then deselect
BCM4 from the "User configuration" menu.

After compiling CM4 target the user is aimed to copy the STM32H747I_CM4.bin file 
exist under PWR_D2ON_D1OFF\Toolchain\STM32H745ZI_Nucleo_CM4\Exe\STM32H745ZI_Nucleo_CM4.bin
in the Flash memory at 0x08080000 address by using the STM32CubeProgrammer.
To do this go to STM32CubeProgrammer->Erasing & Programming  then load your binary file and set the start address at 
0x08080000 and finally click Start Programming button.

After a system RESET or a CM7 core debugging the following steps will be done:
  - HAL_Init() function is called to reset all the peripherals, initialize the 
    Flash interface and the systick.
  - SystemClock_Config() function is called to configure the system clock.
  - memcpy function is called to copy the CM4 code from Flash to D2SRAM memory.
  - HAL_SYSCFG_CM4BootAddConfig function is called to configure the CM4 boot address at 
    0x10000000 (here CM4 will boot from D2SRAM memory when it is released by CM7).
  - HAL_RCCEx_EnableBootCore function is called to release and start CM4 boot process.
  - HAL_PWREx_EnterSTANDBYMode function is called to enter D1 domain to STANDBY mode.
  
When HAL_RCCEx_EnableBootCore function is called by CM7 core, the CM4 core will
be released and start booting from 0x10000000 address therefore it will execute  
it is code copied before by CM7 from Flash to D2SRAM memory. 
The CM4 core will then check if D1 is OFF by using __HAL_RCC_GET_FLAG(RCC_FLAG_D1CKRDY) 
and __HAL_PWR_GET_FLAG(PWR_FLAG_SB_D1) macros therefore if D1 is OFF then LED2 will 
toggle each 100 ms to indicate test passed and if D1 is in RUN mode the LED2 will set 
ON to indicate test failed.
            
Two LEDs are used in this example:
  - LED1: Set ON by CM7 core if a configuration error occurred or if D1 domain
          did not enter STANDBY mode.
  - LED2: Toggle by CM4 core each 100 ms if D1 domain is in Standby mode (test passed).
          Set ON by CM4 core if D1 domain is in RUN mode (test failed).

@note This example can not be used in DEBUG mode due to the fact that 
      the Cortex-M7 and Cortex-M4 cores are no longer clocked during 
      low power mode so debugging features are disabled.

@note Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select 
      the RTC clock source; in this case the Backup domain will be reset in  
      order to modify the RTC Clock source, as consequence RTC registers (including 
      the backup registers) and RCC_CSR register are set to their reset values.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Power, STANDBY mode, RUN mode, Domains, multiprocessor

@par Directory contents 

  - PWR/PWR_D2ON_D1OFF/Common/Src/system_stm32h7xx.c    STM32H7xx system configuration source file
  - PWR/PWR_D2ON_D1OFF/Common/Inc/Common.h              CM4 - CM7 common items

  - PWR/PWR_D2ON_D1OFF/CM7/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_D2ON_D1OFF/CM7/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_D2ON_D1OFF/CM7/Inc/main.h                   Header file for main.c
  - PWR/PWR_D2ON_D1OFF/CM7/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_D2ON_D1OFF/CM7/Src/main.c                   Main program

  - PWR/PWR_D2ON_D1OFF/CM4/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_D2ON_D1OFF/CM4/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_D2ON_D1OFF/CM4/Inc/main.h                   Header file for main.c
  - PWR/PWR_D2ON_D1OFF/CM4/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_D2ON_D1OFF/CM4/Src/main.c                   Main program
  
  
@par Hardware and Software environment  

  - This example runs on STM32H747xx/STM32H745xx devices.
  
  - This example has been tested with STMicroelectronics NUCLEO-H745ZI-Q
    boards and can be easily tailored to any other supported device 
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :

 - deactivate CM4 boot : go to STM32CubeProgrammer->Option_Bytes then unselect
   BOOT CM4 from the "User configuration option byte" menu.
   
 - Open your preferred toolchain 
  - For CM4 target configuration (STM32H745ZI_Nucleo_CM4)
     - Rebuild all files.
     - Load the STM32H745ZI_Nucleo_CM4.bin generated file to the Flash memory at 0x08080000 address 
       using the STM32CubeProgrammer.
       Go to Erasing and Programming menu then set the start address at 
       0x08080000, select the file path for STM32H745ZI_Nucleo_CM4.bin generated file and finally click Start Programming button.

  - For CM7 target configuration (STM32H745ZI_Nucleo_CM7) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) 
   and CPU2 (Cortex-M4) at once.
 - Run the example



 */
