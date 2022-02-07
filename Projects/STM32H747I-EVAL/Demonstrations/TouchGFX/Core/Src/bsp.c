/**
  ******************************************************************************
  * @file    bsp.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel bsp functions
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
#include "bsp.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_BSP
  * @brief Kernel BSP routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct Bitmap_Res_s
{
  uint16_t       x;
  uint16_t       y;
} Bitmap_Res_t;

typedef struct Demo_Header_s
{
  uint32_t              demo_id;
  const uint32_t * const icon_addr;
  Bitmap_Res_t          icon_res;
  const uint32_t * const logo_addr;
  Bitmap_Res_t          logo_res;
  uint32_t              sw_flags;
  uint32_t              reserved[2];
  unsigned char         demo_descr[32];
} Demo_Header_t;

/* Private defines -----------------------------------------------------------*/
#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_14
#define USARTx_TX_GPIO_PORT              GPIOB
#define USARTx_TX_AF                     GPIO_AF4_USART1
#define USARTx_RX_PIN                    GPIO_PIN_15
#define USARTx_RX_GPIO_PORT              GPIOB
#define USARTx_RX_AF                     GPIO_AF4_USART1

#define BOARD_NAME                      "STM32H747I-EVAL_MB1246"
/* TouchGFX Demonstration ID : 1 */
#define DEMO_CODE_REVISION              0xC2010000 /* 0xAAXXYYZZ - 0xCx : Demo Code ID | 0xXXYYZZ : MajorMinorPatch Version number */
#define DEMO_DATA_REVISION              0xD2010000 /* 0xAAXXYYZZ - 0xDx : Demo Data ID | 0xXXYYZZ : MajorMinorPatch Version number */
#define DEMO_DESCRITION                 "TouchGFX Out of Box Demo"

#define SW_FLAG_AUTO_DEMO               (1L << 0)

/* Private macros ------------------------------------------------------------*/
#ifdef __cplusplus
 extern "C" {
#endif

#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#ifdef __cplusplus
}
#endif

#define GET_DEMO_VERSION_STR(tag)  "V%lu.%lu.%lu"
#define GET_DEMO_VERSION_P(tag)    ((tag  &0x00FF0000) >> 16), ((tag  &0x0000FF00) >> 8), (tag  &0x000000FF)

/* Private variables ---------------------------------------------------------*/
extern const uint32_t demo_icon __attribute__ ((section ("demo_icon_section")));
extern const uint32_t demo_logo __attribute__ ((section ("demo_logo_section")));

#if defined ( __ICCARM__ )
#pragma location="IntThirdPartyHeaderSection"
__root
#else
__attribute__((section(".IntThirdPartyHeaderSection"))) __attribute__ ((__used__))
#endif
static const Demo_Header_t IntThirdPartyHeader = {
  .demo_id = DEMO_CODE_REVISION,
  .icon_addr  = 0,
  .icon_res   = { 0, 0 },
  .logo_addr  = 0,
  .logo_res   = { 0, 0 },
  .sw_flags   = SW_FLAG_AUTO_DEMO,
  .demo_descr = DEMO_DESCRITION
};

#if defined ( __ICCARM__ )
#pragma location="ExtThirdPartyHeaderSection"
__root
#else
__attribute__((section(".ExtThirdPartyHeaderSection"))) __attribute__ ((__used__))
#endif
static const Demo_Header_t ExtThirdPartyHeader = {
  .demo_id = DEMO_DATA_REVISION,
  .icon_addr  = &demo_icon,
  .icon_res   = { 180, 180 },
  .logo_addr  = &demo_logo,
  .logo_res   = { 60, 60 },
  .sw_flags   = SW_FLAG_AUTO_DEMO,
  .demo_descr = DEMO_DESCRITION
};

uint8_t detected = 0;
uint8_t Touchdetected = 0;
static UART_HandleTypeDef UART_Handle;
static uint32_t AutoDemoId = 0;
static uint32_t AutoDemoTimeOutMs = AUTO_DEMO_TIMEOUT_0;

/* Private function prototypes -----------------------------------------------*/
static void BSP_ConfigSerial(void);

/* Private functions ---------------------------------------------------------*/
static void BSP_ConfigSerial(void)
{
  UART_Handle.Instance            = USARTx;
  UART_Handle.Init.BaudRate       = 115200;
  UART_Handle.Init.WordLength     = UART_WORDLENGTH_8B;
  UART_Handle.Init.StopBits       = UART_STOPBITS_1;
  UART_Handle.Init.Parity         = UART_PARITY_NONE;
  UART_Handle.Init.Mode           = UART_MODE_TX_RX;
  UART_Handle.Init.HwFlowCtl      = UART_HWCONTROL_NONE;
  UART_Handle.Init.OverSampling   = UART_OVERSAMPLING_16;

  if(HAL_UART_Init(&UART_Handle) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UART_Handle, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

/**
  * @brief  Initializes LEDs, SDRAM, touch screen, CRC and SRAM.
  * @param  None
  * @retval RetVal
  */
uint8_t BspInit(void)
{
  static __IO uint32_t BckRegValue = 0;
  BSP_ConfigSerial();

  printf( "\n\n%s: %s " GET_DEMO_VERSION_STR(IntThirdPartyHeader.demo_id) " (Build %s at %s)\n", BOARD_NAME, IntThirdPartyHeader.demo_descr, GET_DEMO_VERSION_P(IntThirdPartyHeader.demo_id), __DATE__, __TIME__ );
  printf( "CPU running at %luMHz, Peripherals at %luMHz/%luMHz\n", (HAL_RCCEx_GetD1SysClockFreq()/1000000U), (HAL_RCC_GetPCLK1Freq()/1000000U), (HAL_RCC_GetPCLK2Freq()/1000000U) );

  HAL_PWR_EnableBkUpAccess();

  /* Read SW Configuration */
  BckRegValue       = READ_REG(BKP_REG_SW_CFG);
  AutoDemoId        = (((BckRegValue & 0x0000FF00) >>  8) & 0xFF);
  AutoDemoTimeOutMs = (((BckRegValue & 0xFFFF0000) >> 16) & 0xFFFF);
  if(AutoDemoTimeOutMs == 0)
  {
    AutoDemoTimeOutMs = AUTO_DEMO_TIMEOUT_0;
  }

  HAL_PWR_DisableBkUpAccess();
  
  /* Check if we're in AutoDemo Mode */
  if ( AutoDemoId )
  {
    /* Make Auto Demo Timer shorter */
  }

  return 1;
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();

  /* Select SysClk as source of USART1 clocks */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART16;
  RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

  /* Enable USART1 clock */
  USARTx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);

}

/**
  * @}
  */

/**
  * @}
  */

