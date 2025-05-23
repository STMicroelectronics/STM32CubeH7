cmake_minimum_required(VERSION 3.31.6)

# Establish high-level test project
project(STM32CubeH7_CMake_Tests)

# Pull in any dependencies for this test project
include(dependencies.cmake)

# Add relevant tests
add_subdirectory(test_app_gcc)
