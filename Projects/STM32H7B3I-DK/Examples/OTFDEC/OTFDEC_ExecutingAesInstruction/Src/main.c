/**
  * @file    OTFDEC_ExecutingAesInstruction/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to encrypt a binary image with standard AES-128 counter mode IP,
  *          load it in external OSPI memory and execute it using the OTFDEC peripheral to decrypt it on the fly
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
#include "ospi1_code.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef void (*Function_Pointer)(void);

/* Private define ------------------------------------------------------------*/
#define OSPI_BUFFER_SIZE   (BUFFER_SIZE+(16-(BUFFER_SIZE%16)))
#define ENC_BUFFER_SIZE    (OSPI_BUFFER_SIZE / 4L)
#define TIMEOUT_VALUE      0xFFFFFFFFUL
#define TRUE               1U
#define FALSE              0U

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef UartHandle;
OSPI_HandleTypeDef OspiHandle;
OSPI_MemoryMappedTypeDef sMemMappedCfg;
OSPI_RegularCmdTypeDef ospi_command;
CRYP_HandleTypeDef CrypHandle;
OTFDEC_HandleTypeDef OtfdecHandle;
OTFDEC_RegionConfigTypeDef OTFDEC_Config = {0};
uint32_t OspiSize = BUFFER_SIZE;
uint32_t NonceIn[2] = {0x01102332UL ,0x45546776UL};
uint32_t OtfdecKey128[4] = {0x0UL, 0x0UL, 0x0UL, 0x0UL};
uint32_t AesKey128[4] = {0x0F0E0D0CUL ,0x0B0A0908UL ,0x07060504UL ,0x03020100UL};
uint32_t AesIVCtr[4] = {0x0UL, 0x0UL, 0x0UL, 0x0UL};
#if defined ( __GNUC__ )
uint32_t  __attribute__((section ("CD_AHBSRAM1_BASE"))) CodeBufferSwapped[ENC_BUFFER_SIZE];
#elif defined ( __ICCARM__ )
#pragma location = CD_AXISRAM1_BASE
uint32_t CodeBufferSwapped[ENC_BUFFER_SIZE];
#elif defined ( __CC_ARM )
uint32_t CodeBufferSwapped[ENC_BUFFER_SIZE]   __attribute__((at(CD_AXISRAM1_BASE)));
#endif

#if defined ( __GNUC__ )
uint32_t  __attribute__((section ("CD_AHBSRAM1_BASE"))) OspiBufferSwappedEnc[ENC_BUFFER_SIZE];
#elif defined ( __ICCARM__ )
#pragma location = CD_AXISRAM2_BASE
uint32_t OspiBufferSwappedEnc[ENC_BUFFER_SIZE];
#elif defined ( __CC_ARM )
uint32_t OspiBufferSwappedEnc[ENC_BUFFER_SIZE]     __attribute__((at(CD_AXISRAM2_BASE)));
#endif

#if defined ( __GNUC__ )
uint32_t  __attribute__((section ("CD_AHBSRAM1_BASE"))) OspiBufferEnc[ENC_BUFFER_SIZE];
#elif defined ( __ICCARM__ )
#pragma location = CD_AXISRAM3_BASE
uint32_t OspiBufferEnc[ENC_BUFFER_SIZE];
#elif defined ( __CC_ARM )
uint32_t OspiBufferEnc[ENC_BUFFER_SIZE]       __attribute__((at(CD_AXISRAM3_BASE)));
#endif
uint16_t RegionVers = 0x0123UL;
volatile uint8_t CmdCplt, TxCplt;

  /* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
static void SystemClock_Config(void);
static void UART_Config(void);
static void OSPI_Init(void);
static void OSPI_FlashCode(uint8_t* pFlashAddr, uint32_t ospiAddr, uint32_t sizeToFlash);
static void OSPI_EraseMemory(void);
static void OSPI_WriteEnable(OSPI_HandleTypeDef *hospi);
static void OSPI_AutoPollingMemReady(OSPI_HandleTypeDef *hospi);
static void OSPI_SwitchSingleToOctal(OSPI_HandleTypeDef *hospi);
static void OSPI_MemoryMapped(void);
static void Error_Handler(void);
static void Encrypt_Code(uint8_t *pBufIn, uint8_t *pBufOut, uint32_t size);
static void Swap_Buffer(uint32_t * pBufIn, uint32_t * pBufOut, uint32_t size);
static void Setup_Regions(uint32_t modeMask);
static void Execute_Code(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the MPU attributes */
  MPU_Config();

  /* STM32H7xx HAL library initialization:
  - Configure the Systick to generate an interrupt each 1 msec
  - Set NVIC Group Priority to 4
  - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 280 MHz */
  SystemClock_Config();

  /* Configure usart to print messages */
  UART_Config();

  /* Initialise the OSPI */
  OSPI_Init();

  /* Configure the memory in octal mode */
  OSPI_SwitchSingleToOctal(&OspiHandle);

  printf("\n\r********** OCTOSPI%d OCTO MODE DTR *********************\n\r", OSPI_NUMBER);
  printf("CPU FREQUENCY: %ld MHz \n\r", SystemCoreClock / 1000000U);

  /* Erase ospi */
  OSPI_EraseMemory();

  /* Encrypt file */
  Encrypt_Code((uint8_t *) OSPI_Buffer, (uint8_t *) OspiBufferEnc, OSPI_BUFFER_SIZE);

  /* Flash file in OSPI */
  OSPI_FlashCode((uint8_t *) OspiBufferEnc, 0U, BUFFER_SIZE);

  /* Setup OTFDEC region for decryption */
  Setup_Regions(OTFDEC_REG_MODE_INSTRUCTION_OR_DATA_ACCESSES);

  /* Enable memory map */
  OSPI_MemoryMapped();

  /* Execute encrypted code */
  Execute_Code();

  while(TRUE)
  {
  }

}

/**
  * @brief  This function initializes the OSPI memory
  * @param  None
  * @retval None
  */
static void OSPI_Init(void)
{
  /* Initialize OctoSPI ----------------------------------------------------- */
  OspiHandle.Instance = OSPI_INSTANCE;
  HAL_OSPI_DeInit(&OspiHandle);

  OspiHandle.Init.FifoThreshold         = 4U;
  OspiHandle.Init.DualQuad              = HAL_OSPI_DUALQUAD_DISABLE;
  OspiHandle.Init.MemoryType            = HAL_OSPI_MEMTYPE_MACRONIX;
  OspiHandle.Init.DeviceSize            = OSPI_FLASH_SIZE;
  OspiHandle.Init.ChipSelectHighTime    = 2U;
  OspiHandle.Init.FreeRunningClock      = HAL_OSPI_FREERUNCLK_DISABLE;
  OspiHandle.Init.ClockMode             = HAL_OSPI_CLOCK_MODE_0;
  OspiHandle.Init.WrapSize              = HAL_OSPI_WRAP_NOT_SUPPORTED;
  OspiHandle.Init.ClockPrescaler        = 4U; /* OctoSPI clock = 280MHz / ClockPrescaler = 70MHz */
  OspiHandle.Init.SampleShifting        = HAL_OSPI_SAMPLE_SHIFTING_NONE;
  OspiHandle.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
  OspiHandle.Init.ChipSelectBoundary    = 0U;

  if (HAL_OSPI_Init(&OspiHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function erase the OSPI memory
  * @param  None
  * @retval None
  */
static void OSPI_EraseMemory(void)
{
  OSPI_RegularCmdTypeDef ospi_command;
  uint32_t ospi_addr = 0U;
  uint32_t max_size, nb_sectors_to_erase;
  uint8_t step = 0U, memErased = FALSE;

  ospi_command.OperationType        = HAL_OSPI_OPTYPE_COMMON_CFG;
  ospi_command.FlashId              = HAL_OSPI_FLASH_ID_1;
  ospi_command.InstructionMode      = OSPI_INSTRUCTION_MODE;
  ospi_command.InstructionSize      = OSPI_INSTRUCTION_SIZE;
  ospi_command.InstructionDtrMode   = OSPI_INSTRUCTION_DTRMODE;
  ospi_command.AddressSize          = OSPI_ADDRESS_SIZE;
  ospi_command.AddressDtrMode       = OSPI_ADDRESS_DTRMODE;
  ospi_command.AlternateBytesMode   = HAL_OSPI_ALTERNATE_BYTES_NONE;
  ospi_command.DataDtrMode          = OSPI_DATA_DTRMODE;
  ospi_command.SIOOMode             = HAL_OSPI_SIOO_INST_EVERY_CMD;
  ospi_command.DQSMode              = HAL_OSPI_DQS_DISABLE;

  max_size = OspiSize;
  nb_sectors_to_erase = (max_size / OSPI_SECTOR_SIZE) + 1U;

  while (memErased == FALSE)
  {
    switch(step)
    {
      case 0U:
        CmdCplt = FALSE;

        /* Enable write operations ------------------------------------------ */
        OSPI_WriteEnable(&OspiHandle);

        /* Erasing Sequence ------------------------------------------------- */
        ospi_command.OperationType = HAL_OSPI_OPTYPE_COMMON_CFG;
        ospi_command.Instruction   = OSPI_SECTOR_ERASE_CMD;
        ospi_command.AddressMode   = OSPI_SECTOR_ERASE_ADDRESS_MODE;
        ospi_command.AddressSize   = OSPI_SECTOR_ERASE_ADDRESS_SIZE;
        ospi_command.Address       = ospi_addr;
        ospi_command.DataMode      = HAL_OSPI_DATA_NONE;
        ospi_command.DummyCycles   = 0U;
        ospi_command.DQSMode       = HAL_OSPI_DQS_DISABLE;

        if (HAL_OSPI_Command_IT(&OspiHandle, &ospi_command) != HAL_OK)
        {
          Error_Handler();
        }

        step++;
        break;

      case 1U:
        if(CmdCplt != FALSE)
        {
          CmdCplt = FALSE;

          /* Configure automatic polling mode to wait for end of erase ------ */
          OSPI_AutoPollingMemReady(&OspiHandle);

          nb_sectors_to_erase --;
          if(nb_sectors_to_erase != 0U)
          {
            ospi_addr += OSPI_SECTOR_SIZE;
          }
          step++;
        }
        break;

    case 2U:
      TxCplt = FALSE;

      if(nb_sectors_to_erase != 0U)
      {
        step = 0U;
      }
      else
      {
        memErased = TRUE;
      }
      break;

      default :
        Error_Handler();
    }
  }
}

/**
  * @brief  This function flashes buffer in OSPI external memory
  * @param  pFlashAddr: buffer address to be uploaded in OSPI external memory
  * @param  ospiAddr: OSPI external memory index where to upload the buffer
  * @param  sizeToFlash: buffer size to be uploaded
  * @retval None
  */
static void OSPI_FlashCode(uint8_t* pFlashAddr, uint32_t ospiAddr, uint32_t sizeToFlash)
{
  OSPI_RegularCmdTypeDef ospi_command;
  uint32_t ospi_addr = 0U;
  uint32_t max_size, size;
  uint8_t *flash_addr;
  uint8_t step = 0U, codeFlashed = FALSE;

  flash_addr = pFlashAddr;
  ospi_addr = ospiAddr;
  max_size = sizeToFlash + ospiAddr;

  /* Copy only one page if the section is bigger */
  if (sizeToFlash > OSPI_PAGE_SIZE)
  {
    size = OSPI_PAGE_SIZE;
  }
  else
  {
    size = max_size;
  }

  ospi_command.FlashId            = HAL_OSPI_FLASH_ID_1;
  ospi_command.InstructionMode    = OSPI_INSTRUCTION_MODE;
  ospi_command.InstructionSize    = OSPI_INSTRUCTION_SIZE;
  ospi_command.InstructionDtrMode = OSPI_INSTRUCTION_DTRMODE;
  ospi_command.AddressDtrMode     = OSPI_ADDRESS_DTRMODE;
  ospi_command.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  ospi_command.DataDtrMode        = OSPI_DATA_DTRMODE;
  ospi_command.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

  while (codeFlashed == FALSE)
  {
    switch(step)
    {
      case 0U:
        /* Enable write operations ---------------------------------------- */
        OSPI_WriteEnable(&OspiHandle);

        /* Writing Sequence ----------------------------------------------- */
        ospi_command.OperationType   = HAL_OSPI_OPTYPE_COMMON_CFG;
        ospi_command.Instruction     = OSPI_PAGE_PROG_CMD;
        ospi_command.AddressMode     = OSPI_PAGE_PROG_ADDRESS_MODE;
        ospi_command.AddressSize     = OSPI_PAGE_PROG_ADDRESS_SIZE;
        ospi_command.DataMode        = OSPI_DATA_MODE;
        ospi_command.Address         = ospi_addr;
        ospi_command.NbData          = size;
        ospi_command.DummyCycles     = 0U;
        ospi_command.DQSMode         = HAL_OSPI_DQS_DISABLE;

        if (HAL_OSPI_Command(&OspiHandle, &ospi_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        {
          Error_Handler();
        }

        if (HAL_OSPI_Transmit_IT(&OspiHandle, flash_addr) != HAL_OK)
        {
          Error_Handler();
        }

        step++;
      break;

      case 1U:
        if(TxCplt != FALSE)
        {
          TxCplt = FALSE;

          /* Configure automatic polling mode to wait for end of program ----- */
          OSPI_AutoPollingMemReady(&OspiHandle);

          ospi_addr += size;
          flash_addr += size;

          /* Check if a new page writing is needed */
          if (ospi_addr < max_size)
          {
            /* Update the remaining size if it is less than the page size */
            if ((ospi_addr + size) > max_size)
            {
              size = max_size - ospi_addr;
            }
            step = 0U;
          }
          else
          {
            codeFlashed = TRUE;
          }
        }
        break;

      default :
        Error_Handler();
    }
  }
}

/**
  * @brief  This function enable the memory map mode
  * @param  None
  * @retval None
  */
static void OSPI_MemoryMapped(void)
{
  ospi_command.FlashId            = HAL_OSPI_FLASH_ID_1;
  ospi_command.InstructionMode    = OSPI_INSTRUCTION_MODE;
  ospi_command.InstructionSize    = OSPI_INSTRUCTION_SIZE;
  ospi_command.InstructionDtrMode = OSPI_INSTRUCTION_DTRMODE;
  ospi_command.AddressDtrMode     = OSPI_ADDRESS_DTRMODE;
  ospi_command.DataDtrMode        = OSPI_DATA_DTRMODE;
  ospi_command.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

  /* Memory-mapped mode configuration ------------------------------- */
  ospi_command.OperationType      = HAL_OSPI_OPTYPE_WRITE_CFG;
  ospi_command.Instruction        = OSPI_PAGE_PROG_CMD;
  ospi_command.AddressMode        = OSPI_PAGE_PROG_ADDRESS_MODE;
  ospi_command.AddressSize        = OSPI_PAGE_PROG_ADDRESS_SIZE;
  ospi_command.DataMode           = OSPI_DATA_MODE;
  ospi_command.DummyCycles        = OSPI_DUMMY_CYCLE;
  ospi_command.NbData             = 1U;

  if (HAL_OSPI_Command(&OspiHandle, &ospi_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  ospi_command.OperationType      = HAL_OSPI_OPTYPE_READ_CFG;
  ospi_command.Instruction        = OSPI_READ_CMD;
  ospi_command.AddressMode        = OSPI_READ_ADDRESS_MODE;
  ospi_command.AddressSize        = OSPI_READ_ADDRESS_SIZE;
  ospi_command.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  ospi_command.DataMode           = OSPI_DATA_MODE;
  ospi_command.DummyCycles        = OSPI_DUMMY_CYCLE;
  ospi_command.DQSMode            = OSPI_DQS_MODE;

  if (HAL_OSPI_Command(&OspiHandle, &ospi_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;

  if (HAL_OSPI_MemoryMapped(&OspiHandle, &sMemMappedCfg) != HAL_OK)
  {
    Error_Handler();
  }

}

/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @param  hospi: OSPI handle
  * @retval None
  */
static void OSPI_WriteEnable(OSPI_HandleTypeDef *hospi)
{
  OSPI_RegularCmdTypeDef  ospi_command;
  uint8_t reg[2U];

  /* Enable write operations ------------------------------------------ */
  ospi_command.OperationType        = HAL_OSPI_OPTYPE_COMMON_CFG;
  ospi_command.FlashId              = HAL_OSPI_FLASH_ID_1;
  ospi_command.Instruction          = OSPI_WRITE_ENABLE_CMD;
  ospi_command.InstructionMode      = OSPI_INSTRUCTION_MODE;
  ospi_command.InstructionSize      = OSPI_INSTRUCTION_SIZE;
  ospi_command.InstructionDtrMode   = OSPI_INSTRUCTION_DTRMODE;
  ospi_command.AddressMode          = HAL_OSPI_ADDRESS_NONE;
  ospi_command.AlternateBytesMode   = HAL_OSPI_ALTERNATE_BYTES_NONE;
  ospi_command.DataMode             = HAL_OSPI_DATA_NONE;
  ospi_command.DummyCycles          = 0U;
  ospi_command.DQSMode              = HAL_OSPI_DQS_DISABLE;
  ospi_command.SIOOMode             = HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(hospi, &ospi_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure automatic polling mode to wait for write enabling ---- */
  ospi_command.Instruction          = OSPI_READ_STATUS_REG_CMD;
  ospi_command.Address              = 0x0U;
  ospi_command.AddressMode          = OSPI_ADDRESS_MODE;
  ospi_command.AddressSize          = OSPI_ADDRESS_SIZE;
  ospi_command.AddressDtrMode       = OSPI_ADDRESS_DTRMODE;
  ospi_command.DataMode             = OSPI_DATA_MODE;
  ospi_command.DataDtrMode          = OSPI_DATA_DTRMODE;
  ospi_command.NbData               = 2U;
  ospi_command.DummyCycles          = OSPI_DUMMY_CYCLE;
  ospi_command.DQSMode              = OSPI_DQS_MODE;

  do
  {
    if (HAL_OSPI_Command(hospi, &ospi_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      Error_Handler();
    }

    if (HAL_OSPI_Receive(hospi, reg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      Error_Handler();
    }
  } while((reg[0U] & WRITE_ENABLE_MASK_VALUE) != WRITE_ENABLE_MATCH_VALUE);
}

/**
  * @brief  This function read the SR of the memory and wait the EOP.
  * @param  hospi: OSPI handle
  * @retval None
  */
static void OSPI_AutoPollingMemReady(OSPI_HandleTypeDef *hospi)
{
  OSPI_RegularCmdTypeDef  ospi_command;
  uint8_t reg[2U];

  /* Configure automatic polling mode to wait for memory ready ------ */
  ospi_command.OperationType        = HAL_OSPI_OPTYPE_COMMON_CFG;
  ospi_command.FlashId              = HAL_OSPI_FLASH_ID_1;
  ospi_command.Instruction          = OSPI_READ_STATUS_REG_CMD;
  ospi_command.InstructionMode      = OSPI_INSTRUCTION_MODE;
  ospi_command.InstructionSize      = OSPI_INSTRUCTION_SIZE;
  ospi_command.InstructionDtrMode   = OSPI_INSTRUCTION_DTRMODE;
  ospi_command.Address              = 0x0U;
  ospi_command.AddressMode          = OSPI_ADDRESS_MODE;
  ospi_command.AddressSize          = OSPI_ADDRESS_SIZE;
  ospi_command.AddressDtrMode       = OSPI_ADDRESS_DTRMODE;
  ospi_command.AlternateBytesMode   = HAL_OSPI_ALTERNATE_BYTES_NONE;
  ospi_command.DataMode             = OSPI_DATA_MODE;
  ospi_command.DataDtrMode          = OSPI_DATA_DTRMODE;
  ospi_command.NbData               = 2U;
  ospi_command.DummyCycles          = OSPI_DUMMY_CYCLE;
  ospi_command.DQSMode              = OSPI_DQS_MODE;
  ospi_command.SIOOMode             = HAL_OSPI_SIOO_INST_EVERY_CMD;

  do
  {
    if (HAL_OSPI_Command(hospi, &ospi_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      Error_Handler();
    }

    if (HAL_OSPI_Receive(hospi, reg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      Error_Handler();
    }
  } while((reg[0U] & MEMORY_READY_MASK_VALUE) != MEMORY_READY_MATCH_VALUE);
}

/**
  * @brief  Command completed callbacks.
  * @param  hospi: OSPI handle
  * @retval None
  */
void HAL_OSPI_CmdCpltCallback(OSPI_HandleTypeDef *hospi)
{
  CmdCplt++;
}

/**
  * @brief  Tx Transfer completed callback.
  * @param  hospi: OSPI handle
  * @retval None
  */
void HAL_OSPI_TxCpltCallback(OSPI_HandleTypeDef *hospi)
{
  TxCplt++;
}

/**
  * @brief  This function configure the memory in Octal DTR mode.
  * @param  hospi: OSPI handle
  * @retval None
  */
static void OSPI_SwitchSingleToOctal(OSPI_HandleTypeDef *hospi)
{
  uint8_t reg;
  OSPI_RegularCmdTypeDef  sCommand;
  OSPI_AutoPollingTypeDef sConfig;

  sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionMode    = HAL_OSPI_INSTRUCTION_1_LINE;
  sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles        = 0;
  sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

  sConfig.MatchMode           = HAL_OSPI_MATCH_MODE_AND;
  sConfig.AutomaticStop       = HAL_OSPI_AUTOMATIC_STOP_ENABLE;
  sConfig.Interval            = 0x10;


  /* Enable write operations */
  sCommand.Instruction = WRITE_ENABLE_CMD;
  sCommand.DataMode    = HAL_OSPI_DATA_NONE;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;

  if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Reconfigure OSPI to automatic polling mode to wait for write enabling */
  sConfig.Match           = 0x02;
  sConfig.Mask            = 0x02;

  sCommand.Instruction    = READ_STATUS_REG_CMD;
  sCommand.DataMode       = HAL_OSPI_DATA_1_LINE;
  sCommand.NbData         = 1;

  if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_AutoPolling(hospi, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Write Configuration register 2 (with Octal I/O SPI protocol) */
  sCommand.Instruction = WRITE_CFG_REG_2_CMD;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize = HAL_OSPI_ADDRESS_32_BITS;

  sCommand.Address = 0;
  reg = 0x2;


  if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_OSPI_Transmit(hospi, &reg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_Delay(40);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(TRUE)
  {
  }
}

/**
  * @brief  This function jumps to OSPI memory to execute code loaded in the external memory
  * @param  None
  * @retval None
  */
static void Execute_Code(void)
{
  Function_Pointer  jump_function;
  uint32_t  * ospi_jump_address;

  printf("********** Execute encrypted code with AES-CTR from OCTOSPI%d memory ********** \n\r", OSPI_NUMBER);

  /* Disable all activated interrupts */
  HAL_SuspendTick();
  HAL_NVIC_DisableIRQ(OSPI_IRQn);

  ospi_jump_address = (uint32_t *) EXTMEM_JUMP_ADDRESS;
  jump_function = (Function_Pointer) * ospi_jump_address;
  jump_function();
}

/**
  * @brief  This function encrypts code using the Crypto engine in AES 128 counter mode
  * @param  pBufIn: Input buffer containing data to be encrypted
  * @param  pBufOut: Output buffer containing encrypted data with Crypt AES-128 in counter mode
  * @param  size: buffer size to be encrypted
  * @retval None
  */
static void Encrypt_Code(uint8_t *pBufIn, uint8_t *pBufOut, uint32_t size)
{

  uint32_t index;

  /* Built AES IV for OTFDEC decryption */
  /* NonceIn must be swapped */
  AesIVCtr[0U] = NonceIn[1U];
  AesIVCtr[1U] = NonceIn[0U];
  AesIVCtr[2U] = RegionVers;
  AesIVCtr[3U] = (((OTFDREGION_ID-1U) << 28U) | (OTFDEC_REGION1_START_ADD >> 4U));

  /* Set the CRYP parameters in AES CTR Encryption */
  CrypHandle.Instance        = CRYP;
  CrypHandle.Init.DataType   = CRYP_DATATYPE_32B;
  CrypHandle.Init.KeySize    = CRYP_KEYSIZE_128B;
  CrypHandle.Init.pKey       = AesKey128;
  CrypHandle.Init.Algorithm  = CRYP_AES_CTR;
  CrypHandle.Init.pInitVect  = AesIVCtr;

  /* Initialize the CRYP  */
  HAL_CRYP_Init(&CrypHandle);

  /* Swap Input buffer to make it compatible with crypto */
  Swap_Buffer((uint32_t *) pBufIn, (uint32_t *) CodeBufferSwapped, size/4U);

  /* Encryption, result in  CodeBufferSwapped buffer */
  HAL_CRYP_Encrypt(&CrypHandle, (uint32_t *) CodeBufferSwapped, (uint16_t) size/4U, (uint32_t *) OspiBufferSwappedEnc, TIMEOUT_VALUE);

  /* Swap Encrypted buffer to make it compatible with otfdec */
  Swap_Buffer((uint32_t *) OspiBufferSwappedEnc, (uint32_t *) pBufOut, size/4U);

  /* Clean unused bytes */
  for (index=BUFFER_SIZE; index<size; index++)
	{
    pBufOut[index] = 0U;
	}
}

/**
  * @brief  This function allows to post or pre process buffer either it needs to be encrypted with AES-128 counter mode or OTFDEC
  * @brief  It converts 128bits in little or big endian format
  * @param  pBufIn: Input buffer to be swapped
  * @param  pBufOut: Output buffer with swapped words
  * @param  size: buffer size
  * @retval None
  */
static void Swap_Buffer(uint32_t * pBufIn, uint32_t * pBufOut, uint32_t size)
{
  uint32_t index_i, index_k;

  for (index_i=0U; index_i<size; index_i+=4U)
  {
    for (index_k=0U; index_k<4U; index_k++)
		{
      pBufOut[index_i+index_k] = pBufIn[index_i+3U-index_k];
		}
  }

}

/**
  * @brief  This function configures the OTFDEC selected region
  * @param  modeMask: Decryption mode to be applied
  * @retval None
  */
static void Setup_Regions(uint32_t modeMask)
{
  /* AES128Key must be swapped to comply with OTFDEC */
  Swap_Buffer((uint32_t *) AesKey128, (uint32_t *) OtfdecKey128, 4U);

  /* Instantiate OTFDEC region */
  OtfdecHandle.Instance = OTFDEC1;
  if (HAL_OTFDEC_Init(&OtfdecHandle) != HAL_OK)
  {
    Error_Handler();
  }

   /* Set OTFDEC Mode */
  if (HAL_OTFDEC_RegionSetMode( &OtfdecHandle,Region1, modeMask) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set OTFDEC Key */
  if (HAL_OTFDEC_RegionSetKey( &OtfdecHandle,Region1, OtfdecKey128) != HAL_OK)
  {
    Error_Handler();
  }

  /* Activate OTFDEC decryption */
  OTFDEC_Config.Nonce[0]     = NonceIn[0U];
  OTFDEC_Config.Nonce[1]     = NonceIn[1U];
  OTFDEC_Config.StartAddress = OTFDEC_REGION1_START_ADD;
  OTFDEC_Config.EndAddress   = OTFDEC_REGION1_END_ADD;
  OTFDEC_Config.Version      = RegionVers;
  if (HAL_OTFDEC_RegionConfig(&OtfdecHandle,Region1,  &OTFDEC_Config, OTFDEC_REG_CONFIGR_LOCK_ENABLE)!= HAL_OK)
  {
    Error_Handler();
  }

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 280000000 (CPU Clock)
  *            HCLK(Hz)                       = 280000000 (Bus matrix and AHBs Clock)
  *            AHB Prescaler                  = 1
  *            CD APB3 Prescaler              = 2 (APB3 Clock  140MHz)
  *            CD APB1 Prescaler              = 2 (APB1 Clock  140MHz)
  *            CD APB2 Prescaler              = 2 (APB2 Clock  140MHz)
  *            SRD APB4 Prescaler             = 2 (APB4 Clock  140MHz)
  *            HSE Frequency(Hz)              = 24000000
  *            PLL_M                          = 12
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
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* The voltage scaling allows optimizing the power consumption when the device is
  clocked below the maximum system frequency, to update the voltage scaling value
  regarding system frequency refer to product datasheet.
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 280;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
    RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

    /*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
          (GPIO, SPI, FMC, OSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
          The I/O Compensation Cell activation  procedure requires :
        - The activation of the CSI clock
        - The activation of the SYSCFG clock
        - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR

          To do this please uncomment the following code
  */

  __HAL_RCC_CSI_ENABLE() ;

  __HAL_RCC_SYSCFG_CLK_ENABLE() ;

  HAL_EnableCompensationCell();
}


/**
  * @brief  Configures the USARTx and associated pins.
  * @param  None
  * @retval None
  */
static void UART_Config(void)
{
  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits (7 data bit + 1 parity bit) :
       BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
      - Stop Bit    = One Stop bit
      - Parity      = No parity
      - BaudRate    = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance          = USARTx;

  UartHandle.Init.BaudRate     = 115200U;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  UartHandle.Init.Parity       = UART_PARITY_ODD;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

  HAL_UART_Init(&UartHandle);
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1U, 0xFFFFFFUL);

  return ch;
}

/**
  * @brief  Configure the MPU attributes
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU as Strongly ordered for not defined regions */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x00;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU OSPI flash */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x90000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_64MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @}
  */

/**
  * @}
  */

