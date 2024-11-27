/**
  @page PWR_VOS0_480MHZ PWR VOS0 480MHZ example

  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_VOS0_480MHZ/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the PWR VOS0 480MHZ example.
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

How to over clock the system to 480MHz with VOS0

In this example, when the define USE_VOS0_480MHZ_OVERCLOCK (in main.h) is set to zero then :
  - The SystemClock_Config_400MHz() function is used to set the Flash latency and  to configure the system clock :
    - The Cortex-M7 at 400MHz
    - Cortex-M4 at 200MHz.
    - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals
      and D3 Domain AHB4  peripherals at 200MHz.
    - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals
      and D3 Domain APB4 peripherals to run at 100MHz.

when the define USE_VOS0_480MHZ_OVERCLOCK (in main.h) is set to one then :
  - The SystemClock_Config_480MHz() function is used to set the Flash latency and  to configure the system clock :
    - The Cortex-M7 at 480MHz
    - Cortex-M4 at 240MHz.
    - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals
      and D3 Domain AHB4  peripherals at 240MHz.
    - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals
      and D3 Domain APB4 peripherals to run at 120MHz.

   !!!Attention!!!
   Over clocking the system clock to 480MHz is only available with LDO PWR configuration
    by default The NUCLEO-H745ZI-Q board comes with DIRECT_SMPS configuration
    in order Over clock the system clock to 480MHz to the nucleo board must modified
    to change the PWR path to LDO instead of DIRECT_SMPS

   to do so please change the following solder bridges :
   - LDO config :
        Mount   : SB25, R33(0R), SB74
                  C58 & C54 = 2.2uF instead of 100nF
        Removed : SB92, SB79, R35(0R), SB75, SB76

   - Direct SMPS config (default config):
        Mount   : SB92, SB79, R35(0R), SB75, SB76
                  C58 & C54 = 100nF
        Removed : SB25, R33(0R), SB74

   Note that the Board HW configuration must match with the FW config
   if not will face a deadlock (can't connect the board any more)
   the FW PWR configuration correspond in the main.c to the following :
   Function SystemClock_Config :
     - case of LDO config : HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
     - case of Direct SMPS config : HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

   BOARD_HW_CONFIG_IS_LDO and BOARD_HW_CONFIG_IS_DIRECT_SMPS defines are used to confirm the
   board HW setting :
     - Set BOARD_HW_CONFIG_IS_LDO to 1 and BOARD_HW_CONFIG_IS_DIRECT_SMPS to 0 to confirm that
       the board is configured in LDO

     - Set BOARD_HW_CONFIG_IS_LDO to 0 and BOARD_HW_CONFIG_IS_DIRECT_SMPS to 1 to confirm that
       the board is configured in direct SMPS

   if a dead lock is faced due to a mismatch between the HW board setting and the FW setting (LDO,SMPS)
   user can recover the board by the following :
     - Power off the board
     - Put 3.3V on CN11 connector pin BT0 (Boot 0 pin)
     - This will change the boot pin to 1 instead of 0 and thus the device boot address will
        be changed to boot address 1 making the bootloader starting instead of the FW in the flash
       (FW that is setting a wrong LDO/SMPS config versus the HW board config)

     - Power on the board and connect using STM32CubeProgrammer
     - Erase the user FLash
     - Power off the board and remove the 3.3V input from CN11 connector BT0 pin
       to get back to boot address 0 config
     - The board is now recovered and can proceed normally.

@note Over clocking the system to 480MHz is only possible with PWR VOS0 voltage scaling.
      In order to switch to voltage scaling zero (VOS0), the following sequence must be respected :
      - First enable the CYSCFG clock using __HAL_RCC_SYSCFG_CLK_ENABLE macro
      - Using __HAL_PWR_VOLTAGESCALING_CONFIG Set voltage scaling 1 (VOS1) and wait for VOSRDY flag to be set
      - once in VOS1 scale, switch to VOS0 using __HAL_PWR_VOLTAGESCALING_CONFIG and wait again on VOSRDY flag.

@note PWR_REGULATOR_VOLTAGE_SCALE0 is only possible when Vcore is supplied from LDO.
      the SYSCFG Clock must be enabled before selecting PWR_REGULATOR_VOLTAGE_SCALE0
      using macro   __HAL_RCC_SYSCFG_CLK_ENABLE().
      Transition to PWR_REGULATOR_VOLTAGE_SCALE0 is only possible when the system is already in
      PWR_REGULATOR_VOLTAGE_SCALE1.

@note Transition from PWR_REGULATOR_VOLTAGE_SCALE0 is only possible to PWR_REGULATOR_VOLTAGE_SCALE1
      then once in PWR_REGULATOR_VOLTAGE_SCALE1 it is possible to switch to another voltage scale.
      After each regulator voltage setting, wait on PWR_FLAG_VOSRDY to be set using macro __HAL_PWR_GET_FLAG.

@note To enter low power mode , and if current regulator voltage is PWR_REGULATOR_VOLTAGE_SCALE0 then first
      switch to PWR_REGULATOR_VOLTAGE_SCALE1 before entering low power mode.

CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) will release CPU2 (Cortex-M4) when needed
by means of HSEM notification but release could be also ensured by any Domain D2 wakeup source (SEV,EXTI..).

The above will guarantee that CPU2 (Cortex-M4) code execution starts after system initialization :
(system clock config, external memory configuration..).

After Domain D2 wakeup, if  CPU1 attempts to use any resource from such a domain,
the access will not be safe until “clock ready flag” of such a domain is set (by hardware).
The check could be done using this macro : __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY).

This projects is configured for STM32H745xx devices using STM32CubeH7 HAL and running on
NUCLEO-H745ZI-Q board from STMicroelectronics.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@Note For the Cortex-M7, if the application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex-M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, Power, PWR, Voltage scaling, VOS0, Over clocking, LDO, multiprocessor

@par Directory contents

  - PWR/PWR_VOS0_480MHZ/Common/Src/system_stm32h7xx.c   STM32H7xx system configuration source file
  - PWR/PWR_VOS0_480MHZ/Common/Inc/Common.h             CM4 - CM7 common items

  - PWR/PWR_VOS0_480MHZ/CM7/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M7
  - PWR/PWR_VOS0_480MHZ/CM7/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M7
  - PWR/PWR_VOS0_480MHZ/CM7/Inc/main.h                  Header for main.c module for Cortex-M7
  - PWR/PWR_VOS0_480MHZ/CM7/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M7
  - PWR/PWR_VOS0_480MHZ/CM7/Src/main.c                  Main program for Cortex-M7

  - PWR/PWR_VOS0_480MHZ/CM4/Inc/stm32h7xx_hal_conf.h    HAL configuration file for Cortex-M4
  - PWR/PWR_VOS0_480MHZ/CM4/Inc/stm32h7xx_it.h          Interrupt handlers header file for Cortex-M4
  - PWR/PWR_VOS0_480MHZ/CM4/Inc/main.h                  Header for main.c module for Cortex-M4
  - PWR/PWR_VOS0_480MHZ/CM4/Src/stm32h7xx_it.c          Interrupt handlers for Cortex-M4
  - PWR/PWR_VOS0_480MHZ/CM4/Src/main.c                  Main program for Cortex-M4

@par Hardware and Software environment

  - This example runs on STM32H745xx devices.

  - This example has been tested with NUCLEO-H745ZI-Q board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - For each target configuration (STM32H745ZI_Nucleo_CM7 and STM32H745ZI_Nucleo_CM4) :
     - Rebuild all files
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example



 */
