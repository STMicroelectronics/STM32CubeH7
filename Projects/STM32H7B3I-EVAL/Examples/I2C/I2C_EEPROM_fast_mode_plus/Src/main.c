/**
  ******************************************************************************
  * @file    I2C/I2C_EEPROM_fast_mode_plus/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32H7xx I2C HAL API to transmit
  *          and receive a data buffer with a communication process based on
  *          DMA transfer. 
  *          The communication is done using an EEPROM M24LR64
  *          on STM32H7B3I-EVAL Eval board.
  *          =================================================================== 
  *          Notes:
  *           - This example is intended for STM32H7B3xxQ families devices only. 
  *           - The I2C EEPROM memory (M24LR64) is compatible
  *             with the I2C Fast Mode Plus.

  *          ===================================================================
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

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup I2C_EEPROM_fast_mode_plus
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define EEPROM_ADDRESS             0xA6    /* EEPROM M24LR64 Address  */
#define EEPROM_PAGESIZE            4     /* EEPROM M24LR64 used     */
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define I2C_XFER_TIMEOUT_MAX    300
/* When using M24LR64 set TIMING to 0xD0902D2F to reach 1 MHz speed */
/* When using M24LR64 set TIMING to 0xD0902D2F to reach 100 KHz speed */
/* I2C TIMING Register define when I2C clock source is APB1 (SYSCLK/4) */     
/* I2C TIMING is calculated in case of the I2C Clock source is the APB1CLK = 140 MHz */
/* This example use TIMING to 0xD0902D2F to reach 100 KHz speed */
#define EEPROM_TIMING              0xD0902D2F

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* I2C handler declaration */
I2C_HandleTypeDef I2cHandle;

/* Buffer used for transmission */
uint8_t aTxBuffer[] = "/* I2C HAL API EEPROM driver example: \
                        This firmware provides a basic example of how to use the I2C HAL API based on DMA and\
                        an associate I2C EEPROM driver to communicate with M24LR64 EEPROM \
                        I2C peripheral is configured in Master transmitter during write operation and in\
                        Master receiver during read operation from I2C EEPROM.*/";
                          
/* Buffer used for reception */
#if defined ( __ICCARM__ )
#pragma data_alignment = 32
#elif defined ( __CC_ARM )
__align(32)
#elif defined ( __GNUC__ )
__attribute__ ((aligned (32)))
#endif
uint8_t aRxBuffer[RXBUFFERSIZE];

/* Useful variables during communication */
uint16_t Memory_Address; 
int Remaining_Bytes;
__IO uint32_t I2C_Tx_Complete_Flag = 0;
__IO uint32_t I2C_Rx_Complete_Flag = 0;
__IO uint32_t I2C_Error_Flag = 0;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t Memory_Write_Trials = 0;
  uint32_t Memory_Read_Trials = 0;

  /* Configure the MPU attributes */
  MPU_Config();

  /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
   */
  HAL_Init();

  /* Configure the system clock to 280 MHz */
  SystemClock_Config();

  /* Configure LED1, LED3 and LED2 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED2);

  /*##-1- Configure the I2C peripheral ######################################*/
  I2cHandle.Instance             = I2Cx;
  I2cHandle.Init.Timing          = EEPROM_TIMING;
  I2cHandle.Init.OwnAddress1     = 0x00;
  I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2cHandle.Init.OwnAddress2     = 0x00;
  I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

  if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Enable the Analog I2C Filter */
  HAL_I2CEx_ConfigAnalogFilter(&I2cHandle,I2C_ANALOGFILTER_ENABLE);

  /* The board sends the message to EEPROM then reads it back */

  /*##-2- Start writing process ##############################################*/
  /* Initialize Remaining Bytes Value to TX Buffer Size */
  Remaining_Bytes = TXBUFFERSIZE;
  /* Initialize Memory address to 0 since EEPROM write will start from address 0 */
  Memory_Address = 0;
  /* Since page size is 128 bytes, the write procedure will be done in a loop */
  while(Remaining_Bytes > 0)
  {
    do
    {
    /* Write EEPROM_PAGESIZE */
      if (HAL_I2C_Mem_Write_DMA(&I2cHandle , (uint16_t)EEPROM_ADDRESS, Memory_Address, I2C_MEMADD_SIZE_16BIT, (uint8_t*)(aTxBuffer + Memory_Address), EEPROM_PAGESIZE)!= HAL_OK)
      {
      	if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
      	{
          /* Writing process Error */
          Error_Handler();
        }
      }

      /* Increment Trials */
      Memory_Write_Trials++;
    }
    while((Memory_Write_Trials < EEPROM_MAX_TRIALS) && (HAL_I2C_GetError(&I2cHandle) == HAL_I2C_ERROR_AF));
    
    /* Clear Trials */
    Memory_Write_Trials = 0;

    /* Wait for the end of the transfer */
    /*  Before starting a new communication transfer, you need to check the current
      state of the peripheral; if it’s busy you need to wait for the end of current
      transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the
      transfer, but application may perform other tasks while transfer operation
      is ongoing. */
    while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
    {
    } 

    /* Check if the EEPROM is ready for a new operation */  
    while (HAL_I2C_IsDeviceReady(&I2cHandle, EEPROM_ADDRESS, 10, 300) != HAL_OK);

    /* Wait for the end of the transfer */  
    while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
    {
    }

    /* Update Remaining bytes and Memory Address values */
    Remaining_Bytes -= EEPROM_PAGESIZE;
    Memory_Address += EEPROM_PAGESIZE;
  }

  /*##-3- Start reading process ##############################################*/
  do
  {
    if(HAL_I2C_Mem_Read_DMA(&I2cHandle , (uint16_t)EEPROM_ADDRESS, 0, I2C_MEMADD_SIZE_16BIT, (uint8_t*)aRxBuffer, RXBUFFERSIZE)!= HAL_OK)
    {
      if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
      {
        /* Reading process Error */
        Error_Handler();
      }
    }
  
    /* Increment Trials */
    Memory_Read_Trials++;
  }
  while((Memory_Read_Trials < EEPROM_MAX_TRIALS) && (HAL_I2C_GetError(&I2cHandle) == HAL_I2C_ERROR_AF));
  
    /* Clear Trials */
    Memory_Read_Trials = 0;

  /* Wait for the end of the transfer */
  while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
  {
  }

  /*##-4- Compare the sent and received buffers ##############################*/
  if(Buffercmp((uint8_t*)aTxBuffer, (uint8_t*)aRxBuffer, RXBUFFERSIZE))
  {
    /* Processing Error */
    Error_Handler();
  }
 
  /* Infinite loop */  
  while (1)
  {
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
    while(1) {};
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
    while(1) {};
  }
}

/**
  * @brief  Tx Transfer completed callback.
  * @param  I2cHandle: I2C handle
  * @note   This example shows a simple way to report end of DMA Tx transfer, and
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Turn LED1 on: Transfer in transmission process is correct */
  BSP_LED_On(LED1);
}

/**
  * @brief  Rx Transfer completed callback.
  * @param  I2cHandle: I2C handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Turn LED3 on: Transfer in reception process is correct */
  BSP_LED_On(LED3);
}

/**
  * @brief  I2C error callbacks.
  * @param  I2cHandle: I2C handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
 void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Turn LED2 on: Transfer error in reception/transmission process */
  BSP_LED_On(LED2);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
    /* Turn LED2 on */
    BSP_LED_On(LED2);
    while(1)
    {
    }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
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

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
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

