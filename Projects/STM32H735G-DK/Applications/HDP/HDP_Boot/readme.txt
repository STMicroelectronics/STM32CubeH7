/**
  @page HDP_Boot application

  @verbatim
  ******************************************************************************
  * @file    HDP/HDP_Boot/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the HDP_Boot application
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description
This project provides a full description of how to use an RSS service to jump from secure area to non secure area. It shows also how to set, use and remove a single secure area.

STM32H735xx microcontroller embeds enhanced protection service activated by option bytes.
One of this service allows to define up to two secure hide protection areas (HDP).
The size of the flash secure area(s) is chosen by the user and can be up to a complete flash bank.
This protected area can only be set and executed in secure access mode. It will be executed only once during the boot.
Typical applications are code signature or integrity checking (user secure boot), software license checking, secure firmware update….

The removal of the secure area can only be done through a regression where the complete flash is erased.

The user can run this application in "Standard mode" or in "Secure access mode" : 

   - In Standard mode, User can run the application example without setting the secure area .
     
   - In Secure access mode, the user can set the secure area to run the application.
     Booting is forced in the RSS and the secure area code is executed. A service of the RSS allows to jump out of the secure area into the application code at the address defined by the user (see the security area exiting service described in the Reference Manual)  

The HDP application consists in two distinct projects: the HDP_Boot and HDP_Appli.
Both HDP_Boot and HDP_Appli needs to be flashed separately.
HDP_Boot code is executed starting at Flash address 0x08000000 correspending to the first flash area. 

@note that the STLogo is stored in the first flash area that will be later set as secure
The HDP_Boot will jump to the defined address (0x08040000) where the firmware HDP_Appli will be
loaded.

@note Before setting the secure area, it is advised to extensively test the firmware intended
to be executed in this area. All the possible execution locks must be treated.

@note User should follow carefully every step displayed on LCD screen to set and remove a secure area since in some explained cases the device can be locked and can’t be retrieved.

STM32H735G-DK LEDs behavior is displayed on LCD screen.

@note In this example the start address of the second flash has been chosen. 
      It has just to be insured that the firmwares HDP_Boot and HDP_Appli have non overlapping
      address ranges and that both compiled code are fitting in the chosen ranges.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@ note Setting secure area:
      Before setting a secure address range it’s very important to have a valid code
      in this area.
      After booting, the code stored in the secure area will be automatically executed.
      During this code execution the debugger can’t be connected, it will be like an
      extension of the boot but with user defined code and only the CPU accessing to the flash.
      The last instruction executed in this secure area is a “jump” out of the secure area.
      (RSS->exitSecureArea).
      The debugger can then be connected, but the flash content of the chosen secure
      area will be read out as “0000 0000”
      If for any reason the execution in the secure area is locked and the “jump” instruction
      is never executed, it will not be possible anymore to connect a debugger and also not
      possible anymore to modify the flash content.

    1) Set the security bit:
        - Execute the firmware till end and connect the CubeProgrammer (select “Hot Plug” as connecting mode)
        - Click on OB:=> User Configuration
        - Click “SECURITY” => Apply 
        - Click on and read the flash content starting at address 0x0800 0000 and at address 0x0804 0000. Both stored code are visible.
        - Proceed with a power down reset and verify that the two firmwares are still executed
          correctly till the end.

    2) Set the secure area:

        The start and end address of the secure area must be set as indicated on the display.
        - CubeProgrammer: Click on : => Secure Protection
        - Set the start and end address of the secure area => click “Apply”
        - For this example set: Start address: 0x0800 0000 and End address: 0x0801 FFFF
        - Connection will be lost.
        - Proceed with power down reset then execute the firmware till end.

@note Removal of the secure area
      To remove the secure area, it will be required to set first the readout protection (RDP) to
      level 1.
      This will insure that the regression to level 0 will remove the complete flash including
      the content of the secure flash area.
      Please read the product reference manual for the readout protection description.
      Take care to not set the readout protection to level 2 that will definitively lock the
      device.

    1) Setting of the RDP to level 1:

        - Connect Cubeprogrammer after jumping to second firmware.
        - Click on OB: => Read Out Protection => select “BB” to set to level 1 -> click “Apply”
        - Connection will be lost. 
        - Proceed with power down reset then execute the firmware till end.

    2) Resetting the secure area and flash content removal:

        - Connect Cubeprogrammer after jumping to second firmware.
        - An error message will be displayed since the flash content can’t be read due to the RDP level 1 but the option byte can be accessed.
        - Click on OB: =>Open both windows: “Read Out Protection” and “Secure Protection”
        - Set RDP to AA (don’t click apply yet)
        - Set: a secure area start address higher than the secure area end address. (for instance: start address 0x08002000; end address 0x08001000)
        - Click “Apply” only once you have checked that all the settings are as wanted.
        - The complete flash is erased.

    3) Resetting the secure option bit:

        - Connect Cubeprogrammer.
        - Click on OB: => User Configuration
        - Uncheck “SECURITY” => Apply

@par Keywords

HDP, Boot, Secure, Non-secure, Display, Secure area, RDP

@par Directory contents

  - HDP/HDP_Boot/Inc/mx25lm51245g_conf.h                              MX25LM51245G OSPI FLASH memory configuration file
  - HDP/HDP_Boot/Inc/stm32h7xx_hal_conf.h                             HAL configuration file
  - HDP/HDP_Boot/Inc/stm32h735g_discovery_conf                        board configuration file
  - HDP/HDP_Boot/Inc/stm32h7xx_it.h                                   Interrupt handlers header file
  - HDP/HDP_Boot/Inc/main.h                                           Header for main.c module
  - HDP/HDP_Boot/Inc/ft5336_conf.h                                    Configuration for the ft5336.c file
  - HDP/HDP_Boot/Inc/s70kl1281_conf.h                                 S70KL1281DABHI023 OctoSPI RAM memory configuration file
  - HDP/HDP_Boot/Inc/RSS.h                                            RSS public interface file
  - HDP/HDP_Boot/Inc/stlogo.h                                         ST Log BMP image 
  - HDP/HDP_Boot/Src/stm32h7xx_it.c                                   Interrupt handlers
  - HDP/HDP_Boot/Src/main.c                                           Main program
  - HDP/HDP_Boot/Src/system_stm32h7xx.c                               STM32H7xx system source file

@par Hardware and Software environment

  - This application runs on STM32H735xx devices

  - This application has been tested with STM32H735G-DK  board and can be
    easily tailored to any other supported device and development board.

  - At the very beginning, connect to the board using CubeProgrammer:
        1) Check the RDP option byte => should be set to AA
        2) Check the security bit => should be set to 0
        3) Remove the complete flash content

@par How to use it ?
In order to make the program work, you must do the following: 
 - Open your preferred toolchain
 - Rebuild HDP_Boot and HDP_Appli applications
 - Run the HDP_Boot application
 - Run the HDP_Appli application
 - Proceed with a power down reset, the application should start standalone.
 - Insure both applications are executed correctly according indications displayed on the LCD screen.
 - At the end of the HDP_Appli execution it will be required to connect to CubeProgrammer.
 - Follow instruction on the display.
 

 */
