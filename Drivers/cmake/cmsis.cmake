set(STM32H7_CMSIS_COMPONENTS)

# Ordering is important here
include(${CMAKE_CURRENT_LIST_DIR}/cmsis_components/device.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmsis_components/core.cmake)

list(APPEND STM32H7_COMPONENTS ${STM32H7_CMSIS_COMPONENTS})
