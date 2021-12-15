/**
  ******************************************************************************
  * @file    FatFs/FatFs_Dual_Instance/CM4/Src/main.c
  * @author  MCD Application Team
  * @brief   the main CM4 application program
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
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS USBDISKFatFs;           /* File system object for USB disk logical drive */
FIL MyFile;                   /* File object */
char USBDISKPath[4];          /* USB Host logical drive path */
USBH_HandleTypeDef hUSB_Host; /* USB Host handle */
UART_HandleTypeDef UartHandle;

typedef enum {
  APPLICATION_IDLE = 0,
  APPLICATION_RUNNING,
}FS_FileOperationsTypeDef;

FS_FileOperationsTypeDef Appli_state = APPLICATION_IDLE;

/* Private function prototypes -----------------------------------------------*/
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);
static void FS_FileOperations(void);
static void Error_Handler(void);
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);

static uint8_t isFormatted = 0;
uint8_t workBuffer[_MAX_SS];

/* Private functions ---------------------------------------------------------*/

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

 /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */

   if (HAL_Init() != HAL_OK)
   {
     Error_Handler();
   }

   BSP_LED_Init(LED_ORANGE);
   BSP_LED_Init(LED_BLUE);
   
   /* Enable the USB voltage level detector */
   HAL_PWREx_EnableUSBVoltageDetector();

   /*##-1- Init Host Library ################################################*/
   USBH_Init(&hUSB_Host, USBH_UserProcess, 0);

   /*##-2- Add Supported Class ##############################################*/
   USBH_RegisterClass(&hUSB_Host, USBH_MSC_CLASS);

   /*##-3- Start Host Process ###############################################*/
   USBH_Start(&hUSB_Host);

   /*##-4- Run Application (Blocking mode) ##################################*/
   while (1)
   {
     /* USB Host Background task */
     USBH_Process(&hUSB_Host);

     /* Mass Storage Application State Machine */
     switch(Appli_state)
     {
     case APPLICATION_RUNNING:
       FS_FileOperations();
       Appli_state = APPLICATION_IDLE;
       break;

     case APPLICATION_IDLE:
     default:
       break;
     }
   }
}




static void Error_Handler(void)
{
  while(1)
  {
    BSP_LED_Toggle(LED_ORANGE);
    HAL_Delay(200);
  }
}

/**
  * @brief  Main routine for Mass Storage Class
  * @param  None
  * @retval None
  */
static void FS_FileOperations(void)
{
  FRESULT res;                                          /* FatFs function common result code */
  uint32_t byteswritten, bytesread;                     /* File write/read counts */
  uint8_t wtext[] = "CORE_CM4:This is STM32 working with FatFs + USB/HS"; /* File write buffer */
  uint8_t rtext[100];                                   /* File read buffer */

  if(isFormatted != 1)
  {
    if (f_mkfs(USBDISKPath, FM_ANY, 0, workBuffer, sizeof(workBuffer)) != FR_OK)
    {
      Error_Handler();
    }
    isFormatted = 1;
  }
  /* Create and Open a new text file object with write access */
  if(f_open(&MyFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
  {
    /* Write data to the text file */
    res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);

    if((byteswritten > 0) && (res == FR_OK))
    {
      /* Close the open text file */
      f_close(&MyFile);

      /* Open the text file object with read access */
      if(f_open(&MyFile, "STM32.TXT", FA_READ) == FR_OK)
      {
        /* Read data from the text file */
        res = f_read(&MyFile, rtext, sizeof(rtext), (void *)&bytesread);

        if((bytesread > 0) && (res == FR_OK))
        {
          /* Close the open text file */
          f_close(&MyFile);

          /* Compare read data with the expected data */
          if(Buffercmp(rtext, wtext, byteswritten) == 0)
          {
            /* Success of the demo: no error occurrence */
            BSP_LED_On(LED_BLUE);
            BSP_LED_Off(LED_ORANGE);

            return;
          }
        }
      }
    }
  }

  /* Error */
  Error_Handler();
}

/**
  * @brief  User Process
  * @param  phost: Host handle
  * @param  id: Host Library user message ID
  * @retval None
  */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
  switch (id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
    break;

  case HOST_USER_DISCONNECTION:
    Appli_state = APPLICATION_IDLE;
    if (f_mount(NULL, "", 0) != FR_OK)
    {
      Error_Handler();
    }

    if (FATFS_UnLinkDriver(USBDISKPath) != 0)
    {
      Error_Handler();
    }
    BSP_LED_Off(LED_BLUE);
    BSP_LED_On(LED_ORANGE);
    break;

  case HOST_USER_CLASS_ACTIVE:
    Appli_state = APPLICATION_RUNNING;
    break;

  case HOST_USER_CONNECTION:
    BSP_LED_Off(LED_ORANGE);
    if (FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == 0)
    {
      if (f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 0) != FR_OK)
      {
         Error_Handler();
      }
    }
    else
    {
      Error_Handler();
    }
    break;

  default:
    break;
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


