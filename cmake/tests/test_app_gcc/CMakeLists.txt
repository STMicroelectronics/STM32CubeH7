cmake_minimum_required(VERSION 3.31.6)

project(
    test_app_gcc
    LANGUAGES
        C
        ASM
)

find_package(
    STM32CubeH7
    COMPONENTS
        hal_all
        stm32h750xx
        system_init
        startup
    REQUIRED
)

set(TARGET test_app_gcc)

add_executable(${TARGET})

target_sources(
    ${TARGET}
    PRIVATE
        main.c
        cfg/stm32h7xx_hal_conf.h
)

target_compile_options(
    ${TARGET}
    PRIVATE
        -g
        -ffunction-sections
        -fdata-sections
        -march=armv7e-m+fp.dp
)

target_compile_definitions(
    ${TARGET}
    PRIVATE
        USE_FULL_LL_DRIVER
)

target_link_options(
    ${TARGET}
    PRIVATE
        -specs=nano.specs
        -nostartfiles
        -T ${CMAKE_CURRENT_LIST_DIR}/cfg/linker.ld
        "LINKER:--print-memory-usage"
        "LINKER:--gc-sections"
)

target_include_directories(
    ${TARGET}
    PRIVATE
        cfg
)

target_link_libraries(
    ${TARGET}
        stm32cubeh7
)
