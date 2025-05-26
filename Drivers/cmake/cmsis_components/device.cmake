set(STM32H7_CMSIS_DEVICE_DIR ${STM32H7_DRIVERS_DIR}/CMSIS/Device)

set(STM32H7_STARTUP_COMPONENTS startup system_init)

set(STM32H7_DEVICE_COMPONENTS
    stm32h7a3xx
    stm32h7a3xxq
    stm32h7b0xx
    stm32h7b0xxq
    stm32h7b3xx
    stm32h7b3xxq
    stm32h723xx
    stm32h725xx
    stm32h730xx
    stm32h730xxq
    stm32h733xx
    stm32h735xx
    stm32h742xx
    stm32h743xx
    stm32h745xg
    stm32h745xx
    stm32h747xg
    stm32h747xx
    stm32h750xx
    stm32h753xx
    stm32h755xx
    stm32h757xx
)

list(APPEND STM32H7_CMSIS_COMPONENTS ${STM32H7_DEVICE_COMPONENTS} ${STM32H7_STARTUP_COMPONENTS})

# Available boot configurations for single core and dual core devices
set(STM32H7_CORE_BOOT_CONFIGURATIONS
    STM32H7XX_DUALCORE_BOOT_CM4_CM7
    STM32H7XX_DUALCORE_BOOTCM4_CM7GATED
    STM32H7XX_DUALCORE_BOOTCM7_CM4GATED
    STM32H7XX_SINGLECORE
)

# Support compilers for startup code
set(STM32H7_STARTUP_SUPPORTED_COMPILERS
    ARMCC
    GNU
    IAR
)

# Helper: build a clean printable list of valid devices
set(CLEAN_TARGET_DEVICE_LIST "\n")
foreach (device ${STM32H7_DEVICE_COMPONENTS})
    string(APPEND CLEAN_TARGET_DEVICE_LIST "   ${device}\n")
endforeach ()

foreach (comp ${STM32H7_DEVICE_COMPONENTS})
    # This will be used later when creating the target library
    if (${comp} IN_LIST STM32CubeH7_FIND_COMPONENTS)
        if (NOT DEFINED STM32H7_TARGET_DEVICE)
            # If a target device component was passed, then save this information for later.
            set(STM32H7_TARGET_DEVICE ${comp})
        else ()
            # More than one device was specified.
            message(FATAL_ERROR
                "Only one device can be specified. "
                "The device `${STM32H7_TARGET_DEVICE}` has already been specified. "
                "CMake detected that the device `${comp}` was also specified. ")
        endif ()
    endif ()

    set(STM32H7_${comp}_SEARCH_PATHS
        ${STM32H7_CMSIS_DEVICE_DIR}/ST/STM32H7xx/Include
    )

    if (startup IN_LIST STM32CubeH7_FIND_COMPONENTS)
        if (CMAKE_ASM_COMPILER_ID AND CMAKE_ASM_COMPILER_ID IN_LIST STM32H7_STARTUP_SUPPORTED_COMPILERS)
            if (CMAKE_ASM_COMPILER_ID STREQUAL "ARMCC")
                list(APPEND STM32H7_${comp}_SEARCH_PATHS ${STM32H7_CMSIS_DEVICE_DIR}/ST/STM32H7xx/Source/Templates/arm)
            elseif (CMAKE_ASM_COMPILER_ID STREQUAL "GNU")
                list(APPEND STM32H7_${comp}_SEARCH_PATHS ${STM32H7_CMSIS_DEVICE_DIR}/ST/STM32H7xx/Source/Templates/gcc)
            elseif (CMAKE_ASM_COMPILER_ID STREQUAL "IAR")
                list(APPEND STM32H7_${comp}_SEARCH_PATHS ${STM32H7_CMSIS_DEVICE_DIR}/ST/STM32H7xx/Source/Templates/iar)
            endif ()

            list(APPEND STM32H7_${comp}_SOURCES startup_${comp}.s)
        else ()
            message(WARNING "The 'startup' component was requested, but the currently selected ASM compiler "
                "('${CMAKE_ASM_COMPILER}') is not supported by the provided startup code. "
                "As a result, the startup code will be excluded from the generated 'stm32cubeh7' library. "
                "To proceed, either supply your own startup code in the consuming project, or switch to one of the "
                "following supported compilers: ${STM32H7_STARTUP_SUPPORTED_COMPILERS}.")
        endif ()
    endif ()

    set(STM32H7_${comp}_HEADERS ${comp}.h)
endforeach ()

# Validate that STM32H7_TARGET_DEVICE was set and is valid
if (NOT DEFINED STM32H7_TARGET_DEVICE)
    message(FATAL_ERROR
        "No target device was specified as a component for the STM32CubeH7 package. "
        "To resolve this, pass the target device name as a component to the STM32CubeH7 package. "
        "You may choose from the following valid options: ${CLEAN_TARGET_DEVICE_LIST}")
elseif (NOT ${STM32H7_TARGET_DEVICE} IN_LIST STM32H7_DEVICE_COMPONENTS)
    message(FATAL_ERROR
        "'STM32H7_TARGET_DEVICE' has been set to '${STM32H7_TARGET_DEVICE}'. "
        "This is not a valid option. "
        "Please check to ensure this is not defined improperly in your project settings. "
        "You may choose from the following valid options: ${CLEAN_TARGET_DEVICE_LIST}")
endif ()

if (NOT ${STM32H7_TARGET_DEVICE} IN_LIST STM32CubeH7_FIND_COMPONENTS)
    list(APPEND STM32CubeH7_FIND_COMPONENTS ${STM32H7_TARGET_DEVICE})
endif ()

# Remove `startup` from components since it's not needed anymore. The above `foreach()` loop took care of it.
list(REMOVE_ITEM STM32CubeH7_FIND_COMPONENTS startup)

set(STM32H7_system_init_SEARCH_PATHS ${STM32H7_CMSIS_DEVICE_DIR}/ST/STM32H7xx/Source/Templates)

if (DEFINED STM32H7_BOOT_CONFIGURATION AND STM32H7_BOOT_CONFIGURATION IN_LIST STM32H7_CORE_BOOT_CONFIGURATIONS)
    string(TOLOWER ${STM32H7_BOOT_CONFIGURATION} STM32H7_BOOT_CONFIGURATION_LOWER)

    list(APPEND STM32H7_system_init_SOURCES system_${STM32H7_BOOT_CONFIGURATION_LOWER}.c)
else ()
    if (DEFINED STM32H7_BOOT_CONFIGURATION AND NOT STM32H7_BOOT_CONFIGURATION IN_LIST STM32H7_CORE_BOOT_CONFIGURATIONS)
        message(FATAL_ERROR "The specified boot configuration '${STM32H7_BOOT_CONFIGURATION}' is not valid. "
            "The only valid options are: ${STM32H7_CORE_BOOT_CONFIGURATIONS}.")
    else ()
        list(APPEND STM32H7_system_init_SOURCES system_stm32h7xx.c)
    endif ()
endif ()

set(STM32H7_system_init_HEADERS "")
