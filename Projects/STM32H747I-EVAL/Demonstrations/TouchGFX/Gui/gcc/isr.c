
#include <cmsis_os.h>

#include "isr.h"

extern int main(void);
extern int SystemInit(void);

extern unsigned long _etext;
extern unsigned long __data_start;
extern unsigned long _edata;
extern unsigned long __bss_start__;
extern unsigned long __bss_end__;
extern unsigned long __stack_end__;
extern unsigned long __valid_user_code_checksum;

/*
int _sbrk_r (struct _reent *r, int incr)
{
    return 0;
}
*/

__attribute__ ((section(".isr_vectors"), used))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void))((unsigned long)&__stack_end__),
	Reset_Handler,
	NMI_Handler,
	HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0,
	0,
	0,
	0,
	SVC_Handler,
	DebugMon_Handler,
	0,
	PendSV_Handler,
	SysTick_Handler,
	WWDG_IRQHandler,
	PVD_IRQHandler,
	TAMP_STAMP_IRQHandler,
	RTC_WKUP_IRQHandler,
	FLASH_IRQHandler,
	RCC_IRQHandler,
	EXTI0_IRQHandler,
	EXTI1_IRQHandler,
	EXTI2_IRQHandler,
	EXTI3_IRQHandler,
	EXTI4_IRQHandler,
	DMA1_Stream0_IRQHandler,
	DMA1_Stream1_IRQHandler,
	DMA1_Stream2_IRQHandler,
	DMA1_Stream3_IRQHandler,
	DMA1_Stream4_IRQHandler,
	DMA1_Stream5_IRQHandler,
	DMA1_Stream6_IRQHandler,
	ADC_IRQHandler,
	TTCAN_IT0_IRQHandler,
	FDCAN_IT0_IRQHandler,
	TTCAN_IT1_IRQHandler,
	FDCAN_IT1_IRQHandler,
	EXTI9_5_IRQHandler,
	TIM1_BRK_TIM9_IRQHandler,
	TIM1_UP_TIM10_IRQHandler,
	TIM1_TRG_COM_TIM11_IRQHandler,
	TIM1_CC_IRQHandler,
	TIM2_IRQHandler,
	TIM3_IRQHandler,
	TIM4_IRQHandler,
	I2C1_EV_IRQHandler,
	I2C1_ER_IRQHandler,
	I2C2_EV_IRQHandler,
	I2C2_ER_IRQHandler,
	SPI1_IRQHandler,
	SPI2_IRQHandler,
	USART1_IRQHandler,
	USART2_IRQHandler,
	USART3_IRQHandler,
	EXTI15_10_IRQHandler,
	RTC_Alarm_IRQHandler,
	0,
	TIM8_BRK_TIM12_IRQHandler,
	TIM8_UP_TIM13_IRQHandler,
	TIM8_TRG_COM_TIM14_IRQHandler,
	TIM8_CC_IRQHandler,
	DMA1_Stream7_IRQHandler,
	FMC_IRQHandler,
	SDMMC1_IRQHandler,
	TIM5_IRQHandler,
	SPI3_IRQHandler,
	UART4_IRQHandler,
	UART5_IRQHandler,
	TIM6_DAC_IRQHandler,
	TIM7_IRQHandler,
	DMA2_Stream0_IRQHandler,
	DMA2_Stream1_IRQHandler,
	DMA2_Stream2_IRQHandler,
	DMA2_Stream3_IRQHandler,
	DMA2_Stream4_IRQHandler,
	ETH_IRQHandler,
	ETH_WKUP_IRQHandler,
	FDCAN_CU_IRQHandler,
	0,
	0,
	0,
	0,
	DMA2_Stream5_IRQHandler,
	DMA2_Stream6_IRQHandler,
	DMA2_Stream7_IRQHandler,
	USART6_IRQHandler,
	I2C3_EV_IRQHandler,
	I2C3_ER_IRQHandler,
	OTG_HS_EP1_OUT_IRQHandler,
	OTG_HS_EP1_IN_IRQHandler,
	OTG_HS_WKUP_IRQHandler,
	OTG_HS_IRQHandler,
	DCMI_IRQHandler,
	CRYP_IRQHandler,
	HASH_RNG_IRQHandler,
	FPU_IRQHandler,
	UART7_IRQHandler,
	UART8_IRQHandler,
	SPI4_IRQHandler,
	SPI5_IRQHandler,
	SPI6_IRQHandler,
	SAI1_IRQHandler,
	LTDC_IRQHandler,
	LTDC_ER_IRQHandler,
	DMA2D_IRQHandler,
	SAI2_IRQHandler,
	QUADSPI_IRQHandler,
	LPTIM1_IRQHandler,
	CEC_IRQHandler,
	I2C4_EV_IRQHandler,
	I2C4_ER_IRQHandler,
	SPDIF_RX_IRQHandler,
	OTG_FS_EP1_OUT_IRQHandler,
	OTG_FS_EP1_IN_IRQHandler,
	OTG_FS_WKUP_IRQHandler,
	OTG_FS_IRQHandler,
	DMAMUX1_OVR_IRQHandler,
	HRTIM1_Master_IRQHandler,
	HRTIM1_TIMA_IRQHandler,
	HRTIM1_TIMB_IRQHandler,
	HRTIM1_TIMC_IRQHandler,
	HRTIM1_TIMD_IRQHandler,
	HRTIM1_TIME_IRQHandler,
	HRTIM1_FLT_IRQHandler,
	DFSDM0_IRQHandler,
	DFSDM1_IRQHandler,
	DFSDM2_IRQHandler,
	DFSDM3_IRQHandler,
	SAI3_IRQHandler,
	SWPMI1_IRQHandler,
	TIM15_IRQHandler,
	TIM16_IRQHandler,
	TIM17_IRQHandler,
	MDIOS_WKUP_IRQHandler,
	MDIOS_IRQHandler,
	JPEG_IRQHandler,
	MDMA_IRQHandler,
	DSI_IRQHandler,
	SDMMC2_IRQHandler,
	HSEM1_IRQHandler,
	HSEM2_IRQHandler,
	ADC3_IRQHandler,
	DMAMUX2_OVR_IRQHandler,
	BDMA_Channel0_IRQHandler,
	BDMA_Channel1_IRQHandler,
	BDMA_Channel2_IRQHandler,
	BDMA_Channel3_IRQHandler,
	BDMA_Channel4_IRQHandler,
	BDMA_Channel5_IRQHandler,
	BDMA_Channel6_IRQHandler,
	BDMA_Channel7_IRQHandler,
	COMP1_IRQHandler,
	LPTIM2_IRQHandler,
	LPTIM3_IRQHandler,
	LPTIM4_IRQHandler,
	LPTIM5_IRQHandler,
	LPUART1_IRQHandler,
	WWDG_RST_IRQHandler,
	CRS_IRQHandler,
	ECC_IRQHandler,
	SAI4_IRQHandler,
	TEMP_IRQHandler,
	HOLD_CORE_IRQHandler,
	WAKEUP_PIN_IRQHandler
};

extern void (*__init_array_start []) (void) __attribute__((weak));
extern void (*__init_array_end []) (void) __attribute__((weak));

void Reset_Handler(void)
{
    unsigned long *src, *dst;
    
    int count;
    int i;

    
    src = &_etext;
    for(dst = &__data_start; dst < &_edata; ) {
        *dst++ = *src++;
    }

    for(dst = &__bss_start__; dst < &__bss_end__; ) {
        *dst++ = 0;
    }
    
    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++)
        __init_array_start[i] ();
    
    SystemInit();
    main();
}

// void NMI_Handler(void) 
// {
  // while(1);
// }

// void HardFault_Handler(void) 
// {
  // while(1);
// }

// void MemManage_Handler(void) 
// {
  // while(1);
// }

// void BusFault_Handler(void) 
// {
  // while(1);
// }

// void UsageFault_Handler(void) 
// {
  // while(1);
// }

// void DebugMon_Handler(void) 
// {
  // while(1);
// }

// void SysTick_Handler(void)
// {
  // osSystickHandler();
// }

void WWDG_IRQHandler(void)
{
}

void PVD_IRQHandler(void)
{
}

void TAMP_STAMP_IRQHandler(void)
{
}

void RTC_WKUP_IRQHandler(void)
{
}

void FLASH_IRQHandler(void)
{
}

void RCC_IRQHandler(void)
{
}

// void EXTI0_IRQHandler(void)
// {
// }

void EXTI1_IRQHandler(void)
{
}

void EXTI2_IRQHandler(void)
{
}

void EXTI3_IRQHandler(void)
{
}

void EXTI4_IRQHandler(void)
{
}

void DMA1_Stream0_IRQHandler(void)
{
}

void DMA1_Stream1_IRQHandler(void)
{
}

void DMA1_Stream2_IRQHandler(void)
{
}

void DMA1_Stream3_IRQHandler(void)
{
}

void DMA1_Stream4_IRQHandler(void)
{
}

void DMA1_Stream5_IRQHandler(void)
{
}

void DMA1_Stream6_IRQHandler(void)
{
}

void ADC_IRQHandler(void)
{
}

void TTCAN_IT0_IRQHandler(void)
{
}

void FDCAN_IT0_IRQHandler(void)
{
}

void TTCAN_IT1_IRQHandler(void)
{
}

void FDCAN_IT1_IRQHandler(void)
{
}

void EXTI9_5_IRQHandler(void)
{
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
}

void TIM1_UP_TIM10_IRQHandler(void)
{
}

void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
}

void TIM1_CC_IRQHandler(void)
{
}

void TIM2_IRQHandler(void)
{
}

void TIM3_IRQHandler(void)
{
}

void TIM4_IRQHandler(void)
{
}

void I2C1_EV_IRQHandler(void)
{
}

void I2C1_ER_IRQHandler(void)
{
}

void I2C2_EV_IRQHandler(void)
{
}

void I2C2_ER_IRQHandler(void)
{
}

void SPI1_IRQHandler(void)
{
}

void SPI2_IRQHandler(void)
{
}

void USART1_IRQHandler(void)
{
}

void USART2_IRQHandler(void)
{
}

void USART3_IRQHandler(void)
{
}

void EXTI15_10_IRQHandler(void)
{
}

void RTC_Alarm_IRQHandler(void)
{
}

void TIM8_BRK_TIM12_IRQHandler(void)
{
}

void TIM8_UP_TIM13_IRQHandler(void)
{
}

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
}

void TIM8_CC_IRQHandler(void)
{
}

void DMA1_Stream7_IRQHandler(void)
{
}

void FMC_IRQHandler(void)
{
}

void SDMMC1_IRQHandler(void)
{
}

void TIM5_IRQHandler(void)
{
}

void SPI3_IRQHandler(void)
{
}

void UART4_IRQHandler(void)
{
}

void UART5_IRQHandler(void)
{
}

// void TIM6_DAC_IRQHandler(void)
// {
// }

void TIM7_IRQHandler(void)
{
}

void DMA2_Stream0_IRQHandler(void)
{
}

// void DMA2_Stream1_IRQHandler(void)
// {
// }

void DMA2_Stream2_IRQHandler(void)
{
}

void DMA2_Stream3_IRQHandler(void)
{
}

void DMA2_Stream4_IRQHandler(void)
{
}

void ETH_IRQHandler(void)
{
}

void ETH_WKUP_IRQHandler(void)
{
}

void FDCAN_CU_IRQHandler(void)
{
}

void DMA2_Stream5_IRQHandler(void)
{
}

void DMA2_Stream6_IRQHandler(void)
{
}

void DMA2_Stream7_IRQHandler(void)
{
}

void USART6_IRQHandler(void)
{
}

void I2C3_EV_IRQHandler(void)
{
}

void I2C3_ER_IRQHandler(void)
{
}

void OTG_HS_EP1_OUT_IRQHandler(void)
{
}

void OTG_HS_EP1_IN_IRQHandler(void)
{
}

void OTG_HS_WKUP_IRQHandler(void)
{
}

// void OTG_HS_IRQHandler(void)
// {
// }

void DCMI_IRQHandler(void)
{
}

void CRYP_IRQHandler(void)
{
}

void HASH_RNG_IRQHandler(void)
{
}

void FPU_IRQHandler(void)
{
}

void UART7_IRQHandler(void)
{
}

void UART8_IRQHandler(void)
{
}

void SPI4_IRQHandler(void)
{
}

void SPI5_IRQHandler(void)
{
}

void SPI6_IRQHandler(void)
{
}

void SAI1_IRQHandler(void)
{
}

void LTDC_IRQHandler(void)
{
}

// void LTDC_ER_IRQHandler(void)
// {
// }

// void DMA2D_IRQHandler(void)
// {
// }

void SAI2_IRQHandler(void)
{
}

void QUADSPI_IRQHandler(void)
{
}

void LPTIM1_IRQHandler(void)
{
}

void CEC_IRQHandler(void)
{
}

void I2C4_EV_IRQHandler(void)
{
}

void I2C4_ER_IRQHandler(void)
{
}

void SPDIF_RX_IRQHandler(void)
{
}

void OTG_FS_EP1_OUT_IRQHandler(void)
{
}

void OTG_FS_EP1_IN_IRQHandler(void)
{
}

void OTG_FS_WKUP_IRQHandler(void)
{
}

void OTG_FS_IRQHandler(void)
{
}

void DMAMUX1_OVR_IRQHandler(void)
{
}

void HRTIM1_Master_IRQHandler(void)
{
}

void HRTIM1_TIMA_IRQHandler(void)
{
}

void HRTIM1_TIMB_IRQHandler(void)
{
}

void HRTIM1_TIMC_IRQHandler(void)
{
}

void HRTIM1_TIMD_IRQHandler(void)
{
}

void HRTIM1_TIME_IRQHandler(void)
{
}

void HRTIM1_FLT_IRQHandler(void)
{
}

void DFSDM0_IRQHandler(void)
{
}

void DFSDM1_IRQHandler(void)
{
}

void DFSDM2_IRQHandler(void)
{
}

void DFSDM3_IRQHandler(void)
{
}

void SAI3_IRQHandler(void)
{
}

void SWPMI1_IRQHandler(void)
{
}

void TIM15_IRQHandler(void)
{
}

void TIM16_IRQHandler(void)
{
}

void TIM17_IRQHandler(void)
{
}

void MDIOS_WKUP_IRQHandler(void)
{
}

void MDIOS_IRQHandler(void)
{
}

// void JPEG_IRQHandler(void)
// {
// }

// void MDMA_IRQHandler(void)
// {
// }

// void DSI_IRQHandler(void)
// {
// }

void SDMMC2_IRQHandler(void)
{
}

void HSEM1_IRQHandler(void)
{
}

void HSEM2_IRQHandler(void)
{
}

void ADC3_IRQHandler(void)
{
}

void DMAMUX2_OVR_IRQHandler(void)
{
}

void BDMA_Channel0_IRQHandler(void)
{
}

void BDMA_Channel1_IRQHandler(void)
{
}

void BDMA_Channel2_IRQHandler(void)
{
}

void BDMA_Channel3_IRQHandler(void)
{
}

void BDMA_Channel4_IRQHandler(void)
{
}

void BDMA_Channel5_IRQHandler(void)
{
}

void BDMA_Channel6_IRQHandler(void)
{
}

void BDMA_Channel7_IRQHandler(void)
{
}

void COMP1_IRQHandler(void)
{
}

void LPTIM2_IRQHandler(void)
{
}

void LPTIM3_IRQHandler(void)
{
}

void LPTIM4_IRQHandler(void)
{
}

void LPTIM5_IRQHandler(void)
{
}

void LPUART1_IRQHandler(void)
{
}

void WWDG_RST_IRQHandler(void)
{
}

void CRS_IRQHandler(void)
{
}

void ECC_IRQHandler(void)
{
}

void SAI4_IRQHandler(void)
{
}

void TEMP_IRQHandler(void)
{
}

void HOLD_CORE_IRQHandler(void)
{
}

void WAKEUP_PIN_IRQHandler(void)
{
}
