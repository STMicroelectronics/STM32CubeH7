/**
  ******************************************************************************
  * @file    Examples_LL/DMA/DMA_CopyFromFlashToMemory/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use a DMA channel to transfer
  *          a word data buffer from FLASH memory to embedded SRAM memory
  *          through the STM32H7xx DMA LL API.
  *          Peripheral initialization done using LL unitary services functions.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_LL_Examples
  * @{
  */

/** @addtogroup DMA_CopyFromFlashToMemory
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE              32

/* Private macro -------------------------------------------------------------*/

/* Macro to get variable aligned on 32-bytes,needed for cache maintenance purpose */

#if defined   (__GNUC__)        /* GNU Compiler */
#define ALIGN_32BYTES(buf)  buf __attribute__ ((aligned (32)))
#elif defined (__ICCARM__)    /* IAR Compiler */
#define ALIGN_32BYTES(buf) _Pragma("data_alignment=32") buf
#elif defined   (__CC_ARM)      /* ARM Compiler */
#define ALIGN_32BYTES(buf) __align(32) buf
#endif

/* Private variables ---------------------------------------------------------*/
static const uint32_t aSRC_Const_Buffer[BUFFER_SIZE] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80
};

ALIGN_32BYTES(static uint32_t aDST_Buffer[BUFFER_SIZE]);
__IO uint32_t TransferErrorDetected = 0;
__IO uint32_t TransferCompleteDetected = 0;

/* Private function prototypes -----------------------------------------------*/
static void     SystemClock_Config(void);
void     Configure_DMA(void);
uint8_t  Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength);
void     LED_Init(void);
void     LED_On(void);
void     LED_Blinking(uint32_t Period);
static void CPU_CACHE_Enable(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* Configure the system clock to 280 MHz */
  SystemClock_Config();

  /* Initialize LED1 */
  LED_Init();

  /* Configure DMA2 Stream 0                                     */
  /* Enable DMA2 clock                                           */
  /* Configure the DMA functional parameters :                  */
  /*        - Transfer memory word to memory word in normal mode */
  /*        - Memory and Periph increment mode                   */
  /* Configure NVIC for DMA transfer complete/error interrupts   */
  Configure_DMA();

  /* Start the DMA transfer Flash to Memory */
  LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);

  /* Wait for the end of the transfer or error in transfer        */
  while((TransferCompleteDetected == 0) && (TransferErrorDetected == 0))
  {
  }

  if(TransferErrorDetected != 0)
  {
    /* Error detected during DMA transfer */
    LED_Blinking(LED_BLINK_ERROR);
  }
  else
  {
    /*
      CPU Data Cache maintenance :
      It is recommended to invalidate the CPU Data cache after the DMA transfer.
      As the destination buffer may be used by the CPU, this guarantees Up-to-date data when CPU accesses
      to the destination buffer located in the AXI-SRAM (which is cacheable).
     */
     SCB_InvalidateDCache_by_Addr((uint32_t *) aDST_Buffer, sizeof(aDST_Buffer) );

    /* DMA transfer completed */
    /* Verify the data transferred */
    if (Buffercmp((uint32_t*)aSRC_Const_Buffer, (uint32_t*)aDST_Buffer, BUFFER_SIZE) != 0)
    {
      /* DMA data transferred not correctly */
      LED_Blinking(LED_BLINK_ERROR);
    }
    else
    {
      /* DMA data transferred correctly*/
      LED_On();
    }
  }

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function configures the DMA2 Stream 0 to copy data from
  *         Flash memory(aSRC_Const_Buffer) to Internal SRAM(aDST_Buffer).
  * @note   This function is used to :
  *         -1- Enable DMA2 clock
  *         -2- Configure the DMA functional parameters
  *         -3- Configure NVIC for DMA transfer complete/error interrupts
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param   None
  * @retval  None
  */
void Configure_DMA(void)
{
  /* (1) Enable the clock of DMA2 */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);

  /* (2) Configure the DMA functional parameters */
  /* Configuration of the DMA parameters can be done using unitary functions or using the specific configure function */ 
  /* Unitary Functions */

  LL_DMA_SetDataTransferDirection(DMA2, LL_DMA_STREAM_0, LL_DMA_DIRECTION_MEMORY_TO_MEMORY);
  LL_DMA_SetStreamPriorityLevel(DMA2, LL_DMA_STREAM_0, LL_DMA_PRIORITY_HIGH);
  LL_DMA_SetMode(DMA2, LL_DMA_STREAM_0, LL_DMA_MODE_NORMAL);
  LL_DMA_SetPeriphIncMode(DMA2, LL_DMA_STREAM_0, LL_DMA_PERIPH_INCREMENT);
  LL_DMA_SetMemoryIncMode(DMA2, LL_DMA_STREAM_0, LL_DMA_MEMORY_INCREMENT);
  LL_DMA_SetPeriphSize(DMA2, LL_DMA_STREAM_0, LL_DMA_PDATAALIGN_WORD);
  LL_DMA_SetMemorySize(DMA2, LL_DMA_STREAM_0, LL_DMA_MDATAALIGN_WORD);
  
  /* Configure Function */
//  LL_DMA_ConfigTransfer(DMA2, LL_DMA_STREAM_0, LL_DMA_DIRECTION_MEMORY_TO_MEMORY |
//                                               LL_DMA_PRIORITY_HIGH              |
//                                               LL_DMA_MODE_NORMAL                |
//                                               LL_DMA_PERIPH_INCREMENT           |
//                                               LL_DMA_MEMORY_INCREMENT           |
//                                               LL_DMA_PDATAALIGN_WORD            |
//                                               LL_DMA_MDATAALIGN_WORD);
    
  LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_0, BUFFER_SIZE);
  LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_0, (uint32_t)&aSRC_Const_Buffer, (uint32_t)&aDST_Buffer, LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_STREAM_0));
  
  /* (3) Configure NVIC for DMA transfer complete/error interrupts */
  LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_0);
  LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_0);
  NVIC_SetPriority(DMA2_Stream0_IRQn, 0);
  NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0: pBuffer identical to pBuffer1
  *         1: pBuffer differs from pBuffer1
  */
uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  Initialize LED1.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED1 Clock */
  LED1_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED1 */
  LL_GPIO_SetPinMode(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Turn-on LED1.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED1 on */
  LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

/**
  * @brief  Set LED1 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Turn LED1 on */
  LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);
  
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);  
    LL_mDelay(Period);
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL1 (HSE)
  *            SYSCLK(Hz)                     = 280000000 (CPU Clock)
  *            HCLK(Hz)                       = 280000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 1
  *            CD APB3 Prescaler              = 2 (APB3 Clock  140MHz)
  *            CD APB1 Prescaler              = 2 (APB1 Clock  140MHz)
  *            CD APB2 Prescaler              = 2 (APB2 Clock  140MHz)
  *            SRD APB4 Prescaler             = 2 (APB4 Clock  140MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 280
  *            PLL_P                          = 2
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 6
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE0);
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }

  /* Enable HSE oscillator */
  LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_SelectDigitalClock();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  }

  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_6);

  /* Main PLL configuration and activation */
  LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);
  LL_RCC_PLL1P_Enable();
  LL_RCC_PLL1Q_Enable();
  LL_RCC_PLL1R_Enable();
  LL_RCC_PLL1FRACN_Disable();
  LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_2_4);
  LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
  LL_RCC_PLL1_SetM(4);
  LL_RCC_PLL1_SetN(280);
  LL_RCC_PLL1_SetP(2);
  LL_RCC_PLL1_SetQ(2);
  LL_RCC_PLL1_SetR(2);
  LL_RCC_PLL1_Enable();

  while(LL_RCC_PLL1_IsReady() != 1)
  {
  }

  /* Set Sys & AHB & APB1 & APB2 & APB4  prescaler */
  LL_RCC_SetSysPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetAPB4Prescaler(LL_RCC_APB4_DIV_2);

  /* Set PLL1 as System Clock Source */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
  {
  }

  /* Set systick to 1ms */
  SysTick_Config(280000000 / 1000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  SystemCoreClock = 280000000;
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  DMA transfer complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
void TransferComplete()
{
  TransferCompleteDetected = 1;
}

/**
  * @brief  DMA transfer error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during DMA transfer
  * @retval None
  */
void TransferError()
{
  /* Error detected during DMA transfer */
  TransferErrorDetected = 1;
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

