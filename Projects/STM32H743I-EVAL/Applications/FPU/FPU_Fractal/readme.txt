/**
  @page FPU_Fractal Floating Point Unit application.

  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FPU Fractal application.
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

@par Application Description

  This application explains how to use, and demonstrates the benefits brought by, the STM32H7 floating-point
  units (FPU). The CortexM7 FPU is an implementation of the ARM FPv5 double-precision FPU.

In this application, when the define USE_VOS0_480MHZ_OVERCLOCK (in main.h) is set to zero then :
  - The SystemClock_Config_400MHz() function is used to configure the system clock for STM32H743xx Devices :
    The CPU at 400MHz
    The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
    The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

when the define USE_VOS0_480MHZ_OVERCLOCK (in main.h) is set to one then :
  - The SystemClock_Config_480MHz() function is used to configure the system clock for STM32H743xx Devices :
    The CPU at 480MHz
    The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 240MHz.
    The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 120MHz.

@note when the define USE_VOS0_480MHZ_OVERCLOCK (in main.h) is set to one, user can switch between VOS0 at 480MHz and VOS1 at 400MHz
      by pressing the Tamper button and check the impact on the performances.

@note Over clocking the system to 480MHz is only possible with PWR VOS0 voltage scaling.
      In order to switch to voltage scaling zero (VOS0), the following sequence must be respected :
      - First enable the SYSCFG clock using __HAL_RCC_SYSCFG_CLK_ENABLE macro
      - Using __HAL_PWR_VOLTAGESCALING_CONFIG Set voltage scaling 1 (VOS1) and wait for VOSRDY flag to be set
      - once in VOS1 scale, switch to VOS0 using __HAL_PWR_VOLTAGESCALING_CONFIG and wait again on VOSRDY flag.

@note PWR_REGULATOR_VOLTAGE_SCALE0 is only possible when Vcore is supplied from LDO.
      the SYSCFG Clock must be enabled before selecting PWR_REGULATOR_VOLTAGE_SCALE0
      using macro __HAL_RCC_SYSCFG_CLK_ENABLE().
      Transition to PWR_REGULATOR_VOLTAGE_SCALE0 is only possible when the system is already in
      PWR_REGULATOR_VOLTAGE_SCALE1.

@note Transition from PWR_REGULATOR_VOLTAGE_SCALE0 is only possible to PWR_REGULATOR_VOLTAGE_SCALE1
      then once in PWR_REGULATOR_VOLTAGE_SCALE1 it is possible to switch to another voltage scale.
      After each regulator voltage setting, wait on PWR_FLAG_VOSRDY to be set using macro __HAL_PWR_GET_FLAG.

@note To enter low power mode , and if current regulator voltage is PWR_REGULATOR_VOLTAGE_SCALE0 then first
      switch to PWR_REGULATOR_VOLTAGE_SCALE1 before entering low power mode.

  The application computes a simple mathematical fractal: the Julia set.
  The generation algorithm for such a mathematical object is quite simple: for each point of
  the complex plan, we are evaluating the divergence speed of a defined sequence. The Julia
  set equation for the sequence is: z(n+1) = z(n)^2 + c.
  This value is translated into a color, to show graphically the divergence speed of the points of the complex plan.

  Two workspaces are available to activate or not the FPU during the compilation phase:
  - Without using the FPU, these operations are done by software through the C compiler
  library and are not visible to the programmer; but the performances are very low.
  - When enabling the FPU, all of the real numbers calculations are entirely done by hardware in a
  single cycle, for most of the instructions. The C compiler does not use its own floating-point
  library but directly generates FPU native instructions.

  User might change the number of iterations done while calculating the fractal for richer pixels generated,
  by changing the ITERATION define in main.h

  STM32H743I-EVAL Eval board's LEDs can be used to monitor the application status:
  - LED1 is ON in case of success.
  - LED3 is ON in case of error.

@note For more information on how to use floating-point units (FPU) refer to AN4044 found under:
      http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00047230.pdf

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
                must be  properly defined to be aligned to L1-CACHE line size (32 bytes).
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

System, Cortex, floating-point unit, FPU, Double-precision, Fractal, Julia set, Equation, Complex plan, Real numbers, Pixel, Color, Button

@par Directory contents

  - FPU/FPU_Fractal/Inc/stm32h7xx_hal_conf.h           HAL Configuration file
  - FPU/FPU_Fractal/Inc/stm32h7xx_it.h                 Interrupt handlers header file
  - FPU/FPU_Fractal/Inc/main.h                         Main program header file
  - FPU/FPU_Fractal/Inc/button.h                       Pause/Play and zoom buttons header file
  - FPU/FPU_Fractal/Src/stm32h7xx_it.c                 Interrupt handlers
  - FPU/FPU_Fractal/Src/main.c                         Main program
  - FPU/FPU_Fractal/Src/button.c                       Pause/Play and zoom buttons tables
  - FPU/FPU_Fractal/Src/system_stm32h7xx.c             STM32H7xx system source file

@par Hardware and Software environment

  - This application runs on STM32H743xx Device.

  - This application has been tested with STMicroelectronics STM32H743I-EVAL
    evaluation boards and can be easily tailored to any other supported device and development board.


@par How to use it?

      In order to make the program work, you must do the following :
  - Open your preferred toolchain
  - Select FPU On or FPU Off workspace
  - Rebuild all files and load your image into target memory
  - Run the application


 */
