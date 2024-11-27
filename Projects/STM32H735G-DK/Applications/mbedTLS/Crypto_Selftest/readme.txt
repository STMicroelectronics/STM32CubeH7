/**
  @page Crypto_Selftest Selftest Application

  @verbatim
  ******************************************************************************
  * @file    mbedTLS/Crypto_Selftest/readme.txt
  * @author  MCD Application Team
  * @brief   This application runs crypto through mbed TLS selftest functions.
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

This application implements on STM32H735G-DK board a set of cryptographic features
through mbed TLS selftest functions of individual mbed TLS components selectively
chosen in a single configuration file "mbedtls_config.h".

This application contains in particular proprietary implementations (xxx_alt.c
alternate files) with adaptation layers (HAL API) which activates the
crypto engines (processors or accelerators available in the HW) from mbed TLS.
Following IPs are executed :
- RNG : True Random Number Generator
- CRYP : Cryptographic processor
- HASH : Hash processor

Log messages will be displayed by default on LCD to inform user about all cryptographic
algorithms which are executed according to the mbed TLS configuration.

Logs messages can be displayed on the HyperTerminal instead of using LCD.
(USE_LCD must be removed from the project defines, within compiler preprocessor options).
In this case, UART must be configured with the required settings (to be done only once):
   - When the board is connected to a PC,
     open a serial terminal emulator, find the board's COM port and configure it with:
      - 8N1, 9600 bauds, no HW flow control
      - set the line endings to LF or CR-LF (Transmit) and LF (receive).

Executed tests may be passed or failed (All tests pass in this version).

Currently, there are 7 test suites by default covering the following features :
MD5, SHA1, SHA256 (including SHA224),
AES (including ECB, CBC, CFB, OFB, CTR, XTS mode of operations),
CCM,
ENTROPY, TIMING

In case of success the green led (LED1) is toggling at the end of execution.
In case of errors the red led (LED2) is toggling.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.


@par mbed TLS Library Configuration

Mbed TLS can be built with a larger variety of features so that more test suites can be run.
User can customize the set of features and trigger more tests by enabling more features in the
configuration file "mbedtls_config.h" available under the project includes directory.


@par Keywords

Security, mbedTLS, RNG, CRYP, HASH, cryptography, crypto processor,
symmetric algorithms, AES, modes of operation, ECB, CBC, CFB, OFB, CTR, XTS,
hash algorithms, sha1, sha256, Message Digest, MD5,
True Random number generator


@par Directory contents

Inc
  - mbedTLS/Crypto_Selftest/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - mbedTLS/Crypto_Selftest/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - mbedTLS/Crypto_Selftest/Inc/main.h                        Main program header file
  - mbedTLS/Crypto_Selftest/Inc/utilities_conf.h              Utilities configuration file
  - mbedTLS/Crypto_Selftest/Inc/ft5336_conf.h                 FT5336 component configuration file
  - mbedTLS/Crypto_Selftest/Inc/mx25lm51245g.h                MX25LM51245G component configuration file
  - mbedTLS/Crypto_Selftest/Inc/s70kl1281_conf.h              S70kl1281 component configuration file
  - mbedTLS/Crypto_Selftest/Inc/stm32h735g_discovery_conf.h    stm32h735g_discovery configuration file
  - mbedTLS/Crypto_Selftest/Inc/lcd_trace.h                   LCD header file
  - mbedTLS/Crypto_Selftest/Inc/mbedtls_config.h              mbedTLS library configuration options
  - mbedTLS/Crypto_Selftest/Inc/FreeRTOSConfig.h              FreeRTOS configuration options
  - mbedTLS/Crypto_Selftest/Inc/aes_alt.h                     AES HW implementation header file
  - mbedTLS/Crypto_Selftest/Inc/ccm_alt.h                     CCM HW implementation header file
  - mbedTLS/Crypto_Selftest/Src/cryp_stm32.h                  CRYP HW common header file
  - mbedTLS/Crypto_Selftest/Inc/sha1_alt.h                    SHA1 HW implementation header file
  - mbedTLS/Crypto_Selftest/Inc/sha256_alt.h                  SHA224/256 HW implementation header file
  - mbedTLS/Crypto_Selftest/Inc/md5_alt.h                     MD5 HW implementation header file
  - mbedTLS/Crypto_Selftest/Src/hash_stm32.h                  HASH HW common header file
  - mbedTLS/Crypto_Selftest/Inc/timing.h                      timing HW implementation header file

Src
  - mbedTLS/Crypto_Selftest/Src/stm32h7xx_it.c                 Interrupt handlers
  - mbedTLS/Crypto_Selftest/Src/stm32h7xx_hal_timebase_tim.c   HAL time base functions
  - mbedTLS/Crypto_Selftest/Src/main.c                               Main program
  - mbedTLS/Crypto_Selftest/Src/mbedtls_selftest.c                   mbed TLS selftest crypto thread
  - mbedTLS/Crypto_Selftest/Src/stm32h7xx_hal_msp.c            HAL MSP module
  - mbedTLS/Crypto_Selftest/Src/system_stm32h7xx.c             STM32H7xx system source file
  - mbedTLS/Crypto_Selftest/Src/aes_alt.c                            AES HW implementation file
  - mbedTLS/Crypto_Selftest/Src/ccm_alt.c                            CCM HW implementation file
  - mbedTLS/Crypto_Selftest/Src/cryp_stm32.c                         CRYP HW common file
  - mbedTLS/Crypto_Selftest/Src/sha1_alt.c                           SHA1 HW implementation file
  - mbedTLS/Crypto_Selftest/Src/sha256_alt.c                         SHA224/256 HW implementation file
  - mbedTLS/Crypto_Selftest/Src/md5_alt.c                            MD5 HW implementation file
  - mbedTLS/Crypto_Selftest/Src/hash_stm32.c                         HASH HW common file
  - mbedTLS/Crypto_Selftest/Src/timing_alt.c                         timing HW implementation file
  - mbedTLS/Crypto_Selftest/Src/hardware_rng.c                       RNG HW implementation file

@par Hardware and Software environment
  - This example runs on STM32H735IG devices.

  - This example has been tested with STM32H735G-DK board and can be
    easily tailored to any other supported device and development board with security.

  - STM32H735G-DK Set-up:
    - Connect ST-Link cable to the PC USB port to display data on the HyperTerminal.
      A virtual COM port will then appear in the HyperTerminal.

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Optionally, select the destination of the log messages (LCD or Hyperterminal)
 - Optionally, disable/enable others mbed TLS components
 - Rebuild all files and load your image into target memory

    - Connect the STM32H735G-DK board to the PC through 'USB micro B-Male
      to A-Male' cable to the connector:
      - CN15: to use ST-LINK

 - Run the example

 @par License ?
- Question:
This application is delivered under ST Ultimate Liberty (SLA0044) license
that is basically an ST proprietary license. However, several files
delivered in this component has an Apache 2.0 license header most often
with an ARM copyright declaration. Why? What license terms do actually apply
for those files and more generally for this application ?

- Answer:
This application is based on the Open Source mbed TLS reference implementation
that has been ported on STMH7 microcontroller.

This mbed TLS reference is licensed under Apache 2.0 license.
Apache 2.0 license authorizes that “Derivative Works as a whole” using Apache 2.0
code to be delivered with another license provided compatibility is respected.
This is the case as what is delivered her under SLA0044 license.
Particularly, Apache 2.0 license requires that:
1.	We give a copy of this License: this is available in the
    APACHE-2.0.txt file at the root of the mbed TLS component; and
2.	We cause any modified files to carry prominent notices stating that we
    changed the files: these are the files modified by us with the addition of
	STMicroelectronics copyright to the original ARM Limited copyright; and
3.	We retain, in the Source form that we distribute, all copyright and
    attribution notices from the original Source form of the Work: this is the
	reason why ARM Limited original copyright declarations and license heading
	have been retained; and
4.	Any Derivative Works that we distribute must include a readable copy of the
    attribution notices contained within original work, in at least one of
	three possible places (within a NOTICE text file distributed as part of the
	Derivative Works, within the Source form or documentation or within a
	display generated by the Derivative Works): we deliver to you the readable
	copy of the attribution notices within the Source form.

As a conclusion, we confirm that this application is licensed under SLA0044
license terms and that they use parts of ARM copyrighted components originally
licensed under Apache 2.0 license. Respecting the original Apache 2.0 license
terms obliges us to retain ARM copyright declarations as well as Apache 2.0
file headers.


 */