/**
  @page DMAMUX_RequestGen DMA & DMAMUX request generator Example

  @verbatim
  ******************************************************************************
  * @file    DMA/DMAMUX_RequestGen/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the DMA & DMAMUX request generator Example.
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

This example shows how to use the DMA with the DMAMUX request generator to generate DMA transfer requests upon
LPTIM2 output signal, knowing that the LPTIM2 is configured in PWM with 2sec period.

The example uses the BDMA channel 0 configured in memory to peripheral mode.
The DMA request is set to the DMAMUX request generator 0.

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

In this basic example he BDMA channel 0 is configured in memory to peripheral mode to ensure data transfer from the source transmission
buffer (SRC_Buffer_LED1_Toggle) to the GPIOJ ODR register (in order to toggle LED1).
The DMA is configured in circular mode so the transfer will restart automatically each time the amount of data
to be transmitted has been reached.

@note the Domain 3 BDMA has access to  Domain 3 SRAMs and peripherals only,  thus the source buffer (SRC_Buffer_LED1_Toggle)
has been placed to the D3 SRAM (@0x38000000)

The DMAMUX request generator block is configured using function "HAL_DMAEx_ConfigMuxRequestGenerator"
with the following parameters :
- SignalID      : set to HAL_DMAMUX2_REQUEST_GEN_LPTIM2_OUT which corresponds to LPTIM2 output signal.
- Polarity      : Set to RISING to use rising edge the LPTIM2 output signal for DMA requests generation.
- RequestNumber : 1 i.e. on each rising edge of the LPTIM2 output signal, a DMA request is generated.

The DMA request generator is then enabled using function “HAL_DMAEx_EnableMuxRequestGenerator”.

The function "LPTIM_Config" is then used to configure the LPTIM2 to generate a PWM with a period of 2sec.
Note that the example doesn’t need to configure a GPIO to LPTIM2 output alternate function as the LPTIM2 output signal
is used internally to trigger the DMAMUX request generator in order to generate DMA transfer requests every 2sec.

Then the DMA transfer is started in non-blocking mode using the HAL function "HAL_DMA_Start_IT"
On each LPTIM2 output event (i.e every 2sec) the DMAMUX request generator generates a DMA request.
As consequence the DMA will serve the request and write a new value to the GPIOJ ODR register to toggle the LED1
without any CPU intervention.
As consequence LED1 will toggle every 2sec.

The CPU is only used to intercept a DMA transfer interrupt error or a DMAMUX overrun interrupt error if any,
and sets the LED2 (Red LED) to On in this case.

STM32 board's LEDs can be used to monitor the transfer status:
 - LED1 toggles every 2 seconds.
 - LED2 is ON when there is an error during the DMA transfer.

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

System, DMA, DMAMUX, Request generator, Data Transfer, Stream

@par Directory contents

  - DMA/DMAMUX_RequestGen/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration source file

  - DMA/DMAMUX_RequestGen/CM7/Inc/main.h                    Header for main.c module for Cortex-M7
  - DMA/DMAMUX_RequestGen/CM7/Inc/stm32h7xx_it.h            Interrupt handlers header file for Cortex-M7
  - DMA/DMAMUX_RequestGen/CM7/Inc/stm32h7xx_hal_conf.h      HAL configuration file for Cortex-M7
  - DMA/DMAMUX_RequestGen/CM7/Src/main.c                    Main program for Cortex-M7
  - DMA/DMAMUX_RequestGen/CM7/Src/stm32h7xx_it.c            Interrupt handlers for Cortex-M7

  - DMA/DMAMUX_RequestGen/CM4/Inc/main.h                    Main configuration file for Cortex-M4
  - DMA/DMAMUX_RequestGen/CM4/Inc/stm32h7xx_it.h            Interrupt handlers header file for Cortex-M4
  - DMA/DMAMUX_RequestGen/CM4/Inc/stm32h7xx_hal_conf.h      HAL configuration file for Cortex-M4
  - DMA/DMAMUX_RequestGen/CM4/Src/main.c                    Main program  for Cortex-M4
  - DMA/DMAMUX_RequestGen/CM4/Src/stm32h7xx_it.c            Interrupt handlers for Cortex-M4

@par Hardware and Software environment

  - This example runs on STM32H745xx/STM32H755xx devices.

  - This example has been tested with STM32H745I-DISCO boards and can be easily
    tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - For each target configuration (STM32H745I_DISCO_CM7 and STM32H745I_DISCO_CM4) :
     - Rebuild all files
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example


 */
