/**
  ******************************************************************************
  * @file    system_stm32h7xx.c
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-Mx Device Peripheral Access Layer System Source File.
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
  * Copyright (c) 2019 STMicroelectronics.
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

/************************* Miscellaneous Configuration ************************/
/*!< Uncomment the following line if you need to use external SRAM or SDRAM mounted
     on EVAL board as data memory  */
/*#define DATA_IN_ExtSRAM */
/*#define DATA_IN_ExtSDRAM*/

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
  const  uint8_t D1CorePrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

/**
  * @}
  */

/** @addtogroup STM32H7xx_System_Private_FunctionPrototypes
  * @{
  */
#if defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM)
  static void SystemInit_ExtMemCtl(void); 
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */

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
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << (10*2))|(3UL << (11*2)));  /* set CP10 and CP11 Full Access */
  #endif

    /*SEVONPEND enabled so that an interrupt coming from the CPU(n) interrupt signal is
     detectable by the CPU after a WFI/WFE instruction.*/ 
 SCB->SCR |= SCB_SCR_SEVONPEND_Msk;

#ifdef CORE_CM7 
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

  /* Enable CortexM7 HSEM EXTI line (line 78)*/
  EXTI_D2->EMR3 |= 0x4000UL;  

  /* Change  the switch matrix read issuing capability to 1 for the AXI SRAM target (Target 7) */
  if((DBGMCU->IDCODE & 0xFFFF0000U) < 0x20000000U)
  {
    /* if stm32h7 revY*/
    /* Change  the switch matrix read issuing capability to 1 for the AXI SRAM target (Target 7) */
    *((__IO uint32_t*)0x51008108) = 0x00000001U;
  }
  

#if defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM)
  SystemInit_ExtMemCtl(); 
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */
 
#endif /* CORE_CM7*/

#ifdef CORE_CM4

  /* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
  SCB->VTOR = D2_AXISRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
  SCB->VTOR = FLASH_BANK2_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif  

#else
#ifdef CORE_CM7

  /* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
  SCB->VTOR = D1_AXISRAM_BASE  | VECT_TAB_OFFSET;       /* Vector Table Relocation in Internal SRAM */
#else
  SCB->VTOR = FLASH_BANK1_BASE | VECT_TAB_OFFSET;       /* Vector Table Relocation in Internal FLASH */
#endif  

#else
#error Please #define CORE_CM4 or CORE_CM7
#endif                       
#endif

}

/**
   * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock , it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *           
  * @note   Each time the core clock changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.         
  *     
  * @note   - The system frequency computed by this function is not the real 
  *           frequency in the chip. It is calculated based on the predefined 
  *           constant and the selected clock source:
  *             
  *           - If SYSCLK source is CSI, SystemCoreClock will contain the CSI_VALUE(*)                                 
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(**)
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(***) 
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the CSI_VALUE(*),
  *             HSI_VALUE(**) or HSE_VALUE(***) multiplied/divided by the PLL factors.
  *
  *         (*) CSI_VALUE is a constant defined in stm32h7xx_hal.h file (default value
  *             4 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.        
  *         (**) HSI_VALUE is a constant defined in stm32h7xx_hal.h file (default value
  *             64 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.   
  *    
  *         (***)HSE_VALUE is a constant defined in stm32h7xx_hal.h file (default value
  *              25 MHz), user has to ensure that HSE_VALUE is same as the real
  *              frequency of the crystal used. Otherwise, this function may
  *              have wrong result.
  *                
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate (void)
{
  uint32_t pllp, pllsource, pllm, pllfracen, hsivalue, tmp;
  uint32_t common_system_clock;
  float_t fracn1, pllvco;

  /* Get SYSCLK source -------------------------------------------------------*/

  switch (RCC->CFGR & RCC_CFGR_SWS)
  {
  case RCC_CFGR_SWS_HSI:  /* HSI used as system clock source */
    common_system_clock = (uint32_t) (HSI_VALUE >> ((RCC->CR & RCC_CR_HSIDIV)>> 3));
    break;

  case RCC_CFGR_SWS_CSI:  /* CSI used as system clock  source */
    common_system_clock = CSI_VALUE;
    break;

  case RCC_CFGR_SWS_HSE:  /* HSE used as system clock  source */
    common_system_clock = HSE_VALUE;
    break;

  case RCC_CFGR_SWS_PLL1:  /* PLL1 used as system clock  source */

    /* PLL_VCO = (HSE_VALUE or HSI_VALUE or CSI_VALUE/ PLLM) * PLLN
    SYSCLK = PLL_VCO / PLLR
    */
    pllsource = (RCC->PLLCKSELR & RCC_PLLCKSELR_PLLSRC);
    pllm = ((RCC->PLLCKSELR & RCC_PLLCKSELR_DIVM1)>> 4)  ;
    pllfracen = ((RCC->PLLCFGR & RCC_PLLCFGR_PLL1FRACEN)>>RCC_PLLCFGR_PLL1FRACEN_Pos);
    fracn1 = (float_t)(uint32_t)(pllfracen* ((RCC->PLL1FRACR & RCC_PLL1FRACR_FRACN1)>> 3));

    if (pllm != 0U)
    {
      switch (pllsource)
      {
        case RCC_PLLCKSELR_PLLSRC_HSI:  /* HSI used as PLL clock source */

        hsivalue = (HSI_VALUE >> ((RCC->CR & RCC_CR_HSIDIV)>> 3)) ;
        pllvco = ( (float_t)hsivalue / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/(float_t)0x2000) +(float_t)1 );

        break;

        case RCC_PLLCKSELR_PLLSRC_CSI:  /* CSI used as PLL clock source */
          pllvco = ((float_t)CSI_VALUE / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/(float_t)0x2000) +(float_t)1 );
        break;

        case RCC_PLLCKSELR_PLLSRC_HSE:  /* HSE used as PLL clock source */
          pllvco = ((float_t)HSE_VALUE / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/(float_t)0x2000) +(float_t)1 );
        break;

      default:
          pllvco = ((float_t)CSI_VALUE / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/(float_t)0x2000) +(float_t)1 );
        break;
      }
      pllp = (((RCC->PLL1DIVR & RCC_PLL1DIVR_P1) >>9) + 1U ) ;
      common_system_clock =  (uint32_t)(float_t)(pllvco/(float_t)pllp);
    }
    else
    {
      common_system_clock = 0U;
    }
    break;

  default:
    common_system_clock = CSI_VALUE;
    break;
  }

  /* Compute SystemClock frequency --------------------------------------------------*/
  tmp = D1CorePrescTable[(RCC->D1CFGR & RCC_D1CFGR_D1CPRE)>> RCC_D1CFGR_D1CPRE_Pos];

  /* common_system_clock frequency : CM7 CPU frequency  */
  common_system_clock >>= tmp;

  /* SystemD2Clock frequency : CM4 CPU, AXI and AHBs Clock frequency  */
  SystemD2Clock = (common_system_clock >> ((D1CorePrescTable[(RCC->D1CFGR & RCC_D1CFGR_HPRE)>> RCC_D1CFGR_HPRE_Pos]) & 0x1FU));

#if defined(DUAL_CORE) && defined(CORE_CM4)
  SystemCoreClock = SystemD2Clock;
#else
  SystemCoreClock = common_system_clock;
#endif /* DUAL_CORE && CORE_CM4 */
}
#if defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM)
/**
  * @brief  Setup the external memory controller.
  *         Called in startup_stm32h7xx.s before jump to main.
  *         This function configures the external memories (SRAM/SDRAM)
  *         This SRAM/SDRAM will be used as program data memory (including heap and stack).
  * @param  None
  * @retval None
  */
void SystemInit_ExtMemCtl(void)
{
  __IO uint32_t tmp = 0;
  
#if defined (DATA_IN_ExtSDRAM) && defined (DATA_IN_ExtSRAM)
  register uint32_t tmpreg = 0, timeout = 0xFFFF;
  register __IO uint32_t index;
  
  /* Enable GPIOD, GPIOE, GPIOF, GPIOG, GPIOH and GPIOI interface 
      clock */
  RCC->AHB4ENR |= 0x000001F8;
  
  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOEEN);
  
  /* Connect PDx pins to FMC Alternate function */     
  GPIOD->AFR[0]  = 0x00CC00CC;
  GPIOD->AFR[1]  = 0xCCCCCCCC;
  /* Configure PDx pins in Alternate function mode */  
  GPIOD->MODER   = 0xAAAAFAFA;
  /* Configure PDx pins speed to 100 MHz */  
  GPIOD->OSPEEDR = 0xFFFF0F0F;
  /* Configure PDx pins Output type to push-pull */  
  GPIOD->OTYPER  = 0x00000000;
  /* Configure PDx pins in Pull-up */
  GPIOD->PUPDR   = 0x55550505;

  /* Connect PEx pins to FMC Alternate function */
  GPIOE->AFR[0]  = 0xC00CC0CC;
  GPIOE->AFR[1]  = 0xCCCCCCCC;
  /* Configure PEx pins in Alternate function mode */ 
  GPIOE->MODER   = 0xAAAABEBA;
  /* Configure PEx pins speed to 100 MHz */ 
  GPIOE->OSPEEDR = 0xFFFFC3CF;
  /* Configure PEx pins Output type to push-pull */  
  GPIOE->OTYPER  = 0x00000000;
  /* Configure PEx pins in Pull-up */
  GPIOE->PUPDR   = 0x55554145;

  /* Connect PFx pins to FMC Alternate function */
  GPIOF->AFR[0]  = 0x00CCCCCC;
  GPIOF->AFR[1]  = 0xCCCCC000;
  /* Configure PFx pins in Alternate function mode */   
  GPIOF->MODER   = 0xAABFFAAA;
  /* Configure PFx pins speed to 100 MHz */ 
  GPIOF->OSPEEDR = 0xFFC00FFF;
  /* Configure PFx pins Output type to push-pull */  
  GPIOF->OTYPER  = 0x00000000;
  /* Configure PFx pins in Pull-up */
  GPIOF->PUPDR   = 0x55400555;

  /* Connect PGx pins to FMC Alternate function */
  GPIOG->AFR[0]  = 0x00CCCCCC;
  GPIOG->AFR[1]  = 0xC0000C0C;
  /* Configure PGx pins in Alternate function mode */ 
  GPIOG->MODER   = 0xBFEEFAAA;
  /* Configure PGx pins speed to 100 MHz */ 
  GPIOG->OSPEEDR = 0xC0330FFF;
  /* Configure PGx pins Output type to push-pull */  
  GPIOG->OTYPER  = 0x00000000;
  /* Configure PGx pins in Pull-up */ 
  GPIOG->PUPDR   = 0x40110555;
  
  /* Connect PHx pins to FMC Alternate function */
  GPIOH->AFR[0]  = 0xCCC00000;
  GPIOH->AFR[1]  = 0xCCCCCCCC;
  /* Configure PHx pins in Alternate function mode */ 
  GPIOH->MODER   = 0xAAAAABFF;
  /* Configure PHx pins speed to 100 MHz */ 
  GPIOH->OSPEEDR = 0xFFFFFC00;
  /* Configure PHx pins Output type to push-pull */  
  GPIOH->OTYPER  = 0x00000000;
  /* Configure PHx pins in Pull-up */
  GPIOH->PUPDR   = 0x55555400;
  
  /* Connect PIx pins to FMC Alternate function */
  GPIOI->AFR[0]  = 0xCCCCCCCC;
  GPIOI->AFR[1]  = 0x00000CC0;
  /* Configure PIx pins in Alternate function mode */ 
  GPIOI->MODER   = 0xFFEBAAAA;
  /* Configure PIx pins speed to 100 MHz */ 
  GPIOI->OSPEEDR = 0x003CFFFF;
  /* Configure PIx pins Output type to push-pull */  
  GPIOI->OTYPER  = 0x00000000;
  /* Configure PIx pins in Pull-up */
  GPIOI->PUPDR   = 0x00145555;
  
  /* Enable the FMC/FSMC interface clock */
  (RCC->AHB3ENR |= (RCC_AHB3ENR_FMCEN));
  
  /* Configure and enable Bank1_SRAM2 */
  FMC_Bank1_R->BTCR[4]  = 0x00001091;
  FMC_Bank1_R->BTCR[5]  = 0x00110212;
  FMC_Bank1E_R->BWTR[4] = 0x0FFFFFFF;
  
  /*SDRAM Timing and access interface configuration*/
  /*LoadToActiveDelay  = 2
    ExitSelfRefreshDelay = 6
    SelfRefreshTime      = 4
    RowCycleDelay        = 6
    WriteRecoveryTime    = 2
    RPDelay              = 2
    RCDDelay             = 2
    SDBank             = FMC_SDRAM_BANK2
    ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_9 
    RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12
    MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_32
    InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4
    CASLatency         = FMC_SDRAM_CAS_LATENCY_2
    WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE
    SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2
    ReadBurst          = FMC_SDRAM_RBURST_ENABLE
    ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0*/
  
  FMC_Bank5_6_R->SDCR[0] = 0x00001800;
  FMC_Bank5_6_R->SDCR[1] = 0x00000165;
  FMC_Bank5_6_R->SDTR[0] = 0x00105000;
  FMC_Bank5_6_R->SDTR[1] = 0x01010351;
  
  /* SDRAM initialization sequence */
  /* Clock enable command */ 
  FMC_Bank5_6_R->SDCMR = 0x00000009; 
  tmpreg = FMC_Bank5_6_R->SDSR & 0x00000020; 
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6_R->SDSR & 0x00000020; 
  }

  /* Delay */
  for (index = 0; index<1000; index++);
  
  /* PALL command */ 
  FMC_Bank5_6_R->SDCMR = 0x0000000A;
  timeout = 0xFFFF;
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6_R->SDSR & 0x00000020; 
  }
  
  FMC_Bank5_6_R->SDCMR = 0x000000EB;
  timeout = 0xFFFF;
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6_R->SDSR & 0x00000020; 
  }

  FMC_Bank5_6_R->SDCMR = 0x0004400C;
  timeout = 0xFFFF;
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6_R->SDSR & 0x00000020; 
  } 
  /* Set refresh count */
  tmpreg = FMC_Bank5_6_R->SDRTR;
  FMC_Bank5_6_R->SDRTR = (tmpreg | (0x00000603<<1));

  /* Disable write protection */
  tmpreg = FMC_Bank5_6_R->SDCR[1]; 
  FMC_Bank5_6_R->SDCR[1] = (tmpreg & 0xFFFFFDFF);

   /*FMC controller Enable*/
  FMC_Bank1_R->BTCR[0]  |= 0x80000000;
  
#elif defined (DATA_IN_ExtSDRAM)
  register uint32_t tmpreg = 0, timeout = 0xFFFF;
  register __IO uint32_t index;

  /* Enable GPIOD, GPIOE, GPIOF, GPIOG, GPIOH and GPIOI interface 
      clock */
  RCC->AHB4ENR |= 0x000001F8;
  
  /* Connect PDx pins to FMC Alternate function */
  GPIOD->AFR[0]  = 0x000000CC;
  GPIOD->AFR[1]  = 0xCC000CCC;
  /* Configure PDx pins in Alternate function mode */  
  GPIOD->MODER   = 0xAFEAFFFA;
  /* Configure PDx pins speed to 100 MHz */  
  GPIOD->OSPEEDR = 0xF03F000F;
  /* Configure PDx pins Output type to push-pull */  
  GPIOD->OTYPER  = 0x00000000;
  /* Configure PDx pins in Pull-up */
  GPIOD->PUPDR   = 0x50150005;
   
  /* Connect PEx pins to FMC Alternate function */
  GPIOE->AFR[0]  = 0xC00000CC;
  GPIOE->AFR[1]  = 0xCCCCCCCC;
  /* Configure PEx pins in Alternate function mode */ 
  GPIOE->MODER   = 0xAAAABFFA;
  /* Configure PEx pins speed to 100 MHz */ 
  GPIOE->OSPEEDR = 0xFFFFC00F;
  /* Configure PEx pins Output type to push-pull */  
  GPIOE->OTYPER  = 0x00000000;
  /* Configure PEx pins in Pull-up */
  GPIOE->PUPDR   = 0x55554005;
  
  /* Connect PFx pins to FMC Alternate function */
  GPIOF->AFR[0]  = 0x00CCCCCC;
  GPIOF->AFR[1]  = 0xCCCCC000;
  /* Configure PFx pins in Alternate function mode */   
  GPIOF->MODER   = 0xAABFFAAA;
  /* Configure PFx pins speed to 100 MHz */ 
  GPIOF->OSPEEDR = 0xFFC00FFF;
  /* Configure PFx pins Output type to push-pull */  
  GPIOF->OTYPER  = 0x00000000;
  /* Configure PFx pins in Pull-up */
  GPIOF->PUPDR   = 0x55400555;
  
  /* Connect PGx pins to FMC Alternate function */
  GPIOG->AFR[0]  = 0x00CCCCCC;
  GPIOG->AFR[1]  = 0xC000000C;
  /* Configure PGx pins in Alternate function mode */ 
  GPIOG->MODER   = 0xBFFEFAAA;
 /* Configure PGx pins speed to 100 MHz */ 
  GPIOG->OSPEEDR = 0xC0030FFF;
  /* Configure PGx pins Output type to push-pull */  
  GPIOG->OTYPER  = 0x00000000;
  /* Configure PGx pins in Pull-up */ 
  GPIOG->PUPDR   = 0x40010555;
  
  /* Connect PHx pins to FMC Alternate function */
  GPIOH->AFR[0]  = 0xCCC00000;
  GPIOH->AFR[1]  = 0xCCCCCCCC;
  /* Configure PHx pins in Alternate function mode */ 
  GPIOH->MODER   = 0xAAAAABFF;
  /* Configure PHx pins speed to 100 MHz */ 
  GPIOH->OSPEEDR = 0xFFFFFC00;
  /* Configure PHx pins Output type to push-pull */  
  GPIOH->OTYPER  = 0x00000000;
  /* Configure PHx pins in Pull-up */
  GPIOH->PUPDR   = 0x55555400;
  
  /* Connect PIx pins to FMC Alternate function */
  GPIOI->AFR[0]  = 0xCCCCCCCC;
  GPIOI->AFR[1]  = 0x00000CC0;
  /* Configure PIx pins in Alternate function mode */ 
  GPIOI->MODER   = 0xFFEBAAAA;
  /* Configure PIx pins speed to 100 MHz */ 
  GPIOI->OSPEEDR = 0x003CFFFF;
  /* Configure PIx pins Output type to push-pull */  
  GPIOI->OTYPER  = 0x00000000;
  /* Configure PIx pins in Pull-up */
  GPIOI->PUPDR   = 0x00145555;
  
/*-- FMC Configuration ------------------------------------------------------*/
  /* Enable the FMC interface clock */
  (RCC->AHB3ENR |= (RCC_AHB3ENR_FMCEN));
  /*SDRAM Timing and access interface configuration*/
  /*LoadToActiveDelay  = 2
    ExitSelfRefreshDelay = 6
    SelfRefreshTime      = 4
    RowCycleDelay        = 6
    WriteRecoveryTime    = 2
    RPDelay              = 2
    RCDDelay             = 2
    SDBank             = FMC_SDRAM_BANK2
    ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_9 
    RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12
    MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_32
    InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4
    CASLatency         = FMC_SDRAM_CAS_LATENCY_2
    WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE
    SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2
    ReadBurst          = FMC_SDRAM_RBURST_ENABLE
    ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0*/
  
  FMC_Bank5_6_R->SDCR[0] = 0x00001800;
  FMC_Bank5_6_R->SDCR[1] = 0x00000165;
  FMC_Bank5_6_R->SDTR[0] = 0x00105000;
  FMC_Bank5_6_R->SDTR[1] = 0x01010351;

  /* SDRAM initialization sequence */
  /* Clock enable command */ 
  FMC_Bank5_6_R->SDCMR = 0x00000009; 
  tmpreg = FMC_Bank5_6_R->SDSR & 0x00000020; 
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6_R->SDSR & 0x00000020; 
  }

  /* Delay */
  for (index = 0; index<1000; index++);
  
  /* PALL command */ 
  FMC_Bank5_6_R->SDCMR = 0x0000000A;
  timeout = 0xFFFF;
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6_R->SDSR & 0x00000020; 
  }
  
  FMC_Bank5_6_R->SDCMR = 0x000000EB;
  timeout = 0xFFFF;
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6_R->SDSR & 0x00000020; 
  }

  FMC_Bank5_6_R->SDCMR = 0x0004400C;
  timeout = 0xFFFF;
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6_R->SDSR & 0x00000020; 
  } 
  /* Set refresh count */
  tmpreg = FMC_Bank5_6_R->SDRTR;
  FMC_Bank5_6_R->SDRTR = (tmpreg | (0x00000603<<1));

  /* Disable write protection */
  tmpreg = FMC_Bank5_6_R->SDCR[1]; 
  FMC_Bank5_6_R->SDCR[1] = (tmpreg & 0xFFFFFDFF);

   /*FMC controller Enable*/
  FMC_Bank1_R->BTCR[0]  |= 0x80000000;

#elif defined(DATA_IN_ExtSRAM)
/*-- GPIOs Configuration -----------------------------------------------------*/
   /* Enable GPIOD, GPIOE, GPIOF and GPIOG interface clock */
  RCC->AHB4ENR   |= 0x00000078;
  
  /* Connect PDx pins to FMC Alternate function */     
  GPIOD->AFR[0]  = 0x00CC00CC;
  GPIOD->AFR[1]  = 0xCCCCCCCC;
  /* Configure PDx pins in Alternate function mode */  
  GPIOD->MODER   = 0xAAAAFABA;
  /* Configure PDx pins speed to 100 MHz */  
  GPIOD->OSPEEDR = 0xFFFF0F0F;
  /* Configure PDx pins Output type to push-pull */  
  GPIOD->OTYPER  = 0x00000000;
  /* Configure PDx pins in Pull-up */ 
  GPIOD->PUPDR   = 0x55550505;

  /* Connect PEx pins to FMC Alternate function */
  GPIOE->AFR[0]  = 0xC00CC0CC;
  GPIOE->AFR[1]  = 0xCCCCCCCC;
  /* Configure PEx pins in Alternate function mode */ 
  GPIOE->MODER   = 0xAAAABEBA;
  /* Configure PEx pins speed to 100 MHz */ 
  GPIOE->OSPEEDR = 0xFFFFC3CF;
  /* Configure PEx pins Output type to push-pull */  
  GPIOE->OTYPER  = 0x00000000;
  /* Configure PEx pins in Pull-up */
  GPIOE->PUPDR   = 0x55554145;

  /* Connect PFx pins to FMC Alternate function */
  GPIOF->AFR[0]  = 0x00CCCCCC;
  GPIOF->AFR[1]  = 0xCCCC0000;
  /* Configure PFx pins in Alternate function mode */   
  GPIOF->MODER   = 0xAAFFFAAA;
  /* Configure PFx pins speed to 100 MHz */ 
  GPIOF->OSPEEDR = 0xFF000FFF;
  /* Configure PFx pins Output type to push-pull */  
  GPIOF->OTYPER  = 0x00000000;
  /* Configure PFx pins in Pull-up */ 
  GPIOF->PUPDR   = 0x55000555;

  /* Connect PGx pins to FMC Alternate function */
  GPIOG->AFR[0]  = 0x00CCCCCC;
  GPIOG->AFR[1]  = 0x00000C00;
  /* Configure PGx pins in Alternate function mode */ 
  GPIOG->MODER   = 0xFFEFFAAA;
  /* Configure PGx pins speed to 100 MHz */ 
  GPIOG->OSPEEDR = 0x00300FFF;
  /* Configure PGx pins Output type to push-pull */  
  GPIOG->OTYPER  = 0x00000000;
  /* Configure PGx pins in Pull-up */ 
  GPIOG->PUPDR   = 0x00100555;

/*-- FMC/FSMC Configuration --------------------------------------------------*/
  /* Enable the FMC/FSMC interface clock */
  (RCC->AHB3ENR |= (RCC_AHB3ENR_FMCEN));

  /* Configure and enable Bank1_SRAM2 */
  FMC_Bank1_R->BTCR[4]  = 0x00001091;
  FMC_Bank1_R->BTCR[5]  = 0x00110212;
  FMC_Bank1E_R->BWTR[4] = 0x0FFFFFFF;  
  
  /*FMC controller Enable*/
  FMC_Bank1_R->BTCR[0]  |= 0x80000000;  

#endif /* DATA_IN_ExtSRAM */
  
  (void)(tmp);
}
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */

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
