/**
  @page I2C_WakeUpFromStop I2C Two Instances Communication IT Example on Stop Mode

  @verbatim
  ******************************************************************************
  * @file    I2C/I2C_WakeUpFromStop/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the Wake Up from Stop mode example
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

This example describes how to perform I2C data buffer transmission/reception
between two intances using interrupt when one core is in STOP mode.

Board: STM32H747I-EVAL (embeds a STM32H747I device)
  ________________________________________
 |                                        |
 |    _________               _________   |
 |   |         |             |         |  |
 |   |      SCL|PH11______PF1|SCL      |  |
 |   |         |             |         |  |
 |   |         |             |         |  |
 |   |      SDA|PH12______PF0|SDA      |  |
 |   |_________|             |_________|  |
 |       I2C4                   I2C2      |
 |     (master)                (slave)    |
 |                                        |
 |                                        |
 |  ¤     ¤     ¤                 @       |
 | LED4  LED3  LED1             Tamper    |
 |________________________________________|

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

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function.
This later is calling the HAL_I2C_MspInit()function which core is implementing
the configuration of the needed I2C resources according to the used hardware
(CLOCK, GPIO and NVIC). You may update this function to change I2C configuration.

The I2C communication is then initiated.
The project is split in two parts: the Master instance and the Slave instance.
- Master instance : I2C4 (managed by CM7)
  The HAL_I2C_Master_Receive_IT() and the HAL_I2C_Master_Transmit_IT() functions
  allow respectively the reception and the transmission of a predefined data buffer
  in Master mode using interrupt.
- Slave instance : I2C2 (managed by CM4)
  The HAL_I2C_Slave_Receive_IT() and the HAL_I2C_Slave_Transmit_IT() functions
  allow respectively the reception and the transmission of a predefined data buffer
  in Slave mode using interrupt.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user presses the Tamper push-button on the Master Board, I2C Master
starts the communication by sending aTxBuffer through HAL_I2C_Master_Transmit_IT() to
I2C Slave which wakes up from stop mode and receives aRxBuffer through HAL_I2C_Slave_Receive_IT().
The second step starts when the user presses the Tamper push-button on the Master Board,
the I2C Slave after wake up from stop mode at address match, sends aTxBuffer through HAL_I2C_Slave_Transmit_IT()
to the I2C Master which receives aRxBuffer through HAL_I2C_Master_Receive_IT().

The end of this two steps are monitored through the HAL_I2C_GetState() function
result.
Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to
check buffers correctness.

STM32H747I-EVAL's LEDs can be used to monitor the transfer status:
 - LED1 and LED4 are ON when the masterTX/slaveRX process is complete.
 - LED1 and LED4 are OFF when the masterRX/slaveTX process is complete.
 - LED3 is ON when there is an error in transmission/reception process.

@note I2Cx instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note For the Cortex-M7, if the application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex-M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Connectivity, I2C, Communication, Transmission, Reception, SCL, SDA, Low power, Stop mode, Wake up, Interrupt

@par Directory contents

  - I2C/I2C_WakeUpFromStop/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration source file

  - I2C/I2C_WakeUpFromStop/CM7/Inc/main.h                    Header for main.c module for Cortex-M7
  - I2C/I2C_WakeUpFromStop/CM7/Inc/stm32h7xx_it.h            Interrupt handlers header file for Cortex-M7
  - I2C/I2C_WakeUpFromStop/CM7/Inc/stm32h7xx_hal_conf.h      HAL configuration file for Cortex-M7
  - I2C/I2C_WakeUpFromStop/CM7/Src/stm32h7xx_hal_msp.c       HAL MSP module for Cortex-M7
  - I2C/I2C_WakeUpFromStop/CM7/Src/main.c                    Main program for Cortex-M7
  - I2C/I2C_WakeUpFromStop/CM7/Src/stm32h7xx_it.c            Interrupt handlers for Cortex-M7

  - I2C/I2C_WakeUpFromStop/CM4/Inc/main.h                    Main configuration file for Cortex-M4
  - I2C/I2C_WakeUpFromStop/CM4/Inc/stm32h7xx_it.h            Interrupt handlers header file for Cortex-M4
  - I2C/I2C_WakeUpFromStop/CM4/Inc/stm32h7xx_hal_conf.h      HAL configuration file for Cortex-M4
  - I2C/I2C_WakeUpFromStop/CM4/Src/stm32h7xx_hal_msp.c       HAL MSP module for Cortex-M4
  - I2C/I2C_WakeUpFromStop/CM4/Src/main.c                    Main program  for Cortex-M4
  - I2C/I2C_WakeUpFromStop/CM4/Src/stm32h7xx_it.c            Interrupt handlers for Cortex-M4

@par Hardware and Software environment

  - This example runs on STM32H747xx/STM32H757xx devices.

  - This example has been tested with STM32H747I-EVAL  board with SMPS (SD Converter) power supply config and can be
    easily tailored to any other supported device and development board.

  - STM32H747I-EVAL Set-up
    - Connect I2C4_SCL line -master- (PH11, CN11) to I2C2_SCL line -slave- (PF1, CN12).
    - Connect I2C4_SDA line -master- (PH12, CN11) to I2C2_SDA line -slave- (PF0, CN12).

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - For each target configuration (STM32H747I_EVAL_CM7 and STM32H747I_EVAL_CM4) :
     - Rebuild all files
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example


 */
