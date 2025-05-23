include(${CMAKE_CURRENT_LIST_DIR}/../scripts/find_toolchain_program.cmake)

set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE   STATIC_LIBRARY)

find_toolchain_program(arm-none-eabi-gcc    C_COMPILER)
find_toolchain_program(arm-none-eabi-g++    CXX_COMPILER)

set(CMAKE_C_COMPILER    ${C_COMPILER})
set(CMAKE_ASM_COMPILER  ${C_COMPILER})
set(CMAKE_CXX_COMPILER  ${CXX_COMPILER})
