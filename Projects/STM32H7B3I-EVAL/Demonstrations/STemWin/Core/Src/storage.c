/**
  ******************************************************************************
  * @file    storage.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel storage functions
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
#include "storage.h"
#include <string.h>

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_STORAGE
  * @brief Kernel storage routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define STORAGE_BSP_INIT
//#define USE_SDCARD_NO_DMA
#define STORAGE_BSP_SD_ID               0

#define STORAGE_THREAD_STACK_SIZE       (8 * configMINIMAL_STACK_SIZE)
#define STORAGE_THREAD_PRIORITY         osPriorityNormal

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="sdio_heap"
#else
__attribute__((section(".sdio_heap"))) __attribute__ ((aligned (32)))
#endif
static FATFS              StorageDISK_FatFs[NUM_DISK_UNITS];            /* File system object for MSD disk logical drive */
static char               StorageDISK_Drive[NUM_DISK_UNITS+1][4];       /* Storage Host logical drive number */
static osSemaphoreId      StorageSemaphore[NUM_DISK_UNITS];
#if defined(USE_SDCARD) || defined(USE_USB_FS) || defined(USE_USB_HS)
static Diskio_drvTypeDef const * Storage_Driver[NUM_DISK_UNITS];
#endif /* USE_SDCARD | USE_USB_FS | USE_USB_HS */
static  uint8_t           StorageID[NUM_DISK_UNITS];
static STORAGE_Status_t   StorageStatus[NUM_DISK_UNITS];

osMessageQId              StorageEvent    = {0};
osThreadId                StorageThreadId = {0};

#if defined(USE_USB_FS) || defined(USE_USB_HS)
USBH_HandleTypeDef        hUSBHost;
#endif /* USE_USB_FS | USE_USB_HS */

/* Private function prototypes -----------------------------------------------*/
#if defined(USE_SDCARD) || defined(USE_USB_FS) || defined(USE_USB_HS)
static STORAGE_Status_t StorageTryMount( const uint8_t unit );
#endif /* USE_SDCARD | USE_USB_FS | USE_USB_HS */
static STORAGE_Status_t StorageTryUnMount( const uint8_t unit );
static void StorageThread(void const * argument);
#if defined(USE_SDCARD)
static uint8_t StorageInitMSD(void);
#endif /* USE_SDCARD */
#if defined(USE_USB_FS) || defined(USE_USB_HS)
static uint8_t StorageInitUSB(void);
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id);
#endif /* USE_USB_FS | USE_USB_HS */

/* Private functions ---------------------------------------------------------*/
#if defined(USE_SDCARD) || defined(USE_USB_FS) || defined(USE_USB_HS)
static STORAGE_Status_t StorageTryMount( const uint8_t unit )
{
  osSemaphoreWait(StorageSemaphore[unit], osWaitForever);

#if defined(USE_SDCARD)
  if(unit == MSD_DISK_UNIT)
  {
    /* We need to check for SD Card before mounting the volume */
    if(!BSP_SD_IsDetected(STORAGE_BSP_SD_ID))
    {
      StorageStatus[unit] = STORAGE_UNMOUNTED;
      goto unlock_exit;
    }
  }
#endif /* USE_SDCARD */

  if(StorageStatus[unit] != STORAGE_MOUNTED)
  {
    /* Link the disk I/O driver */
    if(FATFS_LinkDriver(Storage_Driver[unit], StorageDISK_Drive[unit]) != 0)
      goto unlock_exit;

    if(f_mount(&StorageDISK_FatFs[unit], StorageDISK_Drive[unit], 0))
      goto unlock_exit;

    /* Set SD storage status */
    // if(unit == MSD_DISK_UNIT)
      StorageStatus[unit] = STORAGE_MOUNTED;
  }

unlock_exit:
  osSemaphoreRelease(StorageSemaphore[unit]);
  return StorageStatus[unit];
}
#endif /* USE_SDCARD | USE_USB_FS | USE_USB_HS */

static STORAGE_Status_t StorageTryUnMount( const uint8_t unit )
{
  if(StorageID[unit] == 0)
    return StorageStatus[unit];

  osSemaphoreWait(StorageSemaphore[unit], osWaitForever);

  if(StorageStatus[unit] != STORAGE_MOUNTED)
    goto unlock_exit;

  if(f_mount(0, StorageDISK_Drive[unit], 0))
    goto unlock_exit;

  if(FATFS_UnLinkDriver(StorageDISK_Drive[unit]))
    goto unlock_exit;

  memset(StorageDISK_Drive[unit], 0, sizeof(StorageDISK_Drive[unit]));
  /* Reset storage status */
  StorageStatus[unit] = STORAGE_UNMOUNTED;

unlock_exit:
  osSemaphoreRelease(StorageSemaphore[unit]);
  return StorageStatus[unit];
}

#if defined(USE_USB_FS) || defined(USE_USB_HS)
/**
  * @brief  User Process
  * @param  phost: Host handle
  * @param  id:    Host Library user message ID
  * @retval None
  */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
  if(!StorageEvent)
    return;

  switch (id)
  {
    case HOST_USER_SELECT_CONFIGURATION:
      break;

    case HOST_USER_DISCONNECTION:
      osMessagePut ( StorageEvent, USBDISK_DISCONNECTION_EVENT, 0);
      break;

    case HOST_USER_CLASS_ACTIVE:
      osMessagePut ( StorageEvent, USBDISK_ACTIVE_CLASS_EVENT, 0);
      break;
    
    case HOST_USER_CONNECTION:
      osMessagePut ( StorageEvent, USBDISK_CONNECTION_EVENT, 0);
      break;
  }
}
#endif /* USE_USB_FS | USE_USB_HS */

/**
  * @brief  Storage Thread
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void StorageThread(void const * argument)
{
  osEvent event;

  for( ;; )
  {
    event = osMessageGet( StorageEvent, osWaitForever );

    if( event.status == osEventMessage )
    {
      switch(event.value.v)
      {
#if defined(USE_USB_FS) || defined(USE_USB_HS)
        case USBDISK_CONNECTION_EVENT:
          // Nothing to do here just wait for the MSC class to be active
          //StorageTryMount(USB_DISK_UNIT);
        break;

        case USBDISK_ACTIVE_CLASS_EVENT:
          StorageTryMount(USB_DISK_UNIT);
        break;

        case USBDISK_DISCONNECTION_EVENT:
          StorageTryUnMount(USB_DISK_UNIT);
        break;
#endif /* USE_USB_FS | USE_USB_HS */
#if defined(USE_SDCARD)
        case MSDDISK_CONNECTION_EVENT:
#ifdef STORAGE_BSP_INIT
          if(BSP_SD_Init(STORAGE_BSP_SD_ID) == BSP_ERROR_NONE)
          {
            StorageTryMount(MSD_DISK_UNIT);
          }
#else
          StorageTryMount(MSD_DISK_UNIT);
#endif /* STORAGE_BSP_INIT */
        break;

        case MSDDISK_DISCONNECTION_EVENT:
          StorageTryUnMount(MSD_DISK_UNIT);
#ifdef STORAGE_BSP_INIT
          BSP_SD_DeInit(STORAGE_BSP_SD_ID);
#endif /* STORAGE_BSP_INIT */
          /* Re-enable interrupts */
          BSP_SD_DetectITConfig(STORAGE_BSP_SD_ID);
        break;
#endif /* USE_SDCARD */
        default:
        break;
      }
    }
  }
}

#if defined(USE_SDCARD)
/**
  * @brief  MSD Storage initialization
  * @param  None
  * @retval int32_t Storage status
  */
static uint8_t StorageInitMSD(void)
{
  int32_t sd_status = BSP_ERROR_NONE;

  if(StorageID[MSD_DISK_UNIT] != 0)
    return StorageID[MSD_DISK_UNIT];

#ifdef STORAGE_BSP_INIT
  BSP_SD_DeInit(STORAGE_BSP_SD_ID);

  sd_status = BSP_SD_DetectITConfig(STORAGE_BSP_SD_ID);
  if( sd_status == BSP_ERROR_NONE )
  {
    /* Configure SD Interrupt mode */
    sd_status = BSP_SD_Init(STORAGE_BSP_SD_ID);
  }
#endif /* STORAGE_BSP_INIT */

  if ((sd_status == BSP_ERROR_NONE) || (sd_status == BSP_ERROR_UNKNOWN_COMPONENT))
  {
    /* Create Storage Semaphore */
    osSemaphoreDef(STORAGE_MSD_Semaphore);
    StorageSemaphore[MSD_DISK_UNIT] = osSemaphoreCreate (osSemaphore(STORAGE_MSD_Semaphore), 1);
    /* Mark the storage as initialized */
    StorageID[MSD_DISK_UNIT] = 1;
    Storage_Driver[MSD_DISK_UNIT] = &SD_Driver;
    /* Try mount the storage */
    StorageTryMount(MSD_DISK_UNIT);
  }

  return StorageID[MSD_DISK_UNIT];
}

/**
  * @brief  MSD Storage destruction
  * @param  None
  * @retval uint8_t Storage status
  */
static void StorageDeInitMSD(void)
{
#ifdef STORAGE_BSP_INIT
  BSP_SD_DeInit(STORAGE_BSP_SD_ID);
#endif /* STORAGE_BSP_INIT */
  
  if(StorageSemaphore[MSD_DISK_UNIT])
  {
    osSemaphoreDelete(StorageSemaphore[MSD_DISK_UNIT]);
    StorageSemaphore[MSD_DISK_UNIT] = 0;
  }
}
#endif /* USE_SDCARD */

#if defined(USE_USB_FS) || defined(USE_USB_HS)
/**
  * @brief  USB Storage initialization
  * @param  None
  * @retval uint8_t Storage status
  */
static uint8_t StorageInitUSB(void)
{
  USBH_StatusTypeDef  usb_status;

  if(StorageID[USB_DISK_UNIT] != 0)
    return StorageID[USB_DISK_UNIT];

  /* Enable the USB voltage level detector */
  HAL_PWREx_EnableUSBVoltageDetector();

  memset(&hUSBHost, 0, sizeof(hUSBHost));

  /* Init Host Library */
  usb_status = USBH_Init(&hUSBHost, USBH_UserProcess, 0);

  if(usb_status == USBH_OK)
  {
    /* Create Storage Semaphore */
    osSemaphoreDef(STORAGE_USB_Semaphore);
    StorageSemaphore[USB_DISK_UNIT] = osSemaphoreCreate (osSemaphore(STORAGE_USB_Semaphore), 1);
    /* Add Supported Class */
    usb_status = USBH_RegisterClass(&hUSBHost, USBH_MSC_CLASS);
    if(usb_status == USBH_OK)
    {
      /* Start Host Process */
      usb_status = USBH_Start(&hUSBHost);
    }
  }

  if(usb_status == USBH_OK)
  {
    /* Mark the storage as initialized */
    StorageID[USB_DISK_UNIT] = 1;
    Storage_Driver[USB_DISK_UNIT] = &USBH_Driver;
  }

  return StorageID[USB_DISK_UNIT];
}

/**
  * @brief  USB Storage destruction
  * @param  None
  * @retval uint8_t Storage status
  */
static void StorageDeInitUSB(void)
{
  USBH_DeInit(&hUSBHost);
  
  if(StorageSemaphore[USB_DISK_UNIT])
  {
    osSemaphoreDelete(StorageSemaphore[USB_DISK_UNIT]);
    StorageSemaphore[USB_DISK_UNIT] = 0;
  }
}
#endif /* USE_USB_FS | USE_USB_HS */

/**
  * @brief  Storage drives initialization
  * @param  None
  * @retval None
  */
void Storage_Init(void)
{
  uint8_t storage_id = 0;

  /* Reset All storage status */
  for(storage_id = 0; storage_id < sizeof(StorageStatus); storage_id++)
  {
    StorageStatus[storage_id] = STORAGE_NOINIT;
  }

  memset(StorageDISK_Drive[NUM_DISK_UNITS], 0, sizeof(StorageDISK_Drive[NUM_DISK_UNITS]));

  /* Create Storage Message Queue */
  osMessageQDef(osqueue, 30, uint16_t);
  StorageEvent = osMessageCreate (osMessageQ(osqueue), NULL);

#if defined(USE_SDCARD)
  /* Initialize the MSD Storage */
  StorageInitMSD();
#endif /* USE_SDCARD */

#if defined(USE_USB_FS) || defined(USE_USB_HS)
  /* Initialize the USB Storage */
  StorageInitUSB();
#endif /* USE_USB_FS | USE_USB_HS */

  /* Check for configured Storages then create Storage thread */
  for(storage_id = 0; storage_id < sizeof(StorageStatus); storage_id++)
  {
    if( StorageID[storage_id] )
    {
      /* It's Okay then Create Storage background task and exit from here */
      osThreadDef(STORAGE_Thread, StorageThread, STORAGE_THREAD_PRIORITY, 0, STORAGE_THREAD_STACK_SIZE);
      StorageThreadId = osThreadCreate (osThread(STORAGE_Thread), NULL);
      return;
    }
  }

  /* Something went wrong */
  //Error_Handler();
}

/**
  * @brief  Storage drives de-initialization
  * @param  None
  * @retval None
  */
void Storage_DeInit(void)
{
  uint8_t storage_id = 0;

  /* Try Unmount All storage */
  for(storage_id = 0; storage_id < sizeof(StorageStatus); storage_id++)
  {
    StorageTryUnMount(storage_id);
  }

  /* Terminate Storage background task */
  if(StorageThreadId)
  {
    osThreadTerminate (StorageThreadId);
    StorageThreadId = 0;
  }

  /* Delete Storage Message Queue */
  if(StorageEvent)
  {
    osMessageDelete (StorageEvent);
    StorageEvent = 0;
  }

#if defined(USE_USB_FS) || defined(USE_USB_HS)
  /* DeInit USB Storage */
  StorageDeInitUSB();
#endif /* USE_USB_FS | USE_USB_HS */

#if defined(USE_SDCARD)
  /* DeInit MSD Storage */
  StorageDeInitMSD();
#endif /* USE_SDCARD */
}

/**
  * @brief  Storage get status
  * @param  unit: logical storage unit index.
  * @retval int
  */
uint8_t Storage_GetStatus (uint8_t unit)
{
  uint8_t timeout = 10;
  uint8_t status = STORAGE_NOINIT;

  /* If it is just initialized then please wait for a while till init is done */
  while(timeout && (StorageStatus[unit] == STORAGE_NOINIT))
  {
    timeout--;
    osDelay(100);
  }

  if(StorageID[unit])
  {
    osSemaphoreWait(StorageSemaphore[unit], osWaitForever);
    status = (StorageStatus[unit] == STORAGE_MOUNTED);
    osSemaphoreRelease(StorageSemaphore[unit]);
  }

  return status;
}


/**
  * @brief  Storage get drive
  * @param  unit: logical storage unit index.
  * @retval char[4]
  */
const char *Storage_GetDrive (uint8_t unit)
{
  uint8_t timeout = 10;

  /* If it is just initialized then please wait for a while till init is done */
  while(timeout && (StorageStatus[unit] == STORAGE_NOINIT))
  {
    timeout--;
    osDelay(100);
  }

  if(StorageStatus[unit] == STORAGE_MOUNTED)
    return StorageDISK_Drive[unit];
  else
    return StorageDISK_Drive[NUM_DISK_UNITS];
}

#if defined(USE_SDCARD)
#if defined(USE_SDCARD_NO_DMA)
HAL_StatusTypeDef MX_SDMMC1_SD_Init(SD_HandleTypeDef *hsd)
{
  HAL_StatusTypeDef ret = HAL_OK;

  __HAL_RCC_SDMMC1_FORCE_RESET();
  __HAL_RCC_SDMMC1_RELEASE_RESET();

  /* uSD device interface configuration */
  hsd->Instance                 = SDMMC1;
  hsd->Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
  hsd->Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd->Init.BusWide             = SDMMC_BUS_WIDE_1B;
  hsd->Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd->Init.ClockDiv            = SDMMC_NSpeed_CLK_DIV * 4;
  hsd->Init.TranceiverPresent   = SDMMC_TRANSCEIVER_NOT_PRESENT;

  /* HAL SD initialization */
  if(HAL_SD_Init(hsd) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}
#endif /* USE_SDCARD_NO_DMA */

/**
  * @brief  BSP SD Callback.
  * @param  Instance SD Instance
  * @param  Status   Pin status
  * @retval None.
  */
void BSP_SD_DetectCallback(uint32_t Instance, uint32_t Status)
{
  if( (!StorageEvent) || (Instance != STORAGE_BSP_SD_ID)
   || (StorageStatus[MSD_DISK_UNIT] == STORAGE_NOINIT) )
    return;

  if(Status)
  {
    osMessagePut (StorageEvent, MSDDISK_CONNECTION_EVENT, 0);
  }
  else
  {
    osMessagePut (StorageEvent, MSDDISK_DISCONNECTION_EVENT, 0);
  }
}
#endif /* USE_SDCARD */

#ifdef SDCARD_LOW_SPEED
/**
  * @brief  Initializes the SDMMC1 peripheral.
  * @param  hsd SD handle
  * @retval HAL status
  */
HAL_StatusTypeDef MX_SDMMC1_SD_Init(SD_HandleTypeDef *hsd)
{
  HAL_StatusTypeDef ret = HAL_OK;
  /* uSD device interface configuration */
  hsd->Instance                 = SDMMC1;
  hsd->Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
  hsd->Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd->Init.BusWide             = SDMMC_BUS_WIDE_1B;
  hsd->Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd->Init.ClockDiv            = SDMMC_NSpeed_CLK_DIV * 4;
  hsd->Init.TranceiverPresent   = SDMMC_TRANSCEIVER_NOT_PRESENT;

  /* HAL SD initialization */
  if(HAL_SD_Init(hsd) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}
#endif
/**
  * @}
  */

/**
  * @}
  */

