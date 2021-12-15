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
static UART_HandleTypeDef UART_Handle;
BSP_QSPI_Init_t init ;
TS_Init_t hTS;
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define DEMO_DESCRITION                 "Out Of the Box Demonstration V1.1.0"
#define STML_DESCRITION                 "ST Menu Launcher V1.1.0"
#define BOARD_NAME                      "STM32H747I-EVAL_MB1246"

#define HSEM_ID_0                       (0U) /* HW semaphore 0*/

#define MAX_FLASH_WRITE_FAILURE         10

#define USARTx                          USART1

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

// #define __RAM_CODE_SECTION __attribute__ ((section ("flasher_code_section"))) __RAM_FUNC
#define __RAM_CODE_SECTION __attribute__ ((section ("flasher_code_section")))
// #define __RAM_CODE_SECTION __RAM_FUNC
// #define __RAM_CODE_SECTION

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
__RAM_CODE_SECTION
HAL_StatusTypeDef HAL_FLASH_Unlock(void);
__RAM_CODE_SECTION
uint32_t HAL_FLASH_GetError(void);
__RAM_CODE_SECTION
HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint32_t DataAddress);
__RAM_CODE_SECTION
HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *SectorError);
__RAM_CODE_SECTION
HAL_StatusTypeDef HAL_FLASHEx_Unlock_Bank1(void);
__RAM_CODE_SECTION
HAL_StatusTypeDef HAL_FLASHEx_Unlock_Bank2(void);


/* Private functions ---------------------------------------------------------*/
__STATIC_INLINE void my_data_copy(void *dst, void *src, size_t size)
{
  printf("Copying data from 0x%08X to 0x%08X - size is %u\n", (unsigned int )src, (unsigned int )dst, size);
  memcpy(dst, src, size);
}

/* copy all bytes between s (inclusive) and e (exclusive) to d */
/* copy the bytes from ITCMROM_region to ITCMRAM_region */
static void Copy_EXT_RAM_data(void)
{
#if defined ( __GNUC__ )
	extern char EXT_RAM_START asm("EXT_RAM_START");
	extern char EXT_RAM_END asm("EXT_RAM_END");
	extern char EXT_ROM_START asm("EXT_ROM_START");
	size_t size = (size_t) (&EXT_RAM_END - &EXT_RAM_START);
  my_data_copy(&EXT_RAM_START, &EXT_ROM_START, size);
#elif defined ( __CC_ARM )
	extern uint32_t Load$$RO_EXT_RAM$$RO$$Base[];
	extern uint32_t Load$$RO_EXT_RAM$$RO$$Length[];
	extern uint32_t Image$$RO_EXT_RAM$$RO$$Base[];
	my_data_copy((void *) Image$$RO_EXT_RAM$$RO$$Base, Load$$RO_EXT_RAM$$RO$$Base, (unsigned long) Load$$RO_EXT_RAM$$RO$$Length);
#elif defined (__ICCARM__)
	#pragma segment="EXT_RAM"
	#pragma segment="EXT_ROM"
	size_t size = (size_t)((uint32_t)__sfe("EXT_ROM") - (uint32_t)__sfb("EXT_ROM"));
	my_data_copy(__sfb("EXT_RAM"), __sfb("EXT_ROM"), size);
#endif
}

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
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static inline uint32_t GetPage(uint32_t Addr)
{
  if IS_FLASH_PROGRAM_ADDRESS_BANK1(Addr)
  {
    /* Bank 1 */
    return (Addr - FLASH_BANK1_BASE) / FLASH_SECTOR_SIZE;
  }
  else
  {
    /* Bank 2 */
    return (Addr - FLASH_BANK2_BASE) / FLASH_SECTOR_SIZE;
  }
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
static inline uint32_t GetBank(uint32_t Addr)
{
  uint32_t bank = 0;

  if (READ_BIT(SYSCFG->UR0, SYSCFG_UR0_BKS) == 0)
  {
  	/* No Bank swap */
    if IS_FLASH_PROGRAM_ADDRESS_BANK1(Addr)
    {
      bank = FLASH_BANK_1;
    }
    else
    {
      bank = FLASH_BANK_2;
    }
  }
  else
  {
  	/* Bank swap */
    if IS_FLASH_PROGRAM_ADDRESS_BANK1(Addr)
    {
      bank = FLASH_BANK_2;
    }
    else
    {
      bank = FLASH_BANK_1;
    }
  }

  return bank;
}

static void LCD_LL_Reset(void)
{
  GPIO_InitTypeDef  gpio_init_structure;

  __HAL_RCC_GPIOF_CLK_ENABLE();

  /* Configure the GPIO on PF10 */
  gpio_init_structure.Pin   = GPIO_PIN_10;
  gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull  = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(GPIOF, &gpio_init_structure);

  /* Activate XRES active low */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET);

  HAL_Delay(20); /* wait 20 ms */

  /* Deactivate XRES */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);

  /* Wait for 300 ms after releasing XRES before sending commands */
  HAL_Delay(300);
}

/**
  * @brief  Initializes the STM324x9I-EVAL's LCD and LEDs resources.
  * @param  None
  * @retval None
  */
uint8_t BSP_Config(void)
{
  uint8_t RetVal = 0;
  uint8_t counter = 10;

  /* Enable CRC to Unlock GUI */
  __HAL_RCC_CRC_CLK_ENABLE();

  /* Configure LEDs */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  /* Init Wakeup/Tamper push-button in EXTI Mode */
  BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_EXTI);
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);

  BSP_ConfigSerial();

  /* Print OoB Demo version */
  printf( "\n\n%s: %s (Build %s at %s)\n\n", BOARD_NAME, DEMO_DESCRITION, __DATE__, __TIME__ );

  /* Print ML version */
  printf( "%s: %s\n", BOARD_NAME, STML_DESCRITION );
  
  /* Print Clock configuration */
  printf( "CPU running at %luMHz, Peripherals at %luMHz/%luMHz\n" , (HAL_RCCEx_GetD1SysClockFreq()/1000000U)
                                                                  , (HAL_RCC_GetPCLK1Freq()/1000000U)
                                                                  , (HAL_RCC_GetPCLK2Freq()/1000000U) );

  /* Initialize the SDRAM memory */
  if ((RetVal = BSP_SDRAM_Init(0)) != BSP_ERROR_NONE)
  {
    printf("Failed to initialize the SDRAM !! (Error %d)\n", RetVal);
    return 0;
  }
  init.InterfaceMode=MT25TL01G_QPI_MODE;
  init.TransferRate= MT25TL01G_DTR_TRANSFER ;
  init.DualFlashMode= MT25TL01G_DUALFLASH_ENABLE;
  /* Initialize the NOR QuadSPI flash */
  if ((RetVal = BSP_QSPI_Init(0,&init)) != BSP_ERROR_NONE)
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

  /* Initialize the Touch screen */
  LCD_LL_Reset(); /* LCD controller need to be initialized */
  do
  {
    hTS.Width = 800;
    hTS.Height = 480;
    hTS.Orientation = TS_SWAP_XY | TS_SWAP_Y;
    hTS.Accuracy = 0;
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

  /* Enable Back up SRAM */
  /* Enable write access to Backup domain */
  PWR->CR1 |= PWR_CR1_DBP;
  while((PWR->CR1 & PWR_CR1_DBP) == RESET)
  {
  }
  /*Enable BKPRAM clock*/
  __HAL_RCC_BKPRAM_CLK_ENABLE();

  Copy_EXT_RAM_data();

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

  BSP_TS_GetState(0,(TS_State_t *)&ts);

  ts.TouchX = TouchScreen_Get_Calibrated_X(ts.TouchX);
  ts.TouchY = TouchScreen_Get_Calibrated_Y(ts.TouchY);

  if((ts.TouchX >= LCD_GetXSize()) || (ts.TouchY >= LCD_GetYSize()))
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

  /* Disable LCD */
  LCD_Off();

  GUI_Delay(200);
  
  /* Disable the MPU */
  HAL_MPU_Disable();

  /* DeInit Storage */
  Storage_DeInit();

  printf("\nJumping to Sub-Demo @0x%08lu ...\n", SubDemoAddress);

  /* Disable and Invalidate I-Cache */
  SCB_DisableICache();
  SCB_InvalidateICache();

  /* Disable, Clean and Invalidate D-Cache */
  SCB_DisableDCache();
  SCB_CleanInvalidateDCache();

  /* Do Software Reset */
  HAL_NVIC_SystemReset();
}

static int BSP_VerifyData(const uint64_t *pData, const uint64_t *pFlash, uint32_t DataSize)
{
  uint32_t i;
  for(i = 0; i < DataSize; i+=8)
  {
    if (pData[i] != pFlash[i] )
      return 1;
  }

  return 0;
}

/**
  * @brief  Copy resource file into the OctoSPI memory
  * @param  hItem    : Progress bar used to indicate the transfer progression
  * @param  pResFile : Resource file to be copied in the OctoSPI memory
  * @param  Address  : Address where the resource will be copied
  * @retval ErrorCode : 0 if success -1 otherwise
  */
int BSP_ResourcesCopy(WM_HWIN hItem, FIL * pResFile, uint32_t Address)
{
  int RetErr = 0;
  BSP_QSPI_Info_t QSPIInfo;
  uint32_t file_size = 0;
  uint32_t numOfReadBytes = 0, FlashAddr = 0, nbTotalBytes = 0;
  int32_t ospiStatus = BSP_ERROR_NONE;
  uint8_t *pSdData = 0;
  int sector = 0, nb_sectors = 0;

  /* Sop Memory Mapping mode */
  BSP_QSPI_DeInit(0);

  /* Re-initialize NOR OctoSPI flash to exit memory-mapped mode */
  if (BSP_QSPI_Init(0,&init) != BSP_ERROR_NONE)
  {
    /* Initialization Error */
    return -1;
  }

  /* Initialize the structure */
  QSPIInfo.FlashSize          = (uint32_t)0x00;
  QSPIInfo.EraseSectorSize    = (uint32_t)0x00;
  QSPIInfo.EraseSectorsNumber = (uint32_t)0x00;
  QSPIInfo.ProgPageSize       = (uint32_t)0x00;
  QSPIInfo.ProgPagesNumber    = (uint32_t)0x00;

  ospiStatus =BSP_QSPI_GetInfo(0,&QSPIInfo);
  if (ospiStatus != BSP_ERROR_NONE)
  {
    RetErr = -1;
    goto exit;
  }

  /* Test the correctness */
  if((QSPIInfo.FlashSize          != EXT_FLASH_SIZE) ||
     (QSPIInfo.EraseSectorSize    != EXT_FLASH_SUBSECTOR_SIZE) ||
     (QSPIInfo.EraseSectorsNumber != (EXT_FLASH_SIZE/EXT_FLASH_SUBSECTOR_SIZE)) ||
     (QSPIInfo.ProgPageSize       != EXT_FLASH_PAGE_SIZE)  ||
     (QSPIInfo.ProgPagesNumber    != (EXT_FLASH_SIZE/EXT_FLASH_PAGE_SIZE)))
  {
    RetErr = -1;
    goto exit;
  }

   pSdData = ff_malloc(QSPIInfo.EraseSectorSize);
   if (pSdData == 0)
   {
     RetErr = -2;
     goto exit;
   }

  if (f_lseek(pResFile, 0) != FR_OK)
  {
    RetErr = -3;
    goto exit;
  }

  file_size = f_size(pResFile);

  nb_sectors = (file_size / QSPIInfo.EraseSectorSize);
  if(file_size % QSPIInfo.EraseSectorSize)
    nb_sectors++;

  PROGBAR_SetMinMax(hItem, 0, nb_sectors);

  FlashAddr = (Address - QSPI_BASE_ADDRESS);
  do
  {
    memset(pSdData, 0xFF, QSPIInfo.EraseSectorSize);

    /* Read and Program data */
    if(f_read(pResFile, pSdData, QSPIInfo.EraseSectorSize, (void *)&numOfReadBytes) != FR_OK)
    {
      RetErr = -4;
      goto exit;
    }

    ospiStatus = BSP_QSPI_EraseBlock(0,FlashAddr,BSP_QSPI_ERASE_8K);
    if (ospiStatus != BSP_ERROR_NONE)
    {
      RetErr = -1;
      goto exit;
    }

    ospiStatus = BSP_QSPI_Write(0,(uint8_t *)pSdData, FlashAddr, numOfReadBytes);
    if (ospiStatus != BSP_ERROR_NONE)
    {
      RetErr = -1;
      goto exit;
    }

    /* Wait the end of write operation */
    do
    {
      ospiStatus = BSP_QSPI_GetStatus(0);
    } while (ospiStatus == BSP_ERROR_BUSY);

    /* Check the write operation correctness */
    if (ospiStatus != BSP_ERROR_NONE)
    {
      RetErr = -1;
      goto exit;
    }

    FlashAddr     += numOfReadBytes;
    nbTotalBytes  += numOfReadBytes;

    PROGBAR_SetValue(hItem, ++sector);
    GUI_Exec();

  } while((numOfReadBytes == QSPIInfo.EraseSectorSize) && (nbTotalBytes < file_size));

exit:
  if(pSdData)
  {
    ff_free(pSdData);
  }
  
  /* Reconfigure memory mapped mode */
  if(BSP_QSPI_EnableMemoryMappedMode(0) != BSP_ERROR_NONE)
  {
    RetErr = -1;
  }

  if(RetErr)
    printf("\nFailed : Error=%d, ospiStatus=%d !!\n", RetErr, (int)ospiStatus);

  GUI_Exec();

  return RetErr;
}

uint8_t BSP_SuspendCPU2 ( void )
{
  uint8_t dual_core = 0;
  int32_t timeout   = 0xFFFF;

  printf("Suspending CPU2 : ");

  /* Check if the CPU 2 is up and running */
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
  if ( timeout > 0 )
  {
    /*Take HSEM */
    HAL_HSEM_FastTake(HSEM_ID_0);
    /*Release HSEM in order to notify the CPU2(CM4)*/
    HAL_HSEM_Release(HSEM_ID_0, 0);

    /* Wait until CPU2 enter in STOP mode */
    timeout   = 0x1FFFF;
    while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
    if ( timeout > 0 )
    {
      dual_core = 1;
    }
  }

  if ( dual_core )
    printf("OK\n");
  else
    printf("OK\n");

  return dual_core;
}

uint8_t BSP_ResumeCPU2 ( void )
{
  uint8_t dual_core = 0;
  int32_t timeout   = 0xFFFF;

  printf("Resuming CPU2 : ");

  /* Check if the CPU 2 is already suspended */
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout > 0 )
  {
    /*Take HSEM */
    HAL_HSEM_FastTake(HSEM_ID_0);
    /*Release HSEM in order to notify the CPU2(CM4)*/
    HAL_HSEM_Release(HSEM_ID_0, 1);

    /* Wait until CPU2 boots and enters in stop mode or timeout*/
    timeout = 0xFFFF;
    while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
    if ( timeout > 0 )
    {
      dual_core = 1;
    }
  }

  if ( dual_core )
    printf("OK\n");
  else
    printf("OK\n");

  return dual_core;
}

/**
  * @brief  Copy program code file into the Internal Flash memory
  * @param  hItem    : Progress bar used to indicate the transfer progression
  * @param  pResFile : Program code file to be copied in the Internal Flash memory
  * @param  Address  : Address where the code will be copied
  * @retval ErrorCode : 0 if success -1 otherwise
  */
int BSP_FlashProgram(WM_HWIN hItem, FIL * pResFile, uint32_t Address)
{
  FLASH_EraseInitTypeDef EraseInitStruct;

  int Ret = 0;
  uint8_t dual_core = 0;
  uint32_t EraseError = 0;
  uint32_t numOfReadBytes = 0, nbTotalBytes = 0;
  uint32_t offset = 0;
  uint8_t *pSdData;
  uint64_t *pData256, *pFlash256;
  int sector = 0;

  pSdData = (uint8_t *)ff_malloc(FLASH_SECTOR_SIZE);
  if (pSdData == NULL)
  {
    return -1;
  }

  if (f_lseek(pResFile, 0) != FR_OK)
  {
    ff_free(pSdData);
    return -2;
  }

  /* Get the 1st page to erase */
  const uint32_t StartSector = GetPage(Address);
  /* Get the number of pages to erase from 1st page */
  const uint32_t NbOfSectors = GetPage(Address + f_size(pResFile) -1) - StartSector + 1;
  /* Get the bank */
  const uint32_t BankNumber = GetBank(Address);

  /* Clear pending flags (if any) */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_BANK1);
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_BANK2);

  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  /* Clear all error flags */
  if(BankNumber == FLASH_BANK_1)
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);
  else
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK2);

  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Banks         = BankNumber;
  EraseInitStruct.Sector        = StartSector;
  EraseInitStruct.NbSectors     = NbOfSectors;

  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &EraseError) != HAL_OK)
  {
    Ret = HAL_FLASH_GetError();
    goto unlock_and_exit;
  }

  PROGBAR_SetMinMax(hItem, 0, NbOfSectors);
  pFlash256 = (uint64_t *)Address;

  do
  {
    memset(pSdData, 0xFF, FLASH_SECTOR_SIZE);

    Ret = f_read(pResFile, pSdData, FLASH_SECTOR_SIZE, (void *)&numOfReadBytes);
    if(Ret == FR_OK)
    {
      uint8_t failure = 0;
      offset          = 0;
      pData256       = (uint64_t *)pSdData;

      /* Disable, Clean and Invalidate D-Cache */
      SCB_DisableDCache();
      SCB_CleanInvalidateDCache();

      /* Program the user Flash area word by word */
      while (offset < numOfReadBytes)
      {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, (uint32_t )pFlash256, (uint32_t )pData256) == HAL_OK)
        {
          if( BSP_VerifyData(pData256, pFlash256, (FLASH_BURST_WIDTH / (sizeof(uint64_t) * sizeof(uint64_t)))) )
          {
            Ret = -1;
            /* Enable D-Cache */
            SCB_EnableDCache();
            goto unlock_and_exit;
          }
          pData256  += (FLASH_BURST_WIDTH / (sizeof(uint64_t) * sizeof(uint64_t)));
          pFlash256 += (FLASH_BURST_WIDTH / (sizeof(uint64_t) * sizeof(uint64_t)));
          offset    += (FLASH_BURST_WIDTH / (sizeof(uint64_t) * sizeof(uint8_t)));
          failure    = 0;
        }
        else
        {
          if (failure++ > MAX_FLASH_WRITE_FAILURE)
          {
            Ret = HAL_FLASH_GetError();
            /* Enable D-Cache */
            SCB_EnableDCache();
            goto unlock_and_exit;
          }
        }
      }

      /* Enable D-Cache */
      SCB_EnableDCache();

      nbTotalBytes += numOfReadBytes;
    }
    else
    {
      goto unlock_and_exit;
    }

    PROGBAR_SetValue(hItem, ++sector);
    GUI_Exec();

  } while(numOfReadBytes == FLASH_SECTOR_SIZE);

unlock_and_exit:
  if(BankNumber == FLASH_BANK_1)
    HAL_FLASHEx_Unlock_Bank1();
  else
    HAL_FLASHEx_Unlock_Bank2();

  ff_free(pSdData);
  GUI_Exec();

  if(dual_core)
  {
    BSP_ResumeCPU2();
  }

  return Ret;
}

__RAM_CODE_SECTION
static int __update_flash(FLASH_EraseInitTypeDef *EraseInitStruct, const uint64_t *pFlash256, const uint64_t *pData256, uint32_t Size)
{
  int       Ret = 0;
  uint32_t  EraseError = 0;
  uint32_t  offset = 0;
  uint8_t   failure = 0;

  /* Clear pending flags (if any) */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_BANK1);
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_BANK2);

  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  /* Clear all error flags */
  if(EraseInitStruct->Banks == FLASH_BANK_1)
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK1);
  else
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS_BANK2);

  __disable_irq();

  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
  if (HAL_FLASHEx_Erase(EraseInitStruct, &EraseError) != HAL_OK)
  {
    Ret = HAL_FLASH_GetError();
    goto unlock_and_exit;
  }

  /* Program the user Flash area word by word */
  offset    = 0;
  while (offset < FLASH_SECTOR_SIZE)
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, (uint32_t )pFlash256, (uint32_t )pData256) == HAL_OK)
    {
      if( BSP_VerifyData(pData256, pFlash256, (FLASH_BURST_WIDTH / (sizeof(uint64_t) * sizeof(uint64_t)))) )
      {
        Ret = -1;
        goto unlock_and_exit;
      }
      pData256  += (FLASH_BURST_WIDTH / (sizeof(uint64_t) * sizeof(uint64_t)));
      pFlash256 += (FLASH_BURST_WIDTH / (sizeof(uint64_t) * sizeof(uint64_t)));
      offset    += (FLASH_BURST_WIDTH / (sizeof(uint64_t) * sizeof(uint8_t)));
      failure    = 0;
    }
    else
    {
      if (failure++ > MAX_FLASH_WRITE_FAILURE)
      {
        Ret = HAL_FLASH_GetError();
        goto unlock_and_exit;
      }
    }
  }

unlock_and_exit:
  if(EraseInitStruct->Banks == FLASH_BANK_1)
    HAL_FLASHEx_Unlock_Bank1();
  else
    HAL_FLASHEx_Unlock_Bank2();

  __enable_irq();
  return Ret;
}

/**
  * @brief  Copy program code file into the Internal Flash memory
  * @param  Address   : Address where the code will be copied
  * @param  pData     : Program code file to be copied in the Internal Flash memory
  * @param  Size      : Size of the data to be programmed
  * @retval ErrorCode : 0 if success -1 otherwise
  */
int BSP_FlashUpdate(uint32_t Address, uint8_t *pData, uint32_t Size)
{
  int             Ret = 0;
  uint8_t         dual_core = 0;
  const uint64_t *pFlash256;
  uint64_t *pData256;
  uint64_t *pDst256;
  uint64_t *SectorData;
  uint32_t  offset = 0;
  FLASH_EraseInitTypeDef EraseInitStruct;

  if (pData == NULL)
  {
    return -1;
  }

  SectorData = (uint64_t *)ff_malloc(FLASH_SECTOR_SIZE);
  if (SectorData == NULL)
  {
    return -2;
  }
  memset(SectorData, 0xFF, FLASH_SECTOR_SIZE);

  /* Get the 1st page to erase */
  const uint32_t StartSector = GetPage(Address);
  /* Get the number of pages to erase from 1st page */
  const uint32_t NbOfSectors = GetPage(Address + Size - 1) - StartSector + 1;
  /* Get the bank */
  const uint32_t BankNumber = GetBank(Address);

  pFlash256 = (uint64_t *)(FLASH_BASE + (StartSector * FLASH_SECTOR_SIZE));
  /* Read current data */
  for(offset = 0; offset < (FLASH_SECTOR_SIZE / 8); offset++ )
  {
    SectorData[offset] = pFlash256[offset];
  }

  pData256  = (uint64_t *)pData;
  pDst256   = (uint64_t *)((uint32_t )SectorData + (uint32_t )(Address & (FLASH_SECTOR_SIZE -1)));
  /* Update DC */
  for(offset = 0; offset < (Size/8); offset++ )
  {
    *pDst256++ = *pData256++;
  }

  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Banks         = BankNumber;
  EraseInitStruct.Sector        = StartSector;
  EraseInitStruct.NbSectors     = NbOfSectors;

  /* Disable, Clean and Invalidate D-Cache */
  SCB_DisableDCache();
  SCB_CleanInvalidateDCache();

  Ret = __update_flash(&EraseInitStruct, pFlash256, SectorData, Size );

  /* Enable D-Cache */
  SCB_EnableDCache();

  if(dual_core)
  {
    BSP_ResumeCPU2();
  }

  ff_free(SectorData);
  return Ret;
}

#if defined (QSPI_LOW_FREQ)
/**
  * @brief  Initializes the QSPI interface.
  * @param  hQspi       QSPI handle
  * @param  Config      QSPI configuration structure
  * @retval BSP status
  */
HAL_StatusTypeDef MX_QSPI_Init(QSPI_HandleTypeDef *hQspi, MX_QSPI_Init_t *Config)
{
  /* patch Prescaler to lower frequency */
  Config->ClockPrescaler = 5;

  /* QSPI initialization */
  /* QSPI freq = SYSCLK /(1 + ClockPrescaler) Mhz */
  hQspi->Instance                = QUADSPI;
  hQspi->Init.ClockPrescaler     = Config->ClockPrescaler;
  hQspi->Init.FifoThreshold      = 1;
  hQspi->Init.SampleShifting     = Config->SampleShifting;
  hQspi->Init.FlashSize          = Config->FlashSize;
  hQspi->Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_4_CYCLE; /* Min 50ns for nonRead */
  hQspi->Init.ClockMode          = QSPI_CLOCK_MODE_0;
  hQspi->Init.FlashID            = QSPI_FLASH_ID_1;
  hQspi->Init.DualFlash          = Config->DualFlashMode;

  return HAL_QSPI_Init(hQspi);
}
#endif /* QSPI_LOW_FREQ */

/**
  * @}
  */

/**
  * @}
  */

