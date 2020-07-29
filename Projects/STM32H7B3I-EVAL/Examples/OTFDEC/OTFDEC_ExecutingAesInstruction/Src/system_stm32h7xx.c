/**
  ******************************************************************************
  * @file    system_stm32h7xx.c
  * @author  MCD Application Team
  * @version V0.3.0
  * @date    09-December-2016
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
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

#include "stm32h7xx_hal.h"

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

#if defined(DATA_IN_ExtSRAM) && defined(DATA_IN_ExtSDRAM)
 #error "Please select DATA_IN_ExtSRAM or DATA_IN_ExtSDRAM " 
#endif /* DATA_IN_ExtSRAM && DATA_IN_ExtSDRAM */

#if defined(DATA_IN_ExtSDRAM)
#define FMC_SDRAM_CMD_NORMAL_MODE             ((uint32_t)0x00000000U)
#define FMC_SDRAM_CMD_CLK_ENABLE              ((uint32_t)0x00000001U)
#define FMC_SDRAM_CMD_PALL                    ((uint32_t)0x00000002U)
#define FMC_SDRAM_CMD_AUTOREFRESH_MODE        ((uint32_t)0x00000003U)
#define FMC_SDRAM_CMD_LOAD_MODE               ((uint32_t)0x00000004U)
#define FMC_SDRAM_CMD_SELFREFRESH_MODE        ((uint32_t)0x00000005U)
#define FMC_SDRAM_CMD_POWERDOWN_MODE          ((uint32_t)0x00000006U)
#define FMC_SDRAM_CMD_TARGET_BANK2            ((uint32_t)0x00000008U)
#define FMC_SDRAM_CMD_TARGET_BANK1            ((uint32_t)0x00000010U)
#endif

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */		 
#if defined(USE_CODE_IN_ITCM) || defined(USE_CODE_IN_SRAM1) || defined(USE_CODE_IN_SRAM2) || defined(USE_CODE_IN_SRAM3) || defined(USE_CODE_IN_AHB_SRAM1) 
#define VECT_TAB_SRAM
#endif																			
#define VECT_TAB_OFFSET  0x00       /*!< Vector Table base offset field. 
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
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif
  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set HSION bit */
  RCC->CR |= RCC_CR_HSION;
  
  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;

  /* Reset HSEON, CSSON , CSION,RC48ON, CSIKERON PLL1ON, PLL2ON and PLL3ON bits */
  RCC->CR &= (uint32_t)0xEAF6ED7F;

  /* Reset CDCFGR1 register */
  RCC->CDCFGR1 = 0x00000000;

  /* Reset CDCFGR2 register */
  RCC->CDCFGR2 = 0x00000000;
  
  /* Reset SRDCFGR register */
  RCC->SRDCFGR = 0x00000000;

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
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Disable all interrupts */
  RCC->CIER = 0x00000000;

/*
   * Disable the FMC bank1 (enabled after reset).
   * This, prevents CPU speculation access on this bank which blocks the use of FMC during
   * 24us. During this time the others FMC master (such as LTDC) cannot use it!
   */
  FMC_Bank1_R->BTCR[0] = 0x000030D2;

#if defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM)
  SystemInit_ExtMemCtl(); 
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */
         

  /* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
#if defined(USE_CODE_IN_ITCM)
  SCB->VTOR = CD_ITCMRAM_BASE  | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal ITCMSRAM */
#elif defined(USE_CODE_IN_SRAM1)	
  SCB->VTOR = CD_AXISRAM1_BASE  | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal AXISRAM1 */
#elif defined(USE_CODE_IN_SRAM2)	
  SCB->VTOR = CD_AXISRAM2_BASE  | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal AXISRAM2 */
#elif defined(USE_CODE_IN_SRAM3)	
  SCB->VTOR = CD_AXISRAM3_BASE  | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal AXISRAM3 */
#elif defined(USE_CODE_IN_AHB_SRAM1)	
  SCB->VTOR = CD_AHBSRAM1_BASE  | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal AHBSRAM1 */
#endif	
#else
#if defined(USE_CODE_IN_OSPI1)
  SCB->VTOR = OCTOSPI1_BASE | VECT_TAB_OFFSET;      /* Vector Table Relocation in OSPI1 */
#elif defined(USE_CODE_IN_OSPI2)
  SCB->VTOR = OCTOSPI2_BASE | VECT_TAB_OFFSET;      /* Vector Table Relocation in OSPI2 */
#else
  SCB->VTOR = FLASH_BANK1_BASE | VECT_TAB_OFFSET;       /* Vector Table Relocation in Internal FLASH */
#endif	
#endif  

#if defined(USE_AHBSRAM)	
	/* Enable AHB SRAM1 and SRAM2 */
	__HAL_RCC_D2SRAM1_CLK_ENABLE();
	__HAL_RCC_D2SRAM2_CLK_ENABLE();
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
uint32_t pllp = 2, pllsource = 0, pllm = 2 ,tmp, pllfracen  =0 , hsivalue = 0;
float fracn1, pllvco = 0 ;

  /* Get SYSCLK source -------------------------------------------------------*/

  switch (RCC->CFGR & RCC_CFGR_SWS)
  {
  case 0x00:  /* HSI used as system clock source */
    SystemCoreClock = (uint32_t) (HSI_VALUE >> ((RCC->CR & RCC_CR_HSIDIV)>> 3));
    break;

  case 0x08:  /* CSI used as system clock  source */
    SystemCoreClock = CSI_VALUE;
    break;

  case 0x10:  /* HSE used as system clock  source */
    SystemCoreClock = HSE_VALUE;
    break;

  case 0x18:  /* PLL1 used as system clock  source */

    /* PLL_VCO = (HSE_VALUE or HSI_VALUE or CSI_VALUE/ PLLM) * PLLN
    SYSCLK = PLL_VCO / PLLR
    */
    pllsource = (RCC->PLLCKSELR & RCC_PLLCKSELR_PLLSRC);
    pllm = ((RCC->PLLCKSELR & RCC_PLLCKSELR_DIVM1)>> 4)  ;
    pllfracen = RCC->PLLCFGR & RCC_PLLCFGR_PLL1FRACEN;
    fracn1 = (pllfracen* ((RCC->PLL1FRACR & RCC_PLL1FRACR_FRACN1)>> 3));
    switch (pllsource)
    {

  case 0x00:  /* HSI used as PLL clock source */
      hsivalue = (HSI_VALUE >> ((RCC->CR & RCC_CR_HSIDIV)>> 3)) ;
      pllvco = (hsivalue/ pllm) * ((RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/0x2000) +1 );
      break;

    case 0x01:  /* CSI used as PLL clock source */
      pllvco = (CSI_VALUE / pllm) * ((RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/0x2000) +1 );
      break;

    case 0x02:  /* HSE used as PLL clock source */
      pllvco = (HSE_VALUE / pllm) * ((RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/0x2000) +1 );
      break;

    default:
      pllvco = (CSI_VALUE / pllm) * ((RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/0x2000) +1 );
      break;
    }
    pllp = (((RCC->PLL1DIVR & RCC_PLL1DIVR_P1) >>9) + 1 ) ;
    SystemCoreClock = (uint32_t) (pllvco/pllp);
    break;

  default:
    SystemCoreClock = CSI_VALUE;
    break;
  }
  
  /* Compute HCLK frequency --------------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = D1CorePrescTable[(RCC->CDCFGR1 & RCC_CDCFGR1_CDCPRE)>> POSITION_VAL(RCC_CDCFGR1_CDCPRE_0)];
  /* HCLK frequency */
  SystemCoreClock >>= tmp;
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
  register __IO uint32_t index;
	uint32_t *pBuffer;
	
#if defined (DATA_IN_ExtSDRAM)
  register uint32_t tmpreg = 0, timeout = 0xFFFF;
#endif	
	
#if defined (USE_FPGA)
	uint32_t uiAddress;
	RCC->D2AHB2ENR|=0xE0000000;
	
	#if defined (USE_SHARK)	
	uiAddress = 0x3003FFFC;
	#elif defined (USE_TINYSHARK)
	uiAddress = 0x3001FFFC;
	#endif
	
	#if defined (DATA_IN_ExtSRAM)
  *(volatile uint32_t*)uiAddress = 0x60000001;  // FMC enable
	__DSB();
  *(volatile uint32_t*)uiAddress = 0xFFFFFFFF;
  __DSB();
	#elif defined (DATA_IN_ExtSDRAM)
  *(volatile uint32_t*)uiAddress = 0x60000007;  //PH2 SDCKE_0 -->SDRAM (BANK1)
	__DSB();
  *(volatile uint32_t*)uiAddress = 0xFFFFFFFF;
  __DSB();  
	#endif	
#endif

#if defined (DATA_IN_ExtSDRAM)
  /* Enable GPIOD, GPIOE, GPIOF, GPIOG, GPIOH and GPIOI interface 
      clock */
  RCC->D3AHB1ENR |= 0x000001F8;
  
  /* Connect PDx pins to FMC Alternate function */
  GPIOD->AFR[0]  = 0x000000CC;
  GPIOD->AFR[1]  = 0xCC000CCC;
  /* Configure PDx pins in Alternate function mode */  
  GPIOD->MODER   = 0xA02A000A;
  /* Configure PDx pins speed to 50 MHz */  
  GPIOD->OSPEEDR = 0xA02A000A;
  /* Configure PDx pins Output type to push-pull */  
  GPIOD->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PDx pins */ 
  GPIOD->PUPDR   = 0x50150005;

  /* Connect PEx pins to FMC Alternate function */
  GPIOE->AFR[0]  = 0xC00000CC;
  GPIOE->AFR[1]  = 0xCCCCCCCC;
  /* Configure PEx pins in Alternate function mode */ 
  GPIOE->MODER   = 0xAAAA800A;
  /* Configure PEx pins speed to 50 MHz */ 
  GPIOE->OSPEEDR = 0xAAAA800A;
  /* Configure PEx pins Output type to push-pull */  
  GPIOE->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PEx pins */ 
  GPIOE->PUPDR   = 0x55554005;

  /* Connect PFx pins to FMC Alternate function */
  GPIOF->AFR[0]  = 0x00CCCCCC;
  GPIOF->AFR[1]  = 0xCCCCC000;
  /* Configure PFx pins in Alternate function mode */   
  GPIOF->MODER   = 0xAA800AAA;
  /* Configure PFx pins speed to 50 MHz */ 
  GPIOF->OSPEEDR = 0xAA800AAA;
  /* Configure PFx pins Output type to push-pull */  
  GPIOF->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PFx pins */ 
  GPIOF->PUPDR   = 0x55400555;

  /* Connect PGx pins to FMC Alternate function */
  GPIOG->AFR[0]  = 0x00CC00CC;
  GPIOG->AFR[1]  = 0xC000000C;
  /* Configure PGx pins in Alternate function mode */ 
  GPIOG->MODER   = 0x80020A0A;
  /* Configure PGx pins speed to 50 MHz */ 
  GPIOG->OSPEEDR = 0x80020A0A;
  /* Configure PGx pins Output type to push-pull */  
  GPIOG->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PGx pins */ 
  GPIOG->PUPDR   = 0x40010505;
  
  /* Connect PHx pins to FMC Alternate function */
  GPIOH->AFR[0]  = 0x00C0CC00;
  GPIOH->AFR[1]  = 0xCCCCCCCC;
  /* Configure PHx pins in Alternate function mode */ 
  GPIOH->MODER   = 0xAAAA08A0;
  /* Configure PHx pins speed to 50 MHz */ 
  GPIOH->OSPEEDR = 0xAAAA08A0;
  /* Configure PHx pins Output type to push-pull */  
  GPIOH->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PHx pins */ 
  GPIOH->PUPDR   = 0x55550450;
  
  /* Connect PIx pins to FMC Alternate function */
  GPIOI->AFR[0]  = 0xCCCCCCCC;
  GPIOI->AFR[1]  = 0x00000CC0;
  /* Configure PIx pins in Alternate function mode */ 
  GPIOI->MODER   = 0x0028AAAA;
  /* Configure PIx pins speed to 50 MHz */ 
  GPIOI->OSPEEDR = 0x0028AAAA;
  /* Configure PIx pins Output type to push-pull */  
  GPIOI->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PIx pins */ 
  GPIOI->PUPDR   = 0x00145555;

/*-- FMC Configuration ------------------------------------------------------*/
  /* Enable the FMC interface clock */
  (RCC->D1AHB1ENR |= (RCC_D1AHB1ENR_FMCEN));
	
  /*SDRAM Timing and access interface configuration*/
  /*LoadToActiveDelay  	 = 2
    ExitSelfRefreshDelay = 7
    SelfRefreshTime      = 4
    RowCycleDelay        = 7
    WriteRecoveryTime    = 2
    RPDelay              = 2
    RCDDelay             = 2
    SDBank             = FMC_SDRAM_BANK1
    ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_9 
    RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12
    MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_32
    InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4
    CASLatency         = FMC_SDRAM_CAS_LATENCY_3
    WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_ALLOW
    SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2
    ReadBurst          = FMC_SDRAM_RBURST_ENABLE
    ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0
	*/
  FMC_Bank5_6->SDCR[0] = 0x000019E5;
  FMC_Bank5_6->SDTR[0] = 0x01116361;

  /* SDRAM BANK1 initialization sequence */
  /* Step 3:  Configure a clock configuration enable command */
  FMC_Bank5_6->SDCMR = FMC_SDRAM_CMD_TARGET_BANK1 | FMC_SDRAM_CMD_CLK_ENABLE;
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  }

  /* Step 4: Insert delay */
  for (index = 0; index<1000; index++);
  
  /* Step 5: Configure a PALL (precharge all) command */ 
  FMC_Bank5_6->SDCMR = FMC_SDRAM_CMD_TARGET_BANK1 | FMC_SDRAM_CMD_PALL;
  timeout = 0xFFFF;
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  }
  
  /* Step 6 : Configure a Auto-Refresh command */ 
  FMC_Bank5_6->SDCMR = (7 << 5); /* 7 cycles */
  FMC_Bank5_6->SDCMR |= FMC_SDRAM_CMD_TARGET_BANK1 | FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  timeout = 0xFFFF;
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  }

  /* Step 7: Program the external memory mode register */
  FMC_Bank5_6->SDCMR = (0x230 << 9);
  FMC_Bank5_6->SDCMR |= FMC_SDRAM_CMD_TARGET_BANK1 | FMC_SDRAM_CMD_LOAD_MODE;
  timeout = 0xFFFF;
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  } 
	
  /* Step 8: Set the refresh rate counter */
  /* (7.81 us x Freq) - 20 */
  /* Set the device refresh counter */
  //tmpreg = FMC_Bank5_6->SDRTR;
  //FMC_Bank5_6->SDRTR = (tmpreg | (448));
  tmpreg = FMC_Bank5_6->SDRTR;
  FMC_Bank5_6->SDRTR = (tmpreg | (0x00000603<<1));

  /* Disable write protection SDRAM BANK1 */
  tmpreg = FMC_Bank5_6->SDCR[0]; 
  FMC_Bank5_6->SDCR[0] = (tmpreg & 0xFFFFFDFF);

	/*NOR/PSRAM bank and SDRAM bank 1/bank2 are swapped*/
	FMC_Bank1->BTCR[0]  |= 0x01000000;
	
	/*FMC controller Enable*/
  FMC_Bank1->BTCR[0]  |= 0x80000000;
	
	/* Due to bank switch, SDRAM is now mapped at 0x60000000 */
	pBuffer = (uint32_t *) 0x60000000;
	for (index=0; index<0x8000; index++)
	{
		pBuffer[index] = 0;
	}
	
#endif /* DATA_IN_ExtSDRAM */

#if defined(DATA_IN_ExtSRAM)
/*-- GPIOs Configuration -----------------------------------------------------*/
   /* Enable GPIOD, GPIOE, GPIOF and GPIOG interface clock */
  //RCC->AHB1ENR   |= 0x00000078;
  RCC->D3AHB1ENR |= 0x000000FF;
  	
  /* Connect PDx pins to FMC Alternate function */
  GPIOD->AFR[0]  = 0xC0CCC0CC;
  GPIOD->AFR[1]  = 0xCCCCCCCC;
  /* Configure PDx pins in Alternate function mode */  
  GPIOD->MODER   = 0xAAAA8A8A;
  /* Configure PDx pins speed to 100 MHz */  
  GPIOD->OSPEEDR = 0xFFFF0FCF;
  /* Configure PDx pins Output type to push-pull */  
  GPIOD->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PDx pins */ 
  GPIOD->PUPDR   = 0x00000000;

  /* Connect PEx pins to FMC Alternate function */
  GPIOE->AFR[0]  = 0xC00CC0CC;
  GPIOE->AFR[1]  = 0xCCCCCCCC;
  /* Configure PEx pins in Alternate function mode */ 
  GPIOE->MODER   = 0xAAAA828A;
  /* Configure PEx pins speed to 100 MHz */ 
  GPIOE->OSPEEDR = 0xFFFFC3CF;
  /* Configure PEx pins Output type to push-pull */  
  GPIOE->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PEx pins */ 
  GPIOE->PUPDR   = 0x00000000;

  /* Connect PFx pins to FMC Alternate function */
  GPIOF->AFR[0]  = 0x00CCCCCC;
  GPIOF->AFR[1]  = 0xCCCC0000;
  /* Configure PFx pins in Alternate function mode */   
  GPIOF->MODER   = 0xAA000AAA;
  /* Configure PFx pins speed to 100 MHz */ 
  GPIOF->OSPEEDR = 0xFF000FFF;
  /* Configure PFx pins Output type to push-pull */  
  GPIOF->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PFx pins */ 
  GPIOF->PUPDR   = 0x00000000;

  /* Connect PGx pins to FMC Alternate function */
  GPIOG->AFR[0]  = 0x00CCCCCC;
  GPIOG->AFR[1]  = 0x000000C0;
  /* Configure PGx pins in Alternate function mode */ 
  GPIOG->MODER   = 0x00080AAA;
  /* Configure PGx pins speed to 100 MHz */ 
  GPIOG->OSPEEDR = 0x000C0FFF;
  /* Configure PGx pins Output type to push-pull */  
  GPIOG->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PGx pins */ 
  GPIOG->PUPDR   = 0x00000000;
	
  
/*-- FMC/FSMC Configuration --------------------------------------------------*/                                                                               
  /* Enable the FMC/FSMC interface clock */
  //RCC->AHB3ENR         |= 0x00000001;
	RCC->D1AHB1ENR |= RCC_D1AHB1ENR_FMCEN;

  /* Configure and enable Bank1_SRAM1 */
  FMC_Bank1->BTCR[0]  = 0x00001091;
  FMC_Bank1->BTCR[1]  = 0x00110212;
  FMC_Bank1E->BWTR[0] = 0x0FFFFFFF;  
  /* Configure and enable Bank1_SRAM2 */
  //FMC_Bank1->BTCR[2]  = 0x00001091;
  //FMC_Bank1->BTCR[3]  = 0x00110212;
  //FMC_Bank1E->BWTR[2] = 0x0FFFFFFF;  
  /* Configure and enable Bank1_SRAM3 */
  //FMC_Bank1->BTCR[4]  = 0x00001091;
  //FMC_Bank1->BTCR[5]  = 0x00110212;
  //FMC_Bank1E->BWTR[4] = 0x0FFFFFFF;  
  /* Configure and enable Bank1_SRAM4 */
  //FMC_Bank1->BTCR[6]  = 0x00001091;
  //FMC_Bank1->BTCR[7]  = 0x00110212;
  //FMC_Bank1E->BWTR[6] = 0x0FFFFFFF;  

  /*FMC controller Enable*/
  FMC_Bank1->BTCR[0]  |= 0x80000000;

	pBuffer = (uint32_t *) 0x60000000;
	for (index=0; index<0x8000; index++)
	{
		pBuffer[index] = 0;
	}

#endif /* DATA_IN_ExtSRAM */
}
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
