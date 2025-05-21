set(STM32H7_HAL_DIR ${STM32H7_DRIVERS_DIR}/STM32H7xx_HAL_Driver)

set(STM32H7_HAL_COMPONENTS
    adc adc_ex cec comp cortex cordic crc crc_ex cryp cryp_ex dac dac_ex dcmi dfsdm dfsdm_ex dma dma2d dma_ex dsi dts
    eth eth_ex exti fdcan flash flash_ex fmac gfxmmu gpio hash hash_ex hcd hrtim hsem i2c i2c_ex i2s i2s_ex irda iwdg
    jpeg lptim ltdc ltdc_ex mdios mdma mmc mmc_ex nand nor opamp opamp_ex ospi otfdec pcd pcd_ex pssi pwr pwr_ex qspi
    ramecc rcc rcc_ex rng rng_ex rtc rtc_ex sai sai_ex sd sd_ex sdio sdram smartcard smartcard_ex smbus smbus_ex spdifrx
    spi spi_ex sram swpmi tim tim_ex uart uart_ex usart usart_ex wwdg eth_leg eth_ex_leg
)

set(STM32H7_HAL_LEGACY_COMPONENTS eth_leg eth_ex_leg)

list(APPEND STM32H7_COMPONENTS ${STM32H7_HAL_COMPONENTS} ${STM32H7_HAL_LEGACY_COMPONENTS})

# Easy way to load all HAL components at once
if (hal_all IN_LIST STM32CubeH7_FIND_COMPONENTS)
    list(APPEND STM32CubeH7_FIND_COMPONENTS ${STM32H7_HAL_COMPONENTS})
    list(REMOVE_ITEM STM32CubeH7_FIND_COMPONENTS hal_all)
endif ()

# Automatically add necessary HAL components
foreach (comp ${STM32H7_HAL_COMPONENTS})
    if (${comp} IN_LIST STM32CubeH7_FIND_COMPONENTS)
        list(APPEND STM32CubeH7_FIND_COMPONENTS cortex rcc )
        break()
    endif ()
endforeach ()

foreach (comp ${STM32H7_HAL_COMPONENTS})
    set(STM32H7_${comp}_SEARCH_PATHS ${STM32H7_HAL_DIR}/Src ${STM32H7_HAL_DIR}/Inc)

    set(STM32H7_${comp}_SOURCES stm32h7xx_hal.c stm32h7xx_hal_${comp}.c)

    set(STM32H7_${comp}_HEADERS stm32h7xx_hal.h stm32h7xx_hal_${comp}.h)
endforeach ()

foreach (comp ${STM32H7_HAL_LEGACY_COMPONENTS})
    set(STM32H7_${comp}_SEARCH_PATHS ${STM32H7_HAL_DIR}/Src/Legacy ${STM32H7_HAL_DIR}/Inc/Legacy)

    if(${comp} STREQUAL eth_leg)
        set(STM32H7_${comp}_SOURCES stm32h7xx_hal.c stm32h7xx_hal_eth.c)

        set(STM32H7_${comp}_HEADERS stm32h7xx_hal_legacy.h stm32h7xx_hal_eth_legacy.h)
    elseif (${comp} STREQUAL eth_ex_leg)
        set(STM32H7_${comp}_SOURCES stm32h7xx_hal.c stm32h7xx_hal_eth_ex.c)

        set(STM32H7_${comp}_HEADERS stm32h7xx_hal_legacy.h stm32h7xx_hal_eth_ex_legacy.h)
    endif ()
endforeach ()
