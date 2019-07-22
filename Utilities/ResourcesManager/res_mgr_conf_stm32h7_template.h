/**
  ******************************************************************************
  * @file    res_mgr_conf_stm32h7_template.h
  * @author  MCD Application Team
  * @brief   Resources Manager configuration template file.
  *          This file should be copied to the application folder and modified
  *          as follows:
  *            - Rename it to 'res_mgr_conf.h'.
  *            - Update it according your needs.
  ******************************************************************************
  *
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the License; You may not use this fileexcept in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  RES_MGR_CONF_H__
#define  RES_MGR_CONF_H__

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stddef.h>
/*#include "stm32xxxx_hal.h" */

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup Common
  * @{
  */

/** @addtogroup RES_MGR_TABLE
  * @{
  */

/** @defgroup RES_MGR_TABLE
  * @brief
  * @{
  */


/** @defgroup RES_MGR_Exported_Defines
  * @{
  */

/* This Part may be filled/customized by CubeMX/User */
/******** Shared Resource IDs ********************************************************/
enum
{
  RESMGR_ID_ADC12_COMMON                    ,
  RESMGR_ID_ADC1                            ,
  RESMGR_ID_ADC2                            ,
  RESMGR_ID_ADC3                            ,
  RESMGR_ID_BDMA                            ,
  RESMGR_ID_BDMA_Channel0                   ,
  RESMGR_ID_BDMA_Channel1                   ,
  RESMGR_ID_BDMA_Channel2                   ,
  RESMGR_ID_BDMA_Channel3                   ,
  RESMGR_ID_BDMA_Channel4                   ,
  RESMGR_ID_BDMA_Channel5                   ,
  RESMGR_ID_BDMA_Channel6                   ,
  RESMGR_ID_BDMA_Channel7                   ,
  RESMGR_ID_CEC                             ,
  RESMGR_ID_COMP1                           ,
  RESMGR_ID_COMP2                           ,
  RESMGR_ID_CRC                             ,
  RESMGR_ID_CRS                             ,
  RESMGR_ID_DAC1                            ,
  RESMGR_ID_DBGMCU                          ,
  RESMGR_ID_DCMI                            ,
  RESMGR_ID_DFSDM1                          ,
  RESMGR_ID_DFSDM1_Channel0                 ,
  RESMGR_ID_DFSDM1_Channel1                 ,
  RESMGR_ID_DFSDM1_Channel2                 ,
  RESMGR_ID_DFSDM1_Channel3                 ,
  RESMGR_ID_DFSDM1_Channel4                 ,
  RESMGR_ID_DFSDM1_Channel5                 ,
  RESMGR_ID_DFSDM1_Channel6                 ,
  RESMGR_ID_DFSDM1_Channel7                 ,
  RESMGR_ID_DFSDM1_Filter0                  ,
  RESMGR_ID_DFSDM1_Filter1                  ,
  RESMGR_ID_DFSDM1_Filter2                  ,
  RESMGR_ID_DFSDM1_Filter3                  ,
  RESMGR_ID_DLYB_QUADSPI                    ,
  RESMGR_ID_DLYB_SDMMC1                     ,
  RESMGR_ID_DLYB_SDMMC2                     ,
  RESMGR_ID_DMA1                            ,
  RESMGR_ID_DMA1_Stream0                    ,
  RESMGR_ID_DMA1_Stream1                    ,
  RESMGR_ID_DMA1_Stream2                    ,
  RESMGR_ID_DMA1_Stream3                    ,
  RESMGR_ID_DMA1_Stream4                    ,
  RESMGR_ID_DMA1_Stream5                    ,
  RESMGR_ID_DMA1_Stream6                    ,
  RESMGR_ID_DMA1_Stream7                    ,
  RESMGR_ID_DMA2                            ,
  RESMGR_ID_DMA2_Stream0                    ,
  RESMGR_ID_DMA2_Stream1                    ,
  RESMGR_ID_DMA2_Stream2                    ,
  RESMGR_ID_DMA2_Stream3                    ,
  RESMGR_ID_DMA2_Stream4                    ,
  RESMGR_ID_DMA2_Stream5                    ,
  RESMGR_ID_DMA2_Stream6                    ,
  RESMGR_ID_DMA2_Stream7                    ,
  RESMGR_ID_DMA2D
  RESMGR_ID_DMAMUX1                         ,
  RESMGR_ID_DMAMUX1_Channel0                ,
  RESMGR_ID_DMAMUX1_Channel1                ,
  RESMGR_ID_DMAMUX1_Channel2                ,
  RESMGR_ID_DMAMUX1_Channel3                ,
  RESMGR_ID_DMAMUX1_Channel4                ,
  RESMGR_ID_DMAMUX1_Channel5                ,
  RESMGR_ID_DMAMUX1_Channel6                ,
  RESMGR_ID_DMAMUX1_Channel7                ,
  RESMGR_ID_DMAMUX1_Channel8                ,
  RESMGR_ID_DMAMUX1_Channel9                ,
  RESMGR_ID_DMAMUX1_Channel10               ,
  RESMGR_ID_DMAMUX1_Channel11               ,
  RESMGR_ID_DMAMUX1_Channel12               ,
  RESMGR_ID_DMAMUX1_Channel13               ,
  RESMGR_ID_DMAMUX1_Channel14               ,
  RESMGR_ID_DMAMUX1_Channel15               ,
  RESMGR_ID_DMAMUX1_RequestGenerator0       ,
  RESMGR_ID_DMAMUX1_RequestGenerator1       ,
  RESMGR_ID_DMAMUX1_RequestGenerator2       ,
  RESMGR_ID_DMAMUX1_RequestGenerator3       ,
  RESMGR_ID_DMAMUX1_RequestGenerator4       ,
  RESMGR_ID_DMAMUX1_RequestGenerator5       ,
  RESMGR_ID_DMAMUX1_RequestGenerator6       ,
  RESMGR_ID_DMAMUX1_RequestGenerator7       ,
  RESMGR_ID_DMAMUX2                         ,
  RESMGR_ID_DMAMUX2_Channel0                ,
  RESMGR_ID_DMAMUX2_Channel1                ,
  RESMGR_ID_DMAMUX2_Channel2                ,
  RESMGR_ID_DMAMUX2_Channel3                ,
  RESMGR_ID_DMAMUX2_Channel4                ,
  RESMGR_ID_DMAMUX2_Channel5                ,
  RESMGR_ID_DMAMUX2_Channel6                ,
  RESMGR_ID_DMAMUX2_Channel7                ,
  RESMGR_ID_DMAMUX2_Channel8                ,
  RESMGR_ID_DMAMUX2_Channel9                ,
  RESMGR_ID_DMAMUX2_Channel10               ,
  RESMGR_ID_DMAMUX2_Channel11               ,
  RESMGR_ID_DMAMUX2_Channel12               ,
  RESMGR_ID_DMAMUX2_Channel13               ,
  RESMGR_ID_DMAMUX2_Channel14               ,
  RESMGR_ID_DMAMUX2_Channel15               ,
  RESMGR_ID_DMAMUX2_RequestGenerator0       ,
  RESMGR_ID_DMAMUX2_RequestGenerator1       ,
  RESMGR_ID_DMAMUX2_RequestGenerator2       ,
  RESMGR_ID_DMAMUX2_RequestGenerator3       ,
  RESMGR_ID_DMAMUX2_RequestGenerator4       ,
  RESMGR_ID_DMAMUX2_RequestGenerator5       ,
  RESMGR_ID_DMAMUX2_RequestGenerator6       ,
  RESMGR_ID_DMAMUX2_RequestGenerator7       ,
  RESMGR_ID_DSI                             ,
  RESMGR_ID_ETH                             ,
  RESMGR_ID_EXTI                            ,
  RESMGR_ID_EXTI_D1                         ,
  RESMGR_ID_EXTI_D2                         ,
  RESMGR_ID_FDCAN1                          ,
  RESMGR_ID_FDCAN2                          ,
  RESMGR_ID_FLASH                           ,
  RESMGR_ID_FMC                             ,
  RESMGR_ID_GPIOA                           ,
  RESMGR_ID_GPIOB                           ,
  RESMGR_ID_GPIOC                           ,
  RESMGR_ID_GPIOD                           ,
  RESMGR_ID_GPIOE                           ,
  RESMGR_ID_GPIOF                           ,
  RESMGR_ID_GPIOG                           ,
  RESMGR_ID_GPIOH                           ,
  RESMGR_ID_GPIOI                           ,
  RESMGR_ID_GPIOJ                           ,
  RESMGR_ID_GPIOK                           ,
  RESMGR_ID_HRTIM1                          ,
  RESMGR_ID_HSEM                            ,
  RESMGR_ID_I2C1                            ,
  RESMGR_ID_I2C2                            ,
  RESMGR_ID_I2C3                            ,
  RESMGR_ID_I2C4                            ,
  RESMGR_ID_IWDG1                           ,
  RESMGR_ID_IWDG2                           ,
  RESMGR_ID_JPEG                            ,
  RESMGR_ID_LPTIM1                          ,
  RESMGR_ID_LPTIM2                          ,
  RESMGR_ID_LPTIM3                          ,
  RESMGR_ID_LPTIM4                          ,
  RESMGR_ID_LPTIM5                          ,
  RESMGR_ID_LPUART1                         ,
  RESMGR_ID_LTDC                            ,
  RESMGR_ID_MDIOS                           ,
  RESMGR_ID_MDMA                            ,
  RESMGR_ID_MDMA_Channel0                   ,
  RESMGR_ID_MDMA_Channel1                   ,
  RESMGR_ID_MDMA_Channel2                   ,
  RESMGR_ID_MDMA_Channel3                   ,
  RESMGR_ID_MDMA_Channel4                   ,
  RESMGR_ID_MDMA_Channel5                   ,
  RESMGR_ID_MDMA_Channel6                   ,
  RESMGR_ID_MDMA_Channel7                   ,
  RESMGR_ID_MDMA_Channel8                   ,
  RESMGR_ID_MDMA_Channel9                   ,
  RESMGR_ID_MDMA_Channel10                  ,
  RESMGR_ID_MDMA_Channel11                  ,
  RESMGR_ID_MDMA_Channel12                  ,
  RESMGR_ID_MDMA_Channel13                  ,
  RESMGR_ID_MDMA_Channel14                  ,
  RESMGR_ID_MDMA_Channel15                  ,
  RESMGR_ID_OPAMP                           ,
  RESMGR_ID_OPAMP1 = RESMGR_ID_OPAMP        ,
  RESMGR_ID_OPAMP2                          ,
  RESMGR_ID_PWR                             ,
  RESMGR_ID_QUADSPI                         ,
  RESMGR_ID_RCC                             ,
  RESMGR_ID_RCC_C1                          ,
  RESMGR_ID_RCC_C2                          ,
  RESMGR_ID_RNG                             ,
  RESMGR_ID_RTC                             ,
  RESMGR_ID_SAI1                            ,
  RESMGR_ID_SAI2                            ,
  RESMGR_ID_SAI3                            ,
  RESMGR_ID_SAI4                            ,
  RESMGR_ID_SDMMC1                          ,
  RESMGR_ID_SDMMC2                          ,
  RESMGR_ID_SPDIFRX                         ,
  RESMGR_ID_SPI1                            ,
  RESMGR_ID_SPI2                            ,
  RESMGR_ID_SPI3                            ,
  RESMGR_ID_SPI4                            ,
  RESMGR_ID_SPI5                            ,
  RESMGR_ID_SPI6                            ,
  RESMGR_ID_SWPMI1                          ,
  RESMGR_ID_SYSCFG                          ,
  RESMGR_ID_TIM1                            ,
  RESMGR_ID_TIM12                           ,
  RESMGR_ID_TIM13                           ,
  RESMGR_ID_TIM14                           ,
  RESMGR_ID_TIM15                           ,
  RESMGR_ID_TIM16                           ,
  RESMGR_ID_TIM17                           ,
  RESMGR_ID_TIM2                            ,
  RESMGR_ID_TIM3                            ,
  RESMGR_ID_TIM4                            ,
  RESMGR_ID_TIM5                            ,
  RESMGR_ID_TIM6                            ,
  RESMGR_ID_TIM7                            ,
  RESMGR_ID_TIM8                            ,
  RESMGR_ID_TMPSENS                         ,
  RESMGR_ID_UART4                           ,
  RESMGR_ID_UART5                           ,
  RESMGR_ID_UART7                           ,
  RESMGR_ID_UART8                           ,
  RESMGR_ID_USART1                          ,
  RESMGR_ID_USART2                          ,
  RESMGR_ID_USART3                          ,
  RESMGR_ID_USART6                          ,
  RESMGR_ID_USB1_OTG_HS                     ,
  RESMGR_ID_USB2_OTG_FS                     ,
  RESMGR_ID_VREFBUF                         ,
  RESMGR_ID_WWDG1                           ,
  RESMGR_ID_WWDG2                           ,
  RESMGR_ID_RESMGR_TABLE                    ,
};

#define   RESMGR_ENTRY_NBR  ((uint32_t)RESMGR_ID_RESMGR_TABLE + 1UL)
#define   RESMGR_ID_ALL                    0x0000FFFFU
#define   RESMGR_ID_NONE                   0xFFFFFFFFU

/**
  * @}
  */


/** @defgroup RES_MGR_Default_ResTable
  * @{
  */
#define RESMGR_USE_DEFAULT_TBL

#define RES_DEFAULT_ASSIGN_NONE   0
#define RES_DEFAULT_ASSIGN_CPU1   1
#define RES_DEFAULT_ASSIGN_CPU2   2

static const uint8_t Default_ResTbl[RESMGR_ENTRY_NBR] = {
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_ADC1                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_ADC2                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_ADC3                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_BDMA                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_CEC                        */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_COMP1                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_COMP2                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_CRC                        */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_CRS                        */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_DAC1                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_DBGMCU                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_DCMI                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_DFSDM1                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_DLYB_QUADSPI               */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_DLYB_SDMMC1                */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_DLYB_SDMMC2                */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_DMA1                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_DMA2                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_DMA2D                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_DMAMUX1                    */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_DMAMUX2                    */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_DSI                        */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_ETH                        */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_EXTI                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_EXTI_D1                    */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_EXTI_D2                    */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_FDCAN_CCU                  */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_FDCAN1                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_FDCAN2                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_FLASH                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_FMC                        */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_GPIOA                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_GPIOB                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_GPIOC                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_GPIOD                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_GPIOE                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_GPIOF                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_GPIOG                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_GPIOH                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_GPIOI                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_GPIOJ                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_GPIOK                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_HRTIM1                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_HSEM                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_I2C1                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_I2C2                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_I2C3                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_I2C4                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_IWDG1                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_IWDG2                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_JPEG                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_LPTIM1                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_LPTIM2                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_LPTIM3                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_LPTIM4                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_LPTIM5                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_LPUART1                    */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_LTDC                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_MDIOS                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_MDMA                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_OPAMP                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_OPAMP1 = RESMGR_ID_OPAMP   */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_OPAMP2                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_PWR                        */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_QUADSPI                    */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_RCC                        */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_RCC_C1                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_RCC_C2                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_RNG                        */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_RTC                        */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_SAI1                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_SAI2                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_SAI3                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_SAI4                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_SDMMC1                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_SDMMC2                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_SPDIFRX                    */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_SPI1                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_SPI2                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_SPI3                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_SPI4                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_SPI5                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_SPI6                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_SWPMI1                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_SYSCFG                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_TIM1                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_TIM12                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_TIM13                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_TIM14                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_TIM15                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_TIM16                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_TIM17                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_TIM2                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_TIM3                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_TIM4                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_TIM5                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_TIM6                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_TIM7                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_TIM8                       */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_TMPSENS                    */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_UART4                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_UART5                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_UART7                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_UART8                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_USART1                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_USART2                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_USART3                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_USART6                     */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_USB1_OTG_HS                */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_USB2_OTG_FS                */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_VREFBUF                    */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_WWDG1                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_WWDG2                      */
  RES_DEFAULT_ASSIGN_NONE , /* RESMGR_ID_RESMGR_TABLE               */
}

/* End of CubeMX/User Part*/

/** @defgroup RES_MGR_Lock_Procedure
  * @{
  */

/* Customized Lock Procedure definition  begin */

/* Customized Lock Procedure definition  end */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* RES_MGR_CONF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
