/**
  @page PWR_VOS0_480MHZ example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_VOS0_480MHZ/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR VOS0 480MHZ example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

In this example, when the define USE_VOS0_480MHZ_OVERCLOCK (in main.h) is set to zero then :
  - The SystemClock_Config_400MHz() function is used to configure the system clock for STM32H743xx Devices :
    The CPU at 400MHz 
    The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
    The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

when the define USE_VOS0_480MHZ_OVERCLOCK (in main.h) is set to one then :
  - The SystemClock_Config_480MHz() function is used to configure the system clock for STM32H743xx Devices :
    The CPU at 480MHz 
    The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 240MHz.
    The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 120MHz.

@note Over clocking the system to 480MHz is only possible with PWR VOS0 voltage scaling. 
      In order to switch to voltage scaling zero (VOS0), the following sequence must be respected :
      - First enable the SYSCFG clock using LL_APB4_GRP1_EnableClock().
      - Using LL_PWR_SetRegulVoltageScaling() Set voltage scaling 1 (VOS1) and wait for VOSRDY flag to be set
      - Once in VOS1 scale, Enable the PWR overdrive.

@note LL_PWR_REGU_VOLTAGE_SCALE0 is only possible when Vcore is supplied from LDO.
      the SYSCFG Clock must be enabled before selecting LL_PWR_REGU_VOLTAGE_SCALE0
      using LL_APB4_GRP1_EnableClock().
      Transition to LL_PWR_REGU_VOLTAGE_SCALE0 is only possible when the system is already in
      LL_PWR_REGU_VOLTAGE_SCALE1.

@note To enter low power mode , and if current regulator voltage is LL_PWR_REGU_VOLTAGE_SCALE0 then first
      switch to LL_PWR_REGU_VOLTAGE_SCALE1 before entering low power mode

@par Keywords

System, Power, PWR, Voltage scaling, VOS0, Over clocking, LDO

@par Directory contents 

  - PWR/PWR_EnterStandbyMode/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - PWR/PWR_EnterStandbyMode/Inc/main.h                  Header for main.c module
  - PWR/PWR_EnterStandbyMode/Inc/stm32_assert.h          Template file to include assert_failed function
  - PWR/PWR_EnterStandbyMode/Src/stm32h7xx_it.c          Interrupt handlers
  - PWR/PWR_EnterStandbyMode/Src/main.c                  Main program
  - PWR/PWR_EnterStandbyMode/Src/system_stm32h7xx.c      STM32H7xx system source file


@par Hardware and Software environment

  - This example runs on STM32H743xx devices.
    
  - This example has been tested with NUCLEO-H743ZI board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
