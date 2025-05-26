# CMake Support for STM32CubeH7

## Overview

This repository provides CMake support for projects that wish to use it as a dependency. It enables consuming 
projects to integrate this repository and selectively include its components as part of their build.

## Usage

### Cloning

To use this repository, consuming projects must first instruct CMake to clone it. This can be done by adding the
following to a CMake file within the consuming project:

```cmake
# Include the FetchContent Module
include(FetchContent)

# Declare the dependency
FetchContent_Declare(
    stm32cubeh7
    GIT_REPOSITORY git@github.com:STMicroelectronics/STM32CubeH7.git
    GIT_TAG master
)

# Fetch and make the dependency available
FetchContent_MakeAvailable(stm32cubeh7)

# Add the CMake module path so CMake can find the package
list(APPEND CMAKE_MODULE_PATH ${stm32cubeh7_SOURCE_DIR}/cmake)
```

When processed, this will automatically clone the master branch of the STM32CubeH7 repository.

### Finding the Package

The consuming project must instruct CMake to locate this package and specify which components it requires. This
determines which source files, header files, and include directories are made available during compilation.

This is done with a call to `find_package()`. At a minimum, the call should specify the microcontroller used by the
consuming project. For example:

```cmake
find_package(
    # Name of the package to find
    STM32CubeH7
    COMPONENTS
        # Microcontroller used by the consuming project
        stm32h750xx
    REQUIRED
)
```

#### HAL Components

Specifying a HAL component in the `find_package()` call makes that component’s source files, header files, and include
directories available to the consuming project during compilation.

For example, adding `i2c` to the list of required components will provide the following files:

- `stm32h7xx_hal.c`
- `stm32h7xx_hal_i2c.c`
- `stm32h7xx_hal_i2c_ex.c`
- `stm32h7xx_ll_i2c.c`

Alternatively, specifying the `hal_all` component will make all available HAL component source files and include
directories available to the consuming project.

The following is a list of all available HAL components that can be specified:

`adc`, `bdma`, `cec`, `comp`, `cordic`, `cortex`, `crc`, `crs`, `cryp`, `dac`,  
`dcmi`, `delayblock`, `dfsdm`, `dma`, `dma2d`, `dsi`, `dts`, `eth`, `exti`, `fdcan`,  
`flash`, `fmac`, `fmc`, `gfxmmu`, `gpio`, `hal_all`, `hash`, `hcd`, `hrtim`, `hsem`,  
`i2c`, `i2s`, `irda`, `iwdg`, `jpeg`, `lptim`, `lpuart`, `ltdc`, `mdios`, `mdma`,  
`mmc`, `nand`, `nor`, `opamp`, `ospi`, `otfdec`, `pcd`, `pssi`, `pwr`, `qspi`,  
`ramecc`, `rcc`, `rng`, `rtc`, `sai`, `sd`, `sdio`, `sdmmc`, `sdram`, `smartcard`,  
`smbus`, `spdifrx`, `spi`, `sram`, `swpmi`, `tim`, `uart`, `usart`, `usb`, `utils`, `wwdg`

#### Device Components

The consuming project must specify which device it is targeting. This allows CMake to configure the appropriate
preprocessor macros and include device-specific source files.

There are two ways to specify the target device:

- By setting the `STM32H7_TARGET_DEVICE` variable:

```cmake
set(STM32H7_TARGET_DEVICE stm32h750xx)
```

- Or by passing the device as a component to find_package():

```cmake
find_package(
    STM32CubeH7
    COMPONENTS
        stm32h750xx  # Microcontroller used by the consuming project
    REQUIRED
)
```

The following device components are available:

`stm32h7a3xx`, `stm32h7a3xxq`, `stm32h7b0xx`, `stm32h7b0xxq`, `stm32h7b3xx`, `stm32h7b3xxq`, `stm32h723xx`,
`stm32h725xx`, `stm32h730xx`, `stm32h730xxq`, `stm32h733xx`, `stm32h735xx`, `stm32h742xx`, `stm32h743xx`,
`stm32h745xg`, `stm32h745xx`, `stm32h747xg`, `stm32h747xx`, `stm32h750xx`, `stm32h753xx`, `stm32h755xx`,
`stm32h757xx`

#### CMSIS Components

There are two CMSIS components that can be specified: `startup` and `system_init`.

The startup component can be omitted if you are using custom startup files, such as `crt0.c` provided by ARM GCC or
your own implementation. The STM32CubeH7 package includes its own startup code for each supported target device.
If the `startup` component is specified, the package’s corresponding startup code for the selected device will be
made available.

If the `system_init` component is specified, the system_stm32h7xx.c source file will, by default, be made available to
the consuming project.

This behavior can be customized by setting the `STM32H7_BOOT_CONFIGURATION` variable. The table below outlines the
available options:

| Value                                 | Description                                                                        |
|---------------------------------------|------------------------------------------------------------------------------------|
| `STM32H7XX_DUALCORE_BOOT_CM4_CM7`     | For use with dualcore devices. Boots both the CM4 and CM7 device cores.            |
| `STM32H7XX_DUALCORE_BOOTCM4_CM7GATED` | For use with dualcore devices. Boots the CM4 core, but does not boot the CM7 core. |
| `STM32H7XX_DUALCORE_BOOTCM7_CM4GATED` | For use with dualcore devices. Boots the CM7 core, but does not boot the CM4 core. |
| `STM32H7XX_SINGLECORE`                | For use with single core devices.                                                  |

## CMake Tests

CMake-based tests are available to verify functionality, and the project uses CMake presets to simplify configuration
and building.

### Available Presets

Currently, the following CMake presets are defined:

- Configure Presets

  - `stm32h7-gcc`: Configures the project to build using ARM GCC using the Ninja generator.

- Build Presets

  - `gcc-debug`: Builds the `stm32h750b_dk_test_app_gcc` target using ARM GCC with the Debug configuration.

### Running the Tests

1. Download the required toolchains by running this command from the `cmake/tests/scripts` directory:
```cmake
cmake -P download_toolchains.cmake
```

This will download toolchains into `cmake/tests/scripts/.cmake`.

2. Configure and build the test application using the presets:

```cmake
cmake --preset stm32h7-gcc
cmake --build --preset gcc-debug
```

More tests and presets will be added in the future as development continues.
