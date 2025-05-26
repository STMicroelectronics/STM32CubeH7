# Check the host operating system
if (APPLE)
    execute_process(COMMAND uname -m OUTPUT_VARIABLE HOST_PROCESSOR OUTPUT_STRIP_TRAILING_WHITESPACE)

    set(ARM_GCC_EXTRACT_DIR ${CMAKE_SOURCE_DIR}/.cmake/toolchains)
    set(ARM_GCC_PACKAGE_FILENAME arm-gnu-toolchain-14.2.rel1-darwin-${HOST_PROCESSOR}-arm-none-eabi.tar.xz)
endif ()

set(ARM_GCC_ARCHIVE ${ARM_GCC_EXTRACT_DIR}/${ARM_GCC_PACKAGE_FILENAME})
set(ARM_GCC_URL https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/${ARM_GCC_PACKAGE_FILENAME})

# Create the toolchains directory if it doesn't exist
file(MAKE_DIRECTORY ${ARM_GCC_EXTRACT_DIR})

#set(temp_path ${ARM_GCC_EXTRACT_DIR}/${ARM_GCC_PACKAGE_FILENAME})
# Call this twice because ARM GCC has two extensions that need to be removed
cmake_path(REMOVE_EXTENSION ARM_GCC_ARCHIVE LAST_ONLY OUTPUT_VARIABLE ARM_GCC_ARCHIVE_INSTALL_DIR)
cmake_path(REMOVE_EXTENSION ARM_GCC_ARCHIVE_INSTALL_DIR LAST_ONLY OUTPUT_VARIABLE ARM_GCC_ARCHIVE_INSTALL_DIR)

# Download ARM GCC and install it
if (NOT EXISTS ${ARM_GCC_ARCHIVE} AND NOT EXISTS ${ARM_GCC_ARCHIVE_INSTALL_DIR})
    message(STATUS "Downloading ARM GCC toolchain...")

    file(DOWNLOAD ${ARM_GCC_URL} ${ARM_GCC_ARCHIVE} STATUS download_status SHOW_PROGRESS)

    list(GET download_status 0 1 download_status_code download_status_message)

    if (NOT ${download_status_code} EQUAL 0)
        message(FATAL_ERROR "Failed to download ARM GCC. ${download_status_message}")
    endif ()
endif ()

if (EXISTS ${ARM_GCC_ARCHIVE} AND NOT EXISTS ${ARM_GCC_ARCHIVE_INSTALL_DIR})
    message(STATUS "Extracting ARM GCC toolchain to ${ARM_GCC_EXTRACT_DIR}.")

    file(ARCHIVE_EXTRACT INPUT ${ARM_GCC_ARCHIVE} DESTINATION ${ARM_GCC_EXTRACT_DIR} VERBOSE)
endif ()

# Delete the downloaded archive.
if (EXISTS ${ARM_GCC_ARCHIVE})
    file(REMOVE ${ARM_GCC_ARCHIVE})
endif ()
