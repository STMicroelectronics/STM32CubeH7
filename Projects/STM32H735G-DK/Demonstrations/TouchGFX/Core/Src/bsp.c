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
#if defined(CONFIG_ROMLESS)
typedef void( *intfunc )( void );
typedef union { intfunc __fun; void * __ptr; } intvec_elem;
#endif /* CONFIG_ROMLESS */

typedef struct Bitmap_Res_s
{
  uint16_t       x;
  uint16_t       y;
} Bitmap_Res_t;

typedef struct Demo_Header_s
{
  uint32_t              demo_id;
  const uint32_t * const icon_addr;
  Bitmap_Res_t          icon_res;
  const uint32_t * const logo_addr;
  Bitmap_Res_t          logo_res;
  uint32_t              sw_flags;
  uint32_t              reserved[2];
  unsigned char         demo_descr[32];
} Demo_Header_t;

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

#if defined(CONFIG_ROMLESS)
extern const intvec_elem __vector_table[];
extern void __iar_init_core(void);
extern void __iar_init_vfp(void);
extern void __cmain(void);
#endif /* CONFIG_ROMLESS */

/* Private defines -----------------------------------------------------------*/
#define USARTx                           USART1
#define USARTx_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();

#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART1
#define USARTx_RX_PIN                    GPIO_PIN_10
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF7_USART1

#define BOARD_NAME                      "STM32H750B-DK_MB1381"
/* TouchGFX Demonstration ID : 1 */
#define DEMO_CODE_REVISION              0xC2010000 /* 0xAAXXYYZZ - 0xCx : Demo Code ID | 0xXXYYZZ : MajorMinorPatch Version number */
#define DEMO_DATA_REVISION              0xD2010000 /* 0xAAXXYYZZ - 0xDx : Demo Data ID | 0xXXYYZZ : MajorMinorPatch Version number */
#define DEMO_DESCRITION                 "TouchGFX Out of Box Demo"

#define SW_FLAG_AUTO_DEMO               (1L << 0)

/* Private macros ------------------------------------------------------------*/
#ifdef __cplusplus
 extern "C" {
#endif

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

#define GET_DEMO_VERSION_STR(tag)  "V%lu.%lu.%lu"
#define GET_DEMO_VERSION_P(tag)    ((tag  &0x00FF0000) >> 16), ((tag  &0x0000FF00) >> 8), (tag  &0x000000FF)

/* Private variables ---------------------------------------------------------*/
extern const uint32_t demo_icon __attribute__ ((section ("demo_icon_section")));
extern const uint32_t demo_logo __attribute__ ((section ("demo_logo_section")));

#if defined ( __ICCARM__ )
#pragma location="IntThirdPartyHeaderSection"
__root
#else
__attribute__((section(".IntThirdPartyHeaderSection"))) __attribute__ ((__used__))
#endif
static const Demo_Header_t IntThirdPartyHeader = {
  .demo_id = DEMO_CODE_REVISION,
  .icon_addr  = 0,
  .icon_res   = { 0, 0 },
  .logo_addr  = 0,
  .logo_res   = { 0, 0 },
  .sw_flags   = SW_FLAG_AUTO_DEMO,
  .demo_descr = DEMO_DESCRITION
};

#if defined ( __ICCARM__ )
#pragma location="ExtThirdPartyHeaderSection"
__root
#else
__attribute__((section(".ExtThirdPartyHeaderSection"))) __attribute__ ((__used__))
#endif
static const Demo_Header_t ExtThirdPartyHeader = {
  .demo_id = DEMO_DATA_REVISION,
  .icon_addr  = &demo_icon,
  .icon_res   = { 102, 102 },
  .logo_addr  = &demo_logo,
  .logo_res   = { 52, 52 },
  .sw_flags   = SW_FLAG_AUTO_DEMO,
  .demo_descr = DEMO_DESCRITION
};

uint8_t detected = 0;
uint8_t Touchdetected = 0;
static UART_HandleTypeDef UART_Handle;
static uint32_t AutoDemoId = 0;
#if defined(USE_STM32H7B3I_DK_OOB)
static uint32_t AutoDemoTimeOutMs = AUTO_DEMO_TIMEOUT_0;
#endif /* USE_STM32H7B3I_DK_OOB */

/* Private function prototypes -----------------------------------------------*/
static void BSP_ConfigSerial(void);

/* Private functions ---------------------------------------------------------*/
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

/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Configure the MPU attributes as Write Through for Internal D1SRAM.
  * @note   The Base Address is 0x24000000 since this memory interface is the AXI.
  *         The Configured Region Size is 512KB because same as D1SRAM size.
  * @param  None
  * @retval None
  */
#if defined ( __ICCARM__ )
#pragma location=".loader"
#else
__attribute__((section(".loader")))
#endif
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

  /* Setup AXI SRAM, SRAM1 and SRAM2 in Write-back */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = D1_AXISRAM_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
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
  MPU_InitStruct.Size             = MPU_REGION_SIZE_256MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER2;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Setup SDRAM in Write-back (Buffers) */
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

#if defined (CONFIG_ROMLESS)
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
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);
#endif /* CONFIG_ROMLESS */

  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
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
  * @brief  Initializes LEDs, SDRAM, touch screen, CRC and OSPI_RAM.
  * @param  None
  * @retval RetVal
  */
uint8_t BspInit(void)
{
  int32_t RetVal = 0;
  BSP_OSPI_RAM_Init_t ospi_ram_init;

  BSP_ConfigSerial();

  printf( "\n\n%s: %s " GET_DEMO_VERSION_STR(IntThirdPartyHeader.demo_id) " (Build %s at %s)\n", BOARD_NAME, IntThirdPartyHeader.demo_descr, GET_DEMO_VERSION_P(IntThirdPartyHeader.demo_id), __DATE__, __TIME__ );
  printf( "CPU running at %ldMHz, Peripherals at %ldMHz/%ldMHz\n", (HAL_RCCEx_GetD1SysClockFreq()/1000000U), (HAL_RCC_GetPCLK1Freq()/1000000U), (HAL_RCC_GetPCLK2Freq()/1000000U) );

#if defined(USE_STM32H735G_DK_OOB)
  HAL_PWR_EnableBkUpAccess();

  /* Read SW Configuration */
  BckRegValue       = READ_REG(BKP_REG_SW_CFG);
  AutoDemoId        = (((BckRegValue & 0x0000FF00) >>  8) & 0xFF);
  AutoDemoTimeOutMs = (((BckRegValue & 0xFFFF0000) >> 16) & 0xFFFF);
  if(AutoDemoTimeOutMs == 0)
  {
    AutoDemoTimeOutMs = AUTO_DEMO_TIMEOUT_0;
  }

  HAL_PWR_DisableBkUpAccess();
#endif /* USE_STM32H735G_DK_OOB */

  /* Initialize the OSPI Hyper RAM memory */
  ospi_ram_init.LatencyType = BSP_OSPI_RAM_FIXED_LATENCY;
  ospi_ram_init.BurstType   = BSP_OSPI_RAM_LINEAR_BURST;
  ospi_ram_init.BurstLength = BSP_OSPI_RAM_BURST_32_BYTES;
  BSP_OSPI_RAM_Init(0, &ospi_ram_init);
  RetVal = BSP_OSPI_RAM_EnableMemoryMappedMode(0);
  if ( RetVal != BSP_ERROR_NONE)
  {
    printf("Failed to initialize the OSPI HyperRAM !! (Error %ld)\n", RetVal);
    return 0;
  }

  if (BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI) != BSP_ERROR_NONE)
  {
    return 0;
  }

  /* Check if we're in AutoDemo Mode */
  if ( AutoDemoId )
  {
    /* Make Auto Demo Timer shorter */
  }

  return 1;
}


#if defined(CONFIG_ROMLESS)
#if defined ( __ICCARM__ )
#pragma location=".loader"
#else
__attribute__((section(".loader")))
#endif
__STATIC_INLINE int __system_init( void )
{
  int32_t ret;
  BSP_OSPI_NOR_Init_t ospi_nor_init;

  /* Initialize HAL : HAL_InitTick required for OSPI BSP !! */
  HAL_Init();

  /* Initialize the NOR OctoSPI flash */
  // BSP_OSPI_NOR_DeInit(0);
  ospi_nor_init.InterfaceMode = BSP_OSPI_NOR_OPI_MODE;
  ospi_nor_init.TransferRate  = BSP_OSPI_NOR_DTR_TRANSFER;
  ret = BSP_OSPI_NOR_Init(0, &ospi_nor_init);
  if(ret == BSP_ERROR_NONE)
  {
    ret = BSP_OSPI_NOR_EnableMemoryMappedMode(0);
  }

  return ret;
}

#if defined ( __ICCARM__ )
#pragma required=__vector_table
#pragma location=".loader"
#else
__attribute__((section(".loader")))
#endif
void __my_program_start( void )
{
#if defined ( __ICCARM__ )
  __iar_init_core();
  __iar_init_vfp();
  __system_init();
  __cmain();
#else
#error Please review this for both MDK-ARM and GCC !!
#endif
}
#endif /* CONFIG_ROMLESS */

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();

  /* Select SysClk as source of USART1 clocks */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART16;
  RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

  /* Enable USART1 clock */
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

