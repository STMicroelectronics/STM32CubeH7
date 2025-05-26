cmake_minimum_required(VERSION 3.31.6)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

project(
    stm32h750b_dk_test_app_gcc
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

set(TARGET stm32h750b_dk_test_app_gcc)

add_executable(${TARGET})

target_sources(
    ${TARGET}
    PRIVATE
        main.c
        ${CMAKE_CURRENT_LIST_DIR}/cfg/stm32h7xx_hal_conf.h
)

target_compile_options(
    ${TARGET}
    PRIVATE
        -Og
        -g
        -mthumb
        -ffunction-sections
        -fdata-sections
        -mfloat-abi=hard
        -mfpu=fpv5-d16
        -march=armv7e-m+fp.dp
)

target_compile_definitions(
    ${TARGET}
    PRIVATE
        USE_FULL_LL_DRIVER
        USER_VECT_TAB_ADDRESS
        USE_PWR_LDO_SUPPLY
)

target_link_options(
    ${TARGET}
    PRIVATE
        -specs=nano.specs
        -nostartfiles
        -mfloat-abi=hard
        -mfpu=fpv5-d16
        -march=armv7e-m+fp.dp
        -T ${CMAKE_CURRENT_LIST_DIR}/cfg/linker.ld
        "LINKER:--print-memory-usage"
        "LINKER:--gc-sections"
        "LINKER:-Map=${CMAKE_BINARY_DIR}"
)

target_include_directories(
    ${TARGET}
    PRIVATE
        cfg
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(
    ${TARGET}
        stm32cubeh7
)
