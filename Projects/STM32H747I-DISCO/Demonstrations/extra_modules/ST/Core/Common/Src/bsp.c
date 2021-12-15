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
#include "stdio.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_BSP
  * @brief Kernel BSP routines
  * @{
  */

/* External variables --------------------------------------------------------*/
static UART_HandleTypeDef UART_Handle;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define BOARD_NAME                      "STM32H747I-DISCO"

/* STemWin Demonstration ID : 1 */
#define STFW_CODE_REVISION              0xF1010000 /* 0xAAXXYYZZ - 0xFx : FW Code ID | 0xXXYYZZ : MajorMinorPatch Version number */
#define STFW_DATA_REVISION              0xE1010000 /* 0xAAXXYYZZ - 0xEx : FW Data ID | 0xXXYYZZ : MajorMinorPatch Version number */
#define STFW_DESCRITION                 "STFirmware (Fractal) V1.0.0"

#define USARTx                          USART1


/* Private macros ------------------------------------------------------------*/
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private variables ---------------------------------------------------------*/
#if defined ( __ICCARM__ )
#pragma location="FwHeaderSection"
__root
#else
__attribute__((section(".FwHeaderSection"))) __attribute__ ((__used__))
#endif
static const Demo_Header_t FwHeader = {
  .demo_id = STFW_CODE_REVISION,
  .demo_descr = STFW_DESCRITION
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void BSP_ConfigSerial( void )
{
  UART_Handle.Instance            = USARTx;
  UART_Handle.Init.BaudRate       = 115200;
  UART_Handle.Init.WordLength     = UART_WORDLENGTH_8B;
  UART_Handle.Init.StopBits       = UART_STOPBITS_1;
  UART_Handle.Init.Parity         = UART_PARITY_NONE;
  UART_Handle.Init.Mode           = UART_MODE_TX_RX;
  UART_Handle.Init.HwFlowCtl      = UART_HWCONTROL_NONE;
  UART_Handle.Init.OverSampling   = UART_OVERSAMPLING_16;

  HAL_UART_Init( &UART_Handle );
}

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
  * @brief  Initializes the STM324x9I-EVAL's LCD and LEDs resources.
  * @param  None
  * @retval None
  */
uint8_t BSP_Config(void)
{
  /* Configure LEDs */
  BSP_LED_Init(LED_ORANGE);
  BSP_LED_Init(LED_BLUE);
  
  BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_EXTI);

  BSP_ConfigSerial();

  printf( "\n%s: %s (Build %s at %s)\n", BOARD_NAME, FwHeader.demo_descr, __DATE__, __TIME__ );
  printf( "CPU running at %luMHz, Peripherals at %luMHz/%luMHz\n", (HAL_RCC_GetHCLKFreq()/1000000U), (HAL_RCC_GetPCLK1Freq()/1000000U), (HAL_RCC_GetPCLK2Freq()/1000000U) );

  BSP_LED_Off(LED_ORANGE);
  BSP_LED_Off(LED_BLUE);
  return 1;
}

/**
  * @}
  */

/**
  * @}
  */

