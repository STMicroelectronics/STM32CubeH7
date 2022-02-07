/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "cmsis_os.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_BSP
  * @brief Kernel BSP routines
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
#if configAPPLICATION_ALLOCATED_HEAP == 1
#if defined ( __GNUC__ ) || defined( __CC_ARM )
__attribute__((section(".heap_mem"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="heap_mem"
#endif
uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
#endif /* configAPPLICATION_ALLOCATED_HEAP */

/* Private defines -----------------------------------------------------------*/
#if defined ( USE_UART_TRACES )
#define USARTx                           USART3
#define USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE();

#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_8
#define USARTx_TX_GPIO_PORT              GPIOD
#define USARTx_TX_AF                     GPIO_AF7_USART3
#define USARTx_RX_PIN                    GPIO_PIN_9
#define USARTx_RX_GPIO_PORT              GPIOD
#define USARTx_RX_AF                     GPIO_AF7_USART3
#endif /* USE_UART_TRACES */

/* Private macros ------------------------------------------------------------*/
#ifdef __cplusplus
 extern "C" {
#endif

#if defined ( USE_UART_TRACES )
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
#endif /* USE_UART_TRACES */

extern TIM_HandleTypeDef TimHandle;

/* Private variables ---------------------------------------------------------*/
#if defined ( USE_UART_TRACES )
UART_HandleTypeDef UART_Handle;
#endif /* USE_UART_TRACES */

/* Private function prototypes -----------------------------------------------*/
#if defined ( USE_UART_TRACES )
static void BSP_ConfigSerial(void);
#endif /* USE_UART_TRACES */

/* Private functions ---------------------------------------------------------*/
#if defined ( USE_UART_TRACES )
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
#endif /* USE_UART_TRACES */

/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Configure the MPU attributes as Write Through for Internal D1SRAM.
  * @note   The Base Address is 0x24000000 since this memory interface is the AXI.
  *         The Configured Region Size is 512KB because same as D1SRAM size.
  * @param  None
  * @retval None
  */
void MPU_Config(void)
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

  /* Setup AXI SRAM, SRAM1 and SRAM2 in Cacheable WT */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = D1_AXISRAM_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Setup SDRAM in SO */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = OCTOSPI2_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_512MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER2;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Setup SDRAM in Cacheable WB (Buffers) */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = OCTOSPI2_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_8MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER3;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes for Quad-SPI area to strongly ordered
     This setting is essentially needed to avoid MCU blockings!
     See also STM Application Note AN4861 */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = OCTOSPI1_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_512MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER4;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes for the QSPI 64MB to Cacheable WT */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = OCTOSPI1_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_64MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER5;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  OSPI Clock Config for OSPI Memories .
  * @param  hospi  OSPI Handle
  *         Being __weak it can be overwritten by the application
  * @retval HAL_status
  */
HAL_StatusTypeDef MX_OSPI_ClockConfig(OSPI_HandleTypeDef *hospi)
{
  /* Use default source clock HCLK = 250 MHz*/
  /* OSPI clock will be 250 / 4 = 62.5MHz */
  return HAL_OK;
}

/**
  * @brief  Initializes the OSPI interface.
  * @param  hospi          OSPI handle
  * @param  Init           OSPI config structure
  * @retval BSP status
  */
HAL_StatusTypeDef MX_OSPI_RAM_Init(OSPI_HandleTypeDef *hospi, MX_OSPI_InitTypeDef *Init)
{
  OSPI_HyperbusCfgTypeDef sHyperbusCfg;
  HAL_StatusTypeDef status;

  hospi->Instance = OCTOSPI2;
  HAL_OSPI_DeInit(hospi);

  /*
   * With Init->ClockPrescaler = 2, the RAM will run at 125 MHz which is above the spec
   * We need to change the default configuration to avoid glitches
   * New Configuration for 62.5 MHz is :
   *   ClockPrescaler = 4
   *   Refresh = 250
   * This will be reconfigured later by the selected Application after succeeding the system reset
   */

  hospi->Init.FifoThreshold         = 4;
  hospi->Init.DualQuad              = HAL_OSPI_DUALQUAD_DISABLE;
  hospi->Init.MemoryType            = HAL_OSPI_MEMTYPE_HYPERBUS;
  hospi->Init.DeviceSize            = Init->MemorySize;
  hospi->Init.ChipSelectHighTime    = 4;
  hospi->Init.FreeRunningClock      = HAL_OSPI_FREERUNCLK_DISABLE;
  hospi->Init.ClockMode             = HAL_OSPI_CLOCK_MODE_0;
  hospi->Init.WrapSize              = HAL_OSPI_WRAP_NOT_SUPPORTED;
  hospi->Init.ClockPrescaler        = 4;
  hospi->Init.SampleShifting        = Init->SampleShifting;
  hospi->Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
  hospi->Init.Refresh               = 250; /*4us @62.5MHz*/
  hospi->Init.ChipSelectBoundary    = 23;  /* memory die boundary 2^23=8MBs*/

  status = HAL_OSPI_Init(hospi);
  if (status == HAL_OK)
  {
    sHyperbusCfg.RWRecoveryTime   = RW_RECOVERY_TIME;
    sHyperbusCfg.AccessTime       = DEFAULT_INITIAL_LATENCY;
    sHyperbusCfg.WriteZeroLatency = HAL_OSPI_LATENCY_ON_WRITE;
    sHyperbusCfg.LatencyMode      = HAL_OSPI_FIXED_LATENCY;

    status = HAL_OSPI_HyperbusCfg(hospi, &sHyperbusCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);
  }

  return status;
}
/**
  * @brief  Initializes the OSPI interface.
  * @param  hospi          OSPI handle
  * @param  Config         OSPI config structure
  * @retval BSP status
  */
HAL_StatusTypeDef MX_OSPI_NOR_Init(OSPI_HandleTypeDef *hospi, MX_OSPI_InitTypeDef *Config)
{
  /*
   * With Init->ClockPrescaler = 2, the OSPI-NOR will run at 125 MHz which is above the spec
   * We need to change the default configuration to avoid corruption.
   * New Configuration for 62.5 MHz is :
   *   ClockPrescaler = 4
   * This will be reconfigured later by the selected Application after succeeding the system reset
   */
  
  /* OctoSPI initialization */
  hospi->Instance = OCTOSPI1;

  hospi->Init.FifoThreshold      = 4;
  hospi->Init.DualQuad           = HAL_OSPI_DUALQUAD_DISABLE;
  hospi->Init.DeviceSize         = Config->MemorySize; /* 512 MBits */
  hospi->Init.ChipSelectHighTime = 2;
  hospi->Init.FreeRunningClock   = HAL_OSPI_FREERUNCLK_DISABLE;
  hospi->Init.ClockMode          = HAL_OSPI_CLOCK_MODE_0;
  hospi->Init.WrapSize           = HAL_OSPI_WRAP_NOT_SUPPORTED;
  hospi->Init.ClockPrescaler     = 4;
  hospi->Init.SampleShifting     = Config->SampleShifting;
  hospi->Init.ChipSelectBoundary = 0;

  if (Config->TransferRate == BSP_OSPI_NOR_DTR_TRANSFER)
  {
    hospi->Init.MemoryType            = HAL_OSPI_MEMTYPE_MACRONIX;
    hospi->Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
  }
  else
  {
    hospi->Init.MemoryType            = HAL_OSPI_MEMTYPE_MICRON;
    hospi->Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_DISABLE;
  }

  return HAL_OSPI_Init(hospi);
}

/**
  * @brief  Initializes LEDs, SDRAM, touch screen, CRC and OSPI_RAM.
  * @param  None
  * @retval RetVal
  */
uint8_t BspInit(const uint8_t minimal)
{
  int32_t RetVal = 0;
  BSP_OSPI_RAM_Init_t ospi_ram_init;
  BSP_OSPI_NOR_Init_t ospi_nor_init;

  if(minimal)
  {
    ospi_nor_init.InterfaceMode = BSP_OSPI_NOR_OPI_MODE;
    ospi_nor_init.TransferRate  = BSP_OSPI_NOR_DTR_TRANSFER;
    // BSP_OSPI_NOR_DeInit(0);
    if(BSP_OSPI_NOR_Init(0, &ospi_nor_init) == BSP_ERROR_NONE)
    {
      if(BSP_OSPI_NOR_EnableMemoryMappedMode(0) == BSP_ERROR_NONE)
      {
        /* Disable Systick interrupt */
        SysTick->CTRL = 0;
        HAL_SuspendTick();
        HAL_TIM_Base_DeInit(&TimHandle);

        /* Return OK */
        return 1;
      }
    }
    return 0;
  }

#if defined ( USE_UART_TRACES )
  BSP_ConfigSerial();

  printf( "CPU running at %dMHz, Peripherals at %dMHz/%dMHz\n", (HAL_RCCEx_GetD1SysClockFreq()/1000000U), (HAL_RCC_GetPCLK1Freq()/1000000U), (HAL_RCC_GetPCLK2Freq()/1000000U) );
#endif /* USE_UART_TRACES */

  ospi_nor_init.InterfaceMode = BSP_OSPI_NOR_OPI_MODE;
  ospi_nor_init.TransferRate  = BSP_OSPI_NOR_DTR_TRANSFER;
  BSP_OSPI_NOR_DeInit(0);
  RetVal = BSP_OSPI_NOR_Init(0, &ospi_nor_init);
  if(RetVal != BSP_ERROR_NONE)
  {
#if defined ( USE_UART_TRACES )
    printf("Failed to initialize the OSPI !! (Error %d)\n", RetVal);
#endif /* USE_UART_TRACES */
    return 0;
  }
  RetVal = BSP_OSPI_NOR_EnableMemoryMappedMode(0);
  if(RetVal != BSP_ERROR_NONE)
  {
#if defined ( USE_UART_TRACES )
    printf("Failed to configure MM Mode for the OSPI !! (Error %d)\n", RetVal);
#endif /* USE_UART_TRACES */
    return 0;
  }

  /* Initialize the OSPI Hyper RAM memory */
  ospi_ram_init.LatencyType = BSP_OSPI_RAM_FIXED_LATENCY;
  ospi_ram_init.BurstType   = BSP_OSPI_RAM_LINEAR_BURST;
  ospi_ram_init.BurstLength = BSP_OSPI_RAM_BURST_32_BYTES;
  BSP_OSPI_RAM_Init(0, &ospi_ram_init);
  RetVal = BSP_OSPI_RAM_EnableMemoryMappedMode(0);
  if ( RetVal != BSP_ERROR_NONE)
  {
#if defined ( USE_UART_TRACES )
    printf("Failed to initialize the OSPI HyperRAM !! (Error %d)\n", RetVal);
#endif /* USE_UART_TRACES */
    return 0;
  }

  if (BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI) != BSP_ERROR_NONE)
  {
    return 0;
  }

  return 1;
}

#if defined ( USE_UART_TRACES )
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

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();

  /* Select SysClk as source of USART3 clocks */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART1CLKSOURCE_D2PCLK2;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

  /* Enable USART3 clock */
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
#endif /* USE_UART_TRACES */

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
  configASSERT(0);
}
#endif

/**
  * @}
  */

/**
  * @}
  */

