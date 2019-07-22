---
pagetitle: Release Notes for Adafruit_Shield Board Drivers
lang: en
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

::: {.card .fluid}
::: {.sectione .dark}
<center>
# <small>Release Notes for</small> <mark>Adafruit_Shield Board Drivers</mark>
Copyright &copy; 2016 STMicroelectronics\
    
[![ST logo](../../../_htmresc/st_logo.png)](https://www.st.com){.logo}
</center>
:::
:::

# License

Licensed by ST under BSD 3-Clause license (the \"License\"). You may
not use this package except in compliance with the License. You may
obtain a copy of the License at:

[https://opensource.org/licenses/BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)

# Purpose

This directory contains the board drivers to demonstrate the capabilities of the Adafruit_Shield Kit.

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section20" checked aria-hidden="true">
<label for="collapse-section20" aria-hidden="true">V3.0.5 / 23-May-2019</label>
<div>			

## Main Changes

- stm32_adafruit_sd.h
  - Remove volatile qualification for SD_CID structure's fields

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section19" checked aria-hidden="true">
<label for="collapse-section19" aria-hidden="true">V3.0.4 / 03-April-2019</label>
<div>			

## Main Changes

- stm32_adafruit.c
  - Correct logical test in BSP_LCD_DisplayStringAt() and DrawChar()

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section18" aria-hidden="true">
<label for="collapse-section18" aria-hidden="true">V3.0.3 / 30-April-2018</label>
<div>			

## Main Changes

- stm32_adafruit_sd.c:
  - Fix BSP_SD_ReadBlocks and BSP_SD_WriteBlocks to support SDHC cards

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section17" aria-hidden="true">
<label for="collapse-section17" aria-hidden="true">V3.0.2 / 24-August-2017</label>
<div>			

## Main Changes

- stm32_adafruit_lcd.c:
  - Fix compilation errors with SW4STM32 toolchain

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section16" aria-hidden="true">
<label for="collapse-section16" aria-hidden="true">V3.0.1 / 02-June-2017</label>
<div>			

## Main Changes

- Add general description of BSP drivers
- Add Dependencies section
- Support of PDSC

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section155" aria-hidden="true">
<label for="collapse-section155" aria-hidden="true">V3.0.0 / 23-December-2016</label>
<div>						

## Main Changes

- Update BSP_SD_ReadBlocks and BSP_SD_WriteBlocks following new HAL SD drivers implementation
- Update BSP_SD_Erase implementation to manage SDHC and SDSC cards
- Fix block size to 512 bytes for all card types
- **Notes:** 
  - These Adafruit BSP drivers break the compatibility with previous versions.
  - If FatFs is required, *FatFS R0.11 ST modified 20161223* must be used with this version of Adafruit BSP drivers.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section15" aria-hidden="true">
<label for="collapse-section15" aria-hidden="true">V2.0.1 / 04-November-2016</label>
<div>						

## Main Changes

- Fix GNU GCC warning on missing break instruction in SD_GetDataResponse()

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section12" aria-hidden="true">
<label for="collapse-section12" aria-hidden="true">V2.0.0 / 10-September-2015</label>
<div>			

## Main Changes

- Improve SD over SPI protocol implementation
- **Note:** This new implementation requires to increase Heap_Size by 0x200 
- **Note:** This driver version needs BSP Nucleo driver V2.0.0 and later

</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section9" aria-hidden="true">
<label for="collapse-section9" aria-hidden="true">V1.1.1 / 21-November-2014</label>
<div>			

## Main Changes

- stm32_adafruit_sd.c
  - Add workaround for the issue seen on some STM32 Nucleo boards; reading the SD card status will return an error
  - A fix will be implemented in next release
- stm32_adafruit_lcd.h: change "\" by "/" in the include path to fix compilation issue under Linux
- Miscellaneous comments update

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section8" aria-hidden="true">
<label for="collapse-section8" aria-hidden="true">V1.1.0 / 22-July-2014</label>
<div>			

## Main Changes

- Add new user API BSP_LCD_DrawBitmap() to draw a bitmap picture loaded in the STM32 MCU internal memory
- Add new static API SetDisplayWindow(), needed by BSP_LCD_DrawBitmap() 
- Update static API DrawChar()
- **Note:** This driver version needs ST7735 component driver V1.1.0 and later.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section7" aria-hidden="true">
<label for="collapse-section7" aria-hidden="true">V1.1.0 / 22-April-2014</label>
<div>			

## Main Changes

- First official release

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on <mark>STM32 Microcontrollers</mark> ,
visit: [http://www.st.com/STM32](http://www.st.com/STM32)
</footer>
