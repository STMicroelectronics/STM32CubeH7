set(TOOLCHAIN_DIR ${CMAKE_CURRENT_LIST_DIR}/.cmake/toolchains)

# Check if the toolchain directory even exists
if (NOT EXISTS ${TOOLCHAIN_DIR})
    message(FATAL_ERROR "Could not locate the directory ${TOOLCHAIN_DIR}. "
        "Make sure you have ran the command `cmake -P download_toolchains.cmake` from your command line before running "
        "this CMake project.")
endif ()

# Locate possible directories for ARM GCC toolchain
file(GLOB ARM_GCC_DIRS "${TOOLCHAIN_DIR}/arm-gnu-toolchain-*/bin")
list(GET ARM_GCC_DIRS 0 FIRST_ARM_GCC_DIR_MATCH)

# TODO: Add possible directories for the IAR toolchain after support for it has been added

# Function to locate a program from the available toolchains
function(find_toolchain_program PROGRAM_NAME OUTPUT_VAR)
    find_program(
        ${OUTPUT_VAR}
        NAMES ${PROGRAM_NAME}
        HINTS ${FIRST_ARM_GCC_DIR_MATCH} # TODO: Update this to include additional directories for other toolchains such as IAR
        PATH_SUFFIXES
        DOC   "Path to ${PROGRAM_NAME}."
        REQUIRED
    )
endfunction()
