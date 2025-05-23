include(FetchContent)

FetchContent_Declare(
    stm32cubeh7
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../..
)

FetchContent_MakeAvailable(stm32cubeh7)

list(APPEND CMAKE_MODULE_PATH ${stm32cubeh7_SOURCE_DIR}/cmake)
