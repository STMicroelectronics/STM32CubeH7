/**
  @page RAMECC_ErrorCount RAMECC Error Count Example 
  
  @verbatim
  ******************************************************************************
  * @file    RAMECC/RAMECC_ErrorCount/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the RAMECC AXI-SRAM Error Count example.
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

How to enable and activate notifications for RAMECC RAMs.

After unplagging and plugging device, all internal RAMs content are not retained.
After activating RAMECC monitor and reading the content of RAMs, the hardware 
will check hamming bits injected and generate an error notification.

This is a false alert due to the fact that the RAM isn't initialized by the 
toolchain(as not used). But this behavior is used to demonstrate how to use the 
HAL RAMECC driver, as it is not easy to provoke a RAMECC failure.

RAMECC notifications can be:
  - Monitor notifications : monitor notification enable interrupts for only 
  selected monitor.
  Monitor notifications can be : * ECC single error.
                                 * ECC double error.
                                 * ECC double error on byte write (BW).
                     
  - Global notifications : global notification enable interrupts for all monitors 
  in RAMECC instance.
  Global notifications can be : * ECC single error.
                                * ECC double error.
                                * ECC double error on byte write (BW).
                                * ECC global.                   
@note ECC global notification enable all notifications for a given RAMECC instance.
  
At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.

The SystemClock_Config() function is used to set the Flash latency and  to 
configure the system clock :
  - The Cortex-M7 at 400MHz 
  - The Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2  
    peripherals and D3 Domain APB4 peripherals to run at 100MHz.

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


  - Scenario : The CM7 core will configure the ramecc monitor, initialize the 
    ramecc peripheral using HAL_RAMECC_Init() API,register the Single or double 
    ECC error detected callback using HAL_RAMECC_RegisterCallback() API, enable 
    notifications using HAL_RAMECC_EnableNotifiaction() API, latching failing 
    errors using HAL_RAMECC_StartMonitor() and reading the hole monitored memory 
    and count the number of detected error.

Two LEDs are used in this program:
  - LED1: To indicate that the hole memory is analyzed.       
  - LED2: To indicate that error occurred during program execution.


@note This example can not be used in DEBUG mode due to the fact that 
      the Cortex-M7 and Cortex-M4 cores are no longer clocked during 
      low power mode so debugging features are disabled.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

SRAM, MONITOR, RAMECC,  

@par Directory contents 

  - RAMECC/RAMECC_ErrorCount/Common/Src/system_stm32h7xx.c    STM32H7xx system configuration source file
  - RAMECC/RAMECC_ErrorCount/Common/Inc/Common.h              CM4 - CM7 common items

  - RAMECC/RAMECC_ErrorCount/CM7/Inc/stm32h7xx_conf.h         HAL Configuration file
  - RAMECC/RAMECC_ErrorCount/CM7/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - RAMECC/RAMECC_ErrorCount/CM7/Inc/main.h                   Header file for main.c
  - RAMECC/RAMECC_ErrorCount/CM7/Src/stm32h7xx_it.c           Interrupt handlers
  - RAMECC/RAMECC_ErrorCount/CM7/Src/main.c                   Main program

  - RAMECC/RAMECC_ErrorCount/CM4/Inc/stm32h7xx_conf.h         HAL Configuration file
  - RAMECC/RAMECC_ErrorCount/CM4/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - RAMECC/RAMECC_ErrorCount/CM4/Inc/main.h                   Header file for main.c
  - RAMECC/RAMECC_ErrorCount/Src/stm32h7xx_it.c               Interrupt handlers
  - RAMECC/RAMECC_ErrorCount/Src/main.c                       Main program
  
@par Hardware and Software environment  

  - This example runs on STM32H745xx device.
  
  - This example has been tested with STMicroelectronics NUCLEO-H745ZI-Q
    boards and can be easily tailored to any other supported device 
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H745ZI_Nucleo_CM7 and STM32H745ZI_Nucleo_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) 
   and CPU2 (Cortex-M4) at once.
 - Run the example



 */
