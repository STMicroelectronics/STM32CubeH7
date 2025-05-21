set(STM32H7_CMSIS_CORE_DIR ${STM32H7_DRIVERS_DIR}/CMSIS/Core)

set(STM32H7_CMSIS_CORE_COMPONENTS cmsis_core)

# This component is required, so add it automatically
list(APPEND STM32CubeH7_FIND_COMPONENTS cmsis_core)

list(APPEND STM32H7_CMSIS_COMPONENTS cmsis_core)

set(STM32H7_cmsis_core_SEARCH_PATHS ${STM32H7_CMSIS_CORE_DIR}/Include)

set(STM32H7_cmsis_core_SOURCES "")

set(STM32H7_cmsis_core_HEADERS core_cm4.h core_cm7.h)
