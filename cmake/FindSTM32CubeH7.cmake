include(FindPackageHandleStandardArgs)

# Create a list to hold all components.
set(STM32H7_COMPONENTS "")

set(STM32H7_DRIVERS_DIR ${CMAKE_CURRENT_LIST_DIR}/../Drivers)

include(${STM32H7_DRIVERS_DIR}/cmake/hal.cmake)
include(${STM32H7_DRIVERS_DIR}/cmake/cmsis.cmake)

list(REMOVE_DUPLICATES STM32CUBEH7_COMPONENTS)
list(REMOVE_DUPLICATES STM32CubeH7_FIND_COMPONENTS)

foreach (comp ${STM32CubeH7_FIND_COMPONENTS})
    if (${comp} IN_LIST STM32H7_COMPONENTS)
        # Find source files
        foreach (source ${STM32H7_${comp}_SOURCES})
            find_file(${source}_DIR ${source} PATHS ${STM32H7_${comp}_SEARCH_PATHS} NO_CACHE)

            if (${${source}_DIR} MATCHES "NOTFOUND")
                message(FATAL_ERROR "Failed to find the source file '${source}'.")
            else ()
                list(APPEND STM32CUBEH7_LIBRARY_SOURCE_FILES ${${source}_DIR})
                list(APPEND STM32CUBEH7_LIBRARY_INCLUDE_DIRS ${STM32H7_${comp}_SEARCH_PATHS})
            endif ()
        endforeach ()

        # Find header files and include directories
        foreach (header ${STM32H7_${comp}_HEADERS})
            find_file(${header}_DIR ${header} PATHS ${STM32H7_${comp}_SEARCH_PATHS} NO_CACHE)

            if (${${header}_dir} MATCHES "NOTFOUND")
                message(FATAL_ERROR "Failed to find header file '${header}'.")
            else ()
                list(APPEND STM32CUBEH7_LIBRARY_HEADER_FILES ${${header}_dir})
                list(APPEND STM32CUBEH7_INCLUDE_DIRS ${STM32H7_${comp}_SEARCH_PATHS})
            endif ()
        endforeach ()

        set(STM32CubeH7_${comp}_FOUND TRUE)
    else ()
        message(FATAL_ERROR "Component '${comp}' does not exist.")
    endif ()
endforeach ()

# Shift component checks to CMake
find_package_handle_standard_args(STM32CubeH7 HANDLE_COMPONENTS)

list(REMOVE_DUPLICATES STM32CUBEH7_INCLUDE_DIRS)
list(REMOVE_DUPLICATES STM32CUBEH7_LIBRARY_SOURCE_FILES)
list(REMOVE_DUPLICATES STM32CUBEH7_LIBRARY_HEADER_FILES)

# Create library for consuming project
add_library(stm32cubeh7 INTERFACE)

target_sources(
    stm32cubeh7
    INTERFACE
        ${STM32CUBEH7_LIBRARY_SOURCE_FILES}
        ${STM32CUBEH7_LIBRARY_HEADER_FILES}
)

target_include_directories(
    stm32cubeh7
    INTERFACE
        ${STM32CUBEH7_INCLUDE_DIRS}
)

if (DEFINED STM32H7_TARGET_DEVICE)
    string(TOUPPER ${STM32H7_TARGET_DEVICE} STM32H7_TARGET_DEVICE_UPPER)
    string(REPLACE "XX" "xx" STM32H7_TARGET_DEVICE_DEFINITION ${STM32H7_TARGET_DEVICE_UPPER})

    target_compile_definitions(
        stm32cubeh7
        INTERFACE
            ${STM32H7_TARGET_DEVICE_DEFINITION}
    )
endif ()
