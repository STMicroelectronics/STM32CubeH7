#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <touchgfx/hal/HAL.hpp>

#ifndef SIMULATOR
#include "bsp.h"

extern DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef hltdc;
extern TIM_HandleTypeDef TimHandle;
#if defined ( USE_UART_TRACES )
extern UART_HandleTypeDef UART_Handle;
#endif /* USE_UART_TRACES */
#endif /* ! SIMULATOR */

Model::Model() :
    modelListener(0),
    autoDemoTimeOutMs(20)
{

}

void Model::tick()
{

}

void Model::SetautoAnimate( bool value )
{
  modelListener->SetautoAnimate( value );
}


bool Model::GetautoAnimate( )
{
  return modelListener->GetautoAnimate( );
}

void Model::startDemo(DEMOS demoId)
{
    unsigned long demo_addr = 0x0;

    switch (demoId)
    {
        case OSCILLOSCOPE:
            demo_addr = 0x90800000;
            break;
        case EEMBC:
            demo_addr = 0x08080000;
            break;
        case TouchGFX:
            demo_addr = 0x90900000;
            break;
        case NUMBER_OF_DEMOS:
        default:
            return;
    }

#ifdef SIMULATOR
    touchgfx_printf("\nJumping to Sub-Demo @0x%08X ...\n", demo_addr);
#else
#if defined ( USE_UART_TRACES )
    printf("\nJumping to Sub-Demo @0x%08X ...\n", demo_addr);
#endif /* USE_UART_TRACES */

    HAL_PWR_EnableBkUpAccess();
    WRITE_REG(BKP_REG_DEMO_LOC, demo_addr);
    HAL_PWR_DisableBkUpAccess();

    /* DeInit TS */
    BSP_TS_DeInit(0);

    /* DeInit DMA2D */
    HAL_DMA2D_DeInit(&hdma2d);

    /* DeInit LTDC */
    HAL_LTDC_DeInit(&hltdc);

    /* De-assert backlight LCD_BL_CTRL pin */
    HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_RESET);
    /* De-assert display enable LCD_DISP_CTRL pin */
    HAL_GPIO_WritePin(LCD_DISP_CTRL_GPIO_PORT, LCD_DISP_CTRL_PIN, GPIO_PIN_RESET);
    /* Assert display enable LCD_DISP_EN pin */
    HAL_GPIO_WritePin(LCD_DISP_EN_GPIO_PORT, LCD_DISP_EN_PIN, GPIO_PIN_SET);

    /* Disable externla memories */
    BSP_OSPI_NOR_DeInit(0);
    BSP_OSPI_RAM_DeInit(0);

#if defined ( USE_UART_TRACES )
    /* Disable traces */
    HAL_UART_DeInit( &UART_Handle );
#endif /* USE_UART_TRACES */

    /* Disable Systick interrupt */
    SysTick->CTRL = 0;
    HAL_SuspendTick();
    HAL_TIM_Base_DeInit(&TimHandle);

    /* Disable the MPU */
    HAL_MPU_Disable();

    /* Do Software Reset */
    HAL_NVIC_SystemReset();
#endif /* ! SIMULATOR */
}

