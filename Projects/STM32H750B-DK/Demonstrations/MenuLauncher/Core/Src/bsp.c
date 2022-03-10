/**
  ******************************************************************************
  * @file    bsp.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel bsp functions
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
extern TIM_HandleTypeDef TimHandle;
extern DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef hltdc;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define DEMO_DESCRITION                 "Out Of the Box Demonstration V1.0.0"
#define STML_DESCRITION                 "ST Menu Launcher V1.0.0"
#define BOARD_NAME                      "STM32H750B-DK_MB1381"

#define HSEM_ID_0                       (0U) /* HW semaphore 0*/

#define MAX_FLASH_WRITE_FAILURE         10

#define USARTx                          USART3

#ifndef EXT_FLASH_SIZE
#define EXT_FLASH_SIZE                  MT25TL01G_FLASH_SIZE
#endif /* EXT_FLASH_SIZE */
#ifndef EXT_FLASH_SECTOR_SIZE
#define EXT_FLASH_SECTOR_SIZE           MT25TL01G_SECTOR_SIZE
#endif /* EXT_FLASH_SECTOR_SIZE */
#ifndef EXT_FLASH_SUBSECTOR_SIZE
#define EXT_FLASH_SUBSECTOR_SIZE        (2 * MT25TL01G_SUBSECTOR_SIZE)
#endif /* EXT_FLASH_SUBSECTOR_SIZE */
#ifndef EXT_FLASH_PAGE_SIZE
#define EXT_FLASH_PAGE_SIZE             MT25TL01G_PAGE_SIZE
#endif /* EXT_FLASH_PAGE_SIZE */

#define FLASH_BURST_WIDTH               256 /* in bits */

/* Private macros ------------------------------------------------------------*/
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private variables ---------------------------------------------------------*/
static UART_HandleTypeDef UART_Handle;

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
uint8_t BSP_Config(const uint8_t minimal)
{
  uint8_t RetVal = 0;
  uint8_t counter = 10;

  BSP_QSPI_Init_t qspi_initParams ;
  TS_Init_t hTS;

  qspi_initParams.InterfaceMode=MT25TL01G_QPI_MODE;
  qspi_initParams.TransferRate= MT25TL01G_DTR_TRANSFER ;
  qspi_initParams.DualFlashMode= MT25TL01G_DUALFLASH_ENABLE;
  
  /* Minimal boot for Apps */
  if(minimal)
  {
    /* LED_RED is needed in case of error */
    BSP_LED_Init(LED_RED);

    /* Initialize the NOR QuadSPI flash */
    if (BSP_QSPI_Init(0, &qspi_initParams) == BSP_ERROR_NONE)
    {
      if(BSP_QSPI_EnableMemoryMappedMode(0) == BSP_ERROR_NONE)
      {
        BSP_LED_DeInit(LED_RED);
        return 1;
      }
    }

    return 0;
  }

  /* Configure LEDs */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  /* Init User push-button in EXTI Mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  BSP_ConfigSerial();

  /* Print OoB Demo version */
  printf( "\n\n%s: %s (Build %s at %s)\n\n", BOARD_NAME, DEMO_DESCRITION, __DATE__, __TIME__ );

  /* Print ML version */
  printf( "%s: %s\n", BOARD_NAME, STML_DESCRITION );

  /* Print Clock configuration */
  printf( "CPU running at %luMHz, Peripherals at %luMHz/%luMHz\n", (HAL_RCCEx_GetD1SysClockFreq()/1000000U), (HAL_RCC_GetPCLK1Freq()/1000000U), (HAL_RCC_GetPCLK2Freq()/1000000U) );

  /* Initialize the SDRAM memory */
  if ((RetVal = BSP_SDRAM_Init(0)) != BSP_ERROR_NONE)
  {
    printf("Failed to initialize the SDRAM !! (Error %d)\n", RetVal);
    return 0;
  }

  /* Initialize the Touch screen */
  hTS.Width = 480;
  hTS.Height = 272;
  hTS.Orientation =TS_SWAP_XY ;
  hTS.Accuracy = 5;

  do
  {
    RetVal = BSP_TS_Init(0, &hTS);
    HAL_Delay(100);
    counter--;
  }
  while (counter && (RetVal != BSP_ERROR_NONE));

  if(RetVal != BSP_ERROR_NONE)
  {
    printf("Failed to initialize TS !! (Error %d)\n", RetVal);
    return 0;
  }

  RetVal = BSP_TS_EnableIT(0);
  if(RetVal != BSP_ERROR_NONE)
  {
    printf("Failed to initialize TS (IT) !! (Error %d)\n", RetVal);
    return 0;
  }

  /* Initialize the NOR QuadSPI flash */
  if ((RetVal = BSP_QSPI_Init(0, &qspi_initParams)) != BSP_ERROR_NONE)
  {
    printf("Failed to initialize the QSPI !! (Error %d)\n", RetVal);
    return 0;
  }
  else
  {
    if((RetVal = BSP_QSPI_EnableMemoryMappedMode(0)) != BSP_ERROR_NONE)
    {
      printf("Failed to configure the QSPI !! (Error %d)\n", RetVal);
      return 0;
    }
  }

  /* Enable Back up SRAM */
  /* Enable write access to Backup domain */
  PWR->CR1 |= PWR_CR1_DBP;
  while((PWR->CR1 & PWR_CR1_DBP) == RESET)
  {
  }
  /*Enable BKPRAM clock*/
  __HAL_RCC_BKPRAM_CLK_ENABLE();

  BSP_LED_On(LED_GREEN);

  return 1;
}

/**
  * @brief  Provide the GUI with current state of the touch screen
  * @param  None
  * @retval None
  */
uint8_t BSP_TouchUpdate(void)
{
  static GUI_PID_STATE TS_State = {0, 0, 0, 0};
  __IO TS_State_t  ts;
  uint16_t xDiff, yDiff;

  BSP_TS_GetState(0, (TS_State_t *)&ts);

  if((ts.TouchX >= LCD_GetXSize()) ||(ts.TouchY >= LCD_GetYSize()) )
  {
    ts.TouchX = 0;
    ts.TouchY = 0;
  }

  xDiff = (TS_State.x > ts.TouchX) ? (TS_State.x - ts.TouchX) : (ts.TouchX - TS_State.x);
  yDiff = (TS_State.y > ts.TouchY) ? (TS_State.y - ts.TouchY) : (ts.TouchY - TS_State.y);

  if((TS_State.Pressed != ts.TouchDetected ) ||
     (xDiff > 8 )||
       (yDiff > 8))
  {
    TS_State.Pressed = ts.TouchDetected;
    TS_State.Layer = 0;
    if(ts.TouchDetected)
    {
      TS_State.x = ts.TouchX;
      TS_State.y = ts.TouchY ;
      GUI_TOUCH_StoreStateEx(&TS_State);
    }
    else
    {
      GUI_TOUCH_StoreStateEx(&TS_State);
      TS_State.x = 0;
      TS_State.y = 0;
    }

    return 1;
  }

  return 0;
}

/**
  * @brief  Jump to Sub demo application
  * @param  Address  : Address where the sub demonstration code is located
  * @retval None
  */
void BSP_JumpToSubDemo(uint32_t SubDemoAddress)
{
  /* Store the address of the Sub Demo binary */
  HAL_PWR_EnableBkUpAccess();
  WRITE_REG(BKP_REG_SUBDEMO_ADDRESS, (uint32_t) SubDemoAddress);
  HAL_PWR_DisableBkUpAccess();

  printf("\nJumping to Sub-Demo @0x%08lu ...\n", SubDemoAddress);

  GUI_Delay(200);

  HAL_DMA2D_DeInit(&hdma2d);
  hltdc.Instance = LTDC;
  HAL_LTDC_DeInit(&hltdc);

  HAL_Delay(100);
  HAL_UART_DeInit( &UART_Handle );

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Disable and Invalidate I-Cache */
  SCB_DisableICache();
  SCB_InvalidateICache();

  /* Disable, Clean and Invalidate D-Cache */
  SCB_DisableDCache();
  SCB_CleanInvalidateDCache();

  /* Disable the TIM6 global Interrupt */
  HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
  HAL_TIM_Base_DeInit(&TimHandle);

  /* Do Software Reset */
  HAL_NVIC_SystemReset();
}

/**
  * @}
  */

/**
  * @}
  */

