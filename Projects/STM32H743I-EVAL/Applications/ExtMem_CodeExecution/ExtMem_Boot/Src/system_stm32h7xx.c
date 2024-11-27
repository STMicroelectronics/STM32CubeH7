/**
  ******************************************************************************
  * @file    system_stm32h7xx.c
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M Device Peripheral Access Layer System Source File.
  *
  *   This file provides two functions and one global variable to be called from
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32h7xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick
  *                                  timer or configure other parameters.
  *
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  *
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
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32h7xx_system
  * @{
  */

/** @addtogroup STM32H7xx_System_Private_Includes
  * @{
  */

#include "stm32h7xx.h"
#include <math.h>

#if !defined  (HSE_VALUE)
#define HSE_VALUE    ((uint32_t)25000000) /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (CSI_VALUE)
  #define CSI_VALUE    ((uint32_t)4000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* CSI_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)64000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

/**
  * @}
  */

/** @addtogroup STM32H7xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32H7xx_System_Private_Defines
  * @{
  */
/*!< Uncomment the following line if you need to use initialized data in D2 domain SRAM  */
/* #define DATA_IN_D2_SRAM */

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00000000UL       /*!< Vector Table base offset field.
                                      This value must be a multiple of 0x200. */
/******************************************************************************/

/**
  * @}
  */

/** @addtogroup STM32H7xx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32H7xx_System_Private_Variables
  * @{
  */
  /* This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetHCLKFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
  */
  uint32_t SystemCoreClock = 64000000;
  uint32_t SystemD2Clock = 64000000;
  const  uint8_t D1CorePrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup STM32H7xx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system
  *         Initialize the FPU setting, vector table location and External memory
  *         configuration.
  * @param  None
  * @retval None
  */
void SystemInit (void)
{
#if defined (DATA_IN_D2_SRAM)
 __IO uint32_t tmpreg;
#endif /* DATA_IN_D2_SRAM */

  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << (10*2))|(3UL << (11*2)));  /* set CP10 and CP11 Full Access */
  #endif
  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set HSION bit */
  RCC->CR |= RCC_CR_HSION;

  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;

  /* Reset HSEON, CSSON , CSION,RC48ON, CSIKERON PLL1ON, PLL2ON and PLL3ON bits */
  RCC->CR &= 0xEAF6ED7FU;

  /* Reset D1CFGR register */
  RCC->D1CFGR = 0x00000000;

  /* Reset D2CFGR register */
  RCC->D2CFGR = 0x00000000;

  /* Reset D3CFGR register */
  RCC->D3CFGR = 0x00000000;

  /* Reset PLLCKSELR register */
  RCC->PLLCKSELR = 0x00000000;

  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x00000000;
  /* Reset PLL1DIVR register */
  RCC->PLL1DIVR = 0x00000000;
  /* Reset PLL1FRACR register */
  RCC->PLL1FRACR = 0x00000000;

  /* Reset PLL2DIVR register */
  RCC->PLL2DIVR = 0x00000000;

  /* Reset PLL2FRACR register */

  RCC->PLL2FRACR = 0x00000000;
  /* Reset PLL3DIVR register */
  RCC->PLL3DIVR = 0x00000000;

  /* Reset PLL3FRACR register */
  RCC->PLL3FRACR = 0x00000000;

  /* Reset HSEBYP bit */
  RCC->CR &= 0xFFFBFFFFU;

  /* Disable all interrupts */
  RCC->CIER = 0x00000000;

  /* Change  the switch matrix read issuing capability to 1 for the AXI SRAM target (Target 7) */
  if((DBGMCU->IDCODE & 0xFFFF0000U) < 0x20000000U)
  {
    /* if stm32h7 revY*/
    /* Change  the switch matrix read issuing capability to 1 for the AXI SRAM target (Target 7) */
    *((__IO uint32_t*)0x51008108) = 0x00000001U;
  }

#if defined (DATA_IN_D2_SRAM)
  /* in case of initialized data in D2 SRAM , enable the D2 SRAM clock */
  RCC->AHB2ENR |= (RCC_AHB2ENR_D2SRAM1EN | RCC_AHB2ENR_D2SRAM2EN | RCC_AHB2ENR_D2SRAM3EN);
  tmpreg = RCC->AHB2ENR;
  (void) tmpreg;
#endif /* DATA_IN_D2_SRAM */


/*
   * Disable the FMC bank1 (enabled after reset).
   * This, prevents CPU speculation access on this bank which blocks the use of FMC during
   * 24us. During this time the others FMC master (such as LTDC) cannot use it!
   */
  FMC_Bank1_R->BTCR[0] = 0x000030D2;

  /* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
  SCB->VTOR = D1_AXISRAM_BASE  | VECT_TAB_OFFSET;       /* Vector Table Relocation in Internal SRAM */
#else
  SCB->VTOR = FLASH_BANK1_BASE | VECT_TAB_OFFSET;       /* Vector Table Relocation in Internal FLASH */
#endif
}

/**
  * @brief  Exit Run* mode and Configure the system Power Supply
  *
  * @note   This function exits the Run* mode and configures the system power supply
  *         according to the definition to be used at compilation preprocessing level.
  *         The application shall set one of the following configuration option:
  *           - PWR_LDO_SUPPLY
  *           - PWR_DIRECT_SMPS_SUPPLY
  *           - PWR_EXTERNAL_SOURCE_SUPPLY
  *           - PWR_SMPS_1V8_SUPPLIES_LDO
  *           - PWR_SMPS_2V5_SUPPLIES_LDO
  *           - PWR_SMPS_1V8_SUPPLIES_EXT_AND_LDO
  *           - PWR_SMPS_2V5_SUPPLIES_EXT_AND_LDO
  *           - PWR_SMPS_1V8_SUPPLIES_EXT
  *           - PWR_SMPS_2V5_SUPPLIES_EXT
  *
  * @note   The function modifies the PWR->CR3 register to enable or disable specific
  *         power supply modes and waits until the voltage level flag is set, indicating
  *         that the power supply configuration is stable.
  *
  * @param  None
  * @retval None
  */
void ExitRun0Mode(void)
{
#if defined(USE_PWR_LDO_SUPPLY)
  #if defined(SMPS)
    /* Exit Run* mode by disabling SMPS and enabling LDO */
    PWR->CR3 = (PWR->CR3 & ~PWR_CR3_SMPSEN) | PWR_CR3_LDOEN;
  #else
    /* Enable LDO mode */
    PWR->CR3 |= PWR_CR3_LDOEN;
  #endif /* SMPS */
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#elif defined(USE_PWR_EXTERNAL_SOURCE_SUPPLY)
  #if defined(SMPS)
    /* Exit Run* mode */
    PWR->CR3 = (PWR->CR3 & ~(PWR_CR3_SMPSEN | PWR_CR3_LDOEN)) | PWR_CR3_BYPASS;
  #else
    PWR->CR3 = (PWR->CR3 & ~(PWR_CR3_LDOEN)) | PWR_CR3_BYPASS;
  #endif /* SMPS */
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#elif defined(USE_PWR_DIRECT_SMPS_SUPPLY) && defined(SMPS)
  /* Exit Run* mode */
  PWR->CR3 &= ~(PWR_CR3_LDOEN);
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#elif defined(USE_PWR_SMPS_1V8_SUPPLIES_LDO) && defined(SMPS)
  /* Exit Run* mode */
  PWR->CR3 |= PWR_CR3_SMPSLEVEL_0 | PWR_CR3_SMPSEN | PWR_CR3_LDOEN;
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#elif defined(USE_PWR_SMPS_2V5_SUPPLIES_LDO) && defined(SMPS)
  /* Exit Run* mode */
  PWR->CR3 |= PWR_CR3_SMPSLEVEL_1 | PWR_CR3_SMPSEN | PWR_CR3_LDOEN;
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#elif defined(USE_PWR_SMPS_1V8_SUPPLIES_EXT_AND_LDO) && defined(SMPS)
  /* Exit Run* mode */
  PWR->CR3 |= PWR_CR3_SMPSLEVEL_0 | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_LDOEN;
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#elif defined(USE_PWR_SMPS_2V5_SUPPLIES_EXT_AND_LDO) && defined(SMPS)
  /* Exit Run* mode */
  PWR->CR3 |= PWR_CR3_SMPSLEVEL_1 | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_LDOEN;
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#elif defined(USE_PWR_SMPS_1V8_SUPPLIES_EXT) && defined(SMPS)
  /* Exit Run* mode */
  PWR->CR3 = (PWR->CR3 & ~(PWR_CR3_LDOEN)) | PWR_CR3_SMPSLEVEL_0 | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_BYPASS;
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#elif defined(USE_PWR_SMPS_2V5_SUPPLIES_EXT) && defined(SMPS)
  /* Exit Run* mode */
  PWR->CR3 = (PWR->CR3 & ~(PWR_CR3_LDOEN)) | PWR_CR3_SMPSLEVEL_1 | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_BYPASS;
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#else
  /* No system power supply configuration is selected at exit Run* mode */
#endif /* USE_PWR_LDO_SUPPLY */
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
