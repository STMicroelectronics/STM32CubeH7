/**
  ******************************************************************************
  * @file    FatFs/FatFs_Shared_Device/CM4/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body for Cortex-M4.
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
#include <stdio.h>

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#define UART_HSEM_ID (2U)

#define CM4_FILE  "cm4.log"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS MMCFatFs;  /* File system object for SD card logical drive */
FIL CM4_File;     /* File object */
char MMCPath[4]; /* SD card logical drive path */

uint8_t workBuffer[_MAX_SS];
ALIGN_32BYTES(uint8_t rtext[96]);
UART_HandleTypeDef     UartHandle;

uint8_t wtext[] = "This is FatFs running on CM4 core"; /* File write buffer */

/* Private function prototypes -----------------------------------------------*/
static void Error_Handler(void);
static void FS_FileOperations(void);
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

static void UART_Config(void);
/* Private functions ---------------------------------------------------------*/

PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFFF);
  return ch;
}

static void UART_Config(void)
{

  UartHandle.Instance        = USARTx;
  UartHandle.Init.BaudRate   = 115200;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_ODD;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

  HAL_UART_DeInit(&UartHandle);
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();

  /* Activate HSEM notification for Cortex-M4*/
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

  /* 
    Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
    perform system initialization (system clock config, external memory configuration.. )   
  */
  HAL_PWREx_ClearPendingEvent();
  HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);

  /* Clear HSEM flag */
  __HAL_HSEM_CLEAR_FLAG(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

  HAL_Init();
  UART_Config();

  /*-1- Link the micro SD disk I/O driver */
  if(FATFS_LinkDriver(&MMC_Driver, MMCPath) == 0)
  {
    LOCK_HSEM(UART_HSEM_ID);
    printf("[CM4]:\tstarting FatFs operation....\n");
    UNLOCK_HSEM(UART_HSEM_ID);
    /* -2- Start the FatFs operation cuncurrently with CM7 */
    FS_FileOperations();
  }
}

static void FS_FileOperations(void)
{
  FRESULT res;
  uint32_t byteswritten, bytesread;           /* File write/read counts */

  /* Register the file system object to the FatFs module */
  res = f_mount(&MMCFatFs, (TCHAR const*)MMCPath, 0);
  if( res != FR_OK)
  {
    goto error;
  }

  LOCK_HSEM(UART_HSEM_ID);
  printf("[CM4]:\topening '%s' for writing...\n", CM4_FILE);
  UNLOCK_HSEM(UART_HSEM_ID);

  /* Create and Open a new text file object with write access */
  res = f_open(&CM4_File, CM4_FILE, FA_CREATE_ALWAYS | FA_WRITE);
  if(res != FR_OK)
  {
    goto error;
  }

  res = f_write(&CM4_File, wtext, sizeof(wtext), (void *)&byteswritten);

  if((res != FR_OK) || (byteswritten == 0))
  {
    f_close(&CM4_File);
    goto error;
  }

  LOCK_HSEM(UART_HSEM_ID);
  printf("[CM4]:\t%s '%lu' bytes written...\n", CM4_FILE, byteswritten);
  UNLOCK_HSEM(UART_HSEM_ID);

  /* Close the open text file */
  f_close(&CM4_File);

  /* Open the text file object with read access */
  if(f_open(&CM4_File, CM4_FILE, FA_READ) != FR_OK)
  {
    goto error;
  }

  LOCK_HSEM(UART_HSEM_ID);
  printf("[CM4]:\treading '%s' content...\n", CM4_FILE);
  UNLOCK_HSEM(UART_HSEM_ID);

  /* Read data from the text file */
  res = f_read(&CM4_File, rtext, sizeof(rtext), (void *)&bytesread);

  if((bytesread == 0) || (res != FR_OK))
  {
    f_close(&CM4_File);
    goto error;
  }

  /* Close the open text file */
  f_close(&CM4_File);

  /* Compare read data with the expected data */
  if(Buffercmp(rtext, (uint8_t *)wtext,  byteswritten) == 0)
  {

    LOCK_HSEM(UART_HSEM_ID);
    printf("[CM4]:\t%s => '>  %s <'\n", CM4_FILE, rtext);
    UNLOCK_HSEM(UART_HSEM_ID);

    while(1)
    {
      BSP_LED_Toggle(LED_GREEN);
      HAL_Delay(250);
    }
  }
  else
  {
    goto error;
  }

error:
  Error_Handler();
}

static void Error_Handler(void)
{
  LOCK_HSEM(UART_HSEM_ID);
  printf ("[CM4]:\tError!! \n");
  UNLOCK_HSEM(UART_HSEM_ID);

  while(1)
  {
  }
}

static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
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




#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

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

