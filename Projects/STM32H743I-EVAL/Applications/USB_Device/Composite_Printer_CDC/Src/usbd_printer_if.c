/**
  ******************************************************************************
  * @file    usbd_printer_if.c
  * @author  MCD Application Team
  * @brief   Source file for USBD printer interface
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                     www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static char file_name[100]={"Printer_file0.txt"};
static uint8_t Prnt_UserRxBuffer[PRNT_APP_RX_DATA_SIZE]; /* Received Data over USB are stored in this buffer */
uint8_t*  port_status;

/* USB handler declaration */
extern  USBD_HandleTypeDef  USBD_Device;

/* SD Card handler declaration */
SD_CardTypeDef hSD;

extern void Error_Handler(void);
/* Private function prototypes -----------------------------------------------*/
static int8_t PRNT_Itf_Init(void);
static int8_t PRNT_Itf_DeInit(void);
static int8_t PRNT_Itf_Control_req (uint8_t req, uint8_t* pbuf, uint16_t* length);
static int8_t PRNT_Itf_Receive(uint8_t* pbuf, uint32_t *Len);

/*printer Private function prototypes*/
void PRNT_PageEndManager(uint8_t* Buf,uint32_t Len);

static uint8_t PRNT_DEVICE_ID[] =
{
0x00,0x3F,
'M','F','G',':','S','T','M','i','c','r','o','e','l','e','c','t','r','o','n','i','c','s',';',                                 //   manufacturer
'M','D','L',':','U','S','B','P','r','i','n','t','e','r',';',                                                                 //   model
'C','M','D',':','P', 'D', 'L','P','C','P',';',                                                                               //   PCP command set
'C','L','S',':','P','R','I','N','T','E','R',';',                                                                             //   class
};

USBD_PRNT_ItfTypeDef USBD_PRNT_fops =
{
  PRNT_Itf_Init,
  PRNT_Itf_DeInit,
  PRNT_Itf_Control_req,
  PRNT_Itf_Receive
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  PRNT_Itf_Init
  *         Initializes the PRNT media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t PRNT_Itf_Init(void)
{
 if(FATFS_LinkDriver(&SD_Driver, hSD.SDPath) == 1U)
  {
    /* Unlink the USB disk I/O driver */
    FATFS_UnLinkDriver(hSD.SDPath);
  }
    /*Register the file system object to the FatFs module*/
    if(f_mount(&hSD.SDFatFs, (TCHAR const*)hSD.SDPath, 0) != FR_OK)
    {
      /* FatFs Initialization Error */
      Error_Handler();
    }
	  else
    {
      /*Create a text file object with write access */
      if(f_open(&hSD.MyFile,(const TCHAR*)file_name,FA_CREATE_ALWAYS|FA_WRITE) != FR_OK)
      {
        /* 'Printer_file.TXT' file Open for write Error */
        Error_Handler();
      }
      else
      {
        /*Initialize the PRNT peripheral */
        (void) USBD_PRNT_SetRxBuffer(&USBD_Device,(uint8_t*) Prnt_UserRxBuffer);
      }
    }


  UNUSED (PRNT_DEVICE_ID);

  /* Return OK */
  return ((int8_t)USBD_OK);
}

/**
  * @brief  PRNT_Itf_DeInit
  *         DeInitializes the PRNT media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t PRNT_Itf_DeInit(void)
{
  /* DeInitialize the PRNT peripheral */

  return ((int8_t)USBD_OK);
}

/**
  * @brief  PRNT_Itf_Control_req
  *         Manage the  PRNT class requests
  * @param  req: Command code
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t PRNT_Itf_Control_req   (uint8_t req, uint8_t* pbuf, uint16_t* length)
{
  uint32_t i = 0;

  /* Check on the setup request value */
  switch (req)
  {
    /* Get Printer Device ID request */
    case PRNT_GET_DEVICE_ID:
      /* Not using for loop here due to MISRA-C2012-Rule-16.1 */
      while (i < sizeof(PRNT_DEVICE_ID))
      {
        pbuf[i] = PRNT_DEVICE_ID[i];
        i++;
      }
      *length = i;
      break;

    /* Get Printer current status */
    case PRNT_GET_PORT_STATUS:
      *port_status =  PRNT_STATUS_PAPER_EMPTY | PRNT_STATUS_SELECTED | PRNT_STATUS_NO_ERROR;
      *pbuf = *port_status;
      *length = 1U;
      break;

    /* Printer SOFT RESET request: cleanup pending tasks */
    case PRNT_SOFT_RESET:
      (void)f_close(&hSD.MyFile);
      break;

    default:
      /* Unkown commands are not managed */
      break;
  }

  return ((int8_t)USBD_OK);
}

/**
  * @brief  PRNT_Itf_DataRx
  *         Data received over USB OUT endpoint are sent over PRNT interface
  *         through this function.
  * @param  Buf: Buffer of data to be transmitted
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t PRNT_Itf_Receive(uint8_t* Buf, uint32_t *Len)
{
  /* Update the Buffer where data shall be received */
  (void)USBD_PRNT_SetRxBuffer(&USBD_Device, (uint8_t*) Prnt_UserRxBuffer);

  /* Write data buffer to sd card */
  (void) PRNT_PageEndManager(Buf,*Len);

  /* Set the buffer to receive incoming data */
  (void) USBD_PRNT_ReceivePacket(&USBD_Device);

  /* Return OK status */
  return ((int8_t)USBD_OK);
}

/**
  * @brief PRNT_PageEndManager, defined by user
  *        Call this function frequently to check if data is received.
  * @param Buf: Buffer of data to be received
  * @param Len: Number of data received (in bytes)
  */
void PRNT_PageEndManager(uint8_t* Buf,uint32_t Len)
{
  static uint8_t file_count=0;

  /* Write the data buffer to SD Card */
  hSD.res = f_write(&hSD.MyFile, Prnt_UserRxBuffer, Len,(void *)&hSD.byteswritten);

  /* Check if write operation went well */
  if (hSD.res != FR_OK)
  {
    Error_Handler();
  }

  /* Detect end of page */
  if(Len < PRNT_DATA_OUT_PACKET_SIZE)
  {
    /* Close the text file*/
    (void)f_close(&hSD.MyFile);

    /* Tell user that the print operation has finished*/
    HAL_Delay(100);
    BSP_LED_Toggle(LED1);

    /* Move to next text file*/
    file_count++;
    (void) sprintf (file_name, "Printer_file%d.txt",file_count );

    /* Open file for next page */
    if(f_open(&hSD.MyFile,(const TCHAR*)file_name,(uint8_t)(FA_CREATE_ALWAYS|FA_WRITE)) != FR_OK)
    {
      Error_Handler();
    }
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
