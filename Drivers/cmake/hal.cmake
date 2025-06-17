set(STM32H7_HAL_DIR ${STM32H7_DRIVERS_DIR}/STM32H7xx_HAL_Driver)

set(STM32H7_HAL_COMPONENTS
    adc bdma cec comp cordic cortex crc crs cryp dac dcmi delayblock dfsdm dma dma2d dsi dts
    eth exti fdcan flash fmac fmc gfxmmu gpio hash hcd hrtim hsem i2c i2s irda iwdg
    jpeg lptim lpuart ltdc mdios mdma mmc nand nor opamp ospi otfdec pcd pssi pwr qspi
    ramecc rcc rng rtc sai sd sdio sdmmc sdram smartcard smbus spdifrx
    spi sram swpmi tim uart usart usb utils wwdg
)

set(STM32H7_HAL_LEGACY_COMPONENTS eth_leg eth_ex_leg)

set(only_ll_components bdma crs delayblock fmc lpuart sdmmc usb utils)

set(ll_components adc comp cordic crc dac dma dma2d exti fmac gpio hrtim
    i2c lptim mdma opamp pwr rcc rng rtc spi swpmi tim usart ${only_ll_components})

set(ex_components adc crc cryp dac dfsdm dma eth flash hash i2c i2s ltdc mmc opamp pcd pwr rcc rng rtc sai sd
    smartcard smbus spi tim uart usart)

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

    set(STM32H7_${comp}_SOURCES stm32h7xx_hal.c)

    set(STM32H7_${comp}_HEADERS stm32h7xx_hal.h)

    if (${comp} IN_LIST ll_components)
        list(APPEND STM32H7_${comp}_SOURCES stm32h7xx_ll_${comp}.c)
        list(APPEND STM32H7_${comp}_HEADERS stm32h7xx_ll_${comp}.h)
    endif ()

    if (${comp} IN_LIST ex_components)
        list(APPEND STM32H7_${comp}_SOURCES stm32h7xx_hal_${comp}_ex.c)
        list(APPEND STM32H7_${comp}_HEADERS stm32h7xx_hal_${comp}_ex.h)
    endif ()

    if (NOT ${comp} IN_LIST only_ll_components)
        list(APPEND STM32H7_${comp}_SOURCES stm32h7xx_hal_${comp}.c)
        list(APPEND STM32H7_${comp}_HEADERS stm32h7xx_hal_${comp}.h)
    endif ()
endforeach ()

foreach (comp ${STM32H7_HAL_LEGACY_COMPONENTS})
    set(STM32H7_${comp}_SEARCH_PATHS ${STM32H7_HAL_DIR}/Src/Legacy ${STM32H7_HAL_DIR}/Inc/Legacy)

    if(${comp} STREQUAL "eth_leg")
        set(STM32H7_${comp}_SOURCES stm32h7xx_hal.c stm32h7xx_hal_eth.c)

        set(STM32H7_${comp}_HEADERS stm32h7xx_hal_legacy.h stm32h7xx_hal_eth_legacy.h)
    elseif (${comp} STREQUAL "eth_ex_leg")
        set(STM32H7_${comp}_SOURCES stm32h7xx_hal.c stm32h7xx_hal_eth_ex.c)

        set(STM32H7_${comp}_HEADERS stm32h7xx_hal_legacy.h stm32h7xx_hal_eth_ex_legacy.h)
    endif ()
endforeach ()
