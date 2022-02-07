/**
  @page Embedded Wizard Demonstration
  
  @verbatim
  ******************** COPYRIGHT (C) 2017 TARA Systems GmbH ********************
  * @file    readme.txt 
  * @author  Embedded Wizard Team
  * @brief   Description of the MasterDemo GUI example application.
  ******************************************************************************
  *
  * BY INSTALLING, COPYING, DOWNLOADING, ACCESSING OR OTHERWISE USING THIS
  * SOFTWARE OR ANY PART THEREOF (AND THE RELATED DOCUMENTATION) FROM
  * STMICROELECTRONICS INTERNATIONAL N.V., SWISS BRANCH AND/OR ITS AFFILIATED
  * COMPANIES (STMICROELECTRONICS), YOU, ON BEHALF OF YOURSELF, OR ON BEHALF OF
  * ANY ENTITY BY WHICH YOU ARE EMPLOYED AND/OR ENGAGED AGREE TO BE BOUND BY THE
  * TERMS OF THE ST-EMBEDDEDWIZARD LICENSE, WHICH YOU MAY DOWNLOAD, REVIEW, AND
  * PRINT FROM HERE: <www.st.com/st-embeddedwizard_license>.
  *
  * You may not use this Software except in compliance with the
  * ST-EmbeddedWizard License. In addition to the other applicable terms, you
  * may not use the software for production purposes, except in connection with
  * the list of Dedicated Parts provided here:
  * <www.st.com/st-embeddedwizard_license>.
  *
  * If you are agreeing to the ST-EmbeddedWizard License on behalf of an entity,
  * you represent that you have the authority to bind the entity to the terms of
  * the ST-EmbeddedWizard License.
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the ST-EmbeddedWizard License is distributed on an "AS IS"
  * BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  * implied.
  *
  * See the ST-EmbeddedWizard License for the specific language governing
  * permissions and limitations applicable to this software.
  *
  ******************************************************************************
   @endverbatim


@par Description

This application showcases a GUI created with Embedded Wizard using
STMicroelectronics' graphic accelerator DMA2D.

Further information about the GUI application itself can be found as in-line
annotations within the GUI project. Therefore please open the project
  EmbeddedWizard/EmWiProject/MasterDemo.ewp
with the Embedded Wizard Studio development environment.


@par How to use it?

In order to make the program work, you must do the following :
 - Open your preferred toolchain and rebuild all files
 - Once the application built, please use STM32CubeProgrammer to flash the 
   corresponding .HEX file

To open the GUI itself, please ensure that you have installed a priori the
 - Embedded Wizard Studio and
 - Embedded Wizard Platform Package for STM32
on your Windows host PC. Therefore please install both components by executing
'EmbeddedWizardSTM32.msi', which is found within the directory
'/Middlewares/ST/ST-EmbeddedWizard/Software'.


This combined application is too complex. Hence it is not possible to save your
project or to re-generate the target specific source code within the Embedded
Wizard Studio. Please have a look at the provided applications instead.


@par Getting started with Embedded Wizard

In order to get familiar with Embedded Wizard and the UI development
work-flow, we highly recommend to study our online documentation:

  http://doc.embedded-wizard.de/welcome-to-embedded-wizard
  http://doc.embedded-wizard.de/basic-concepts
  http://doc.embedded-wizard.de/working-with-embedded-wizard

Furthermore, we have collected many 'Questions and Answers' covering
typical Embedded Wizard programming aspects. Please visit our community:

  http://ask.embedded-wizard.de

Please use this platform to drop your questions, answers and ideas.


@par Simulation

Please open the 'Simulation/index.html' web application with a WebGL capable
desktop browser in order to simulate the target GUI application on your host PC.
Please follow the instructions as stated there.


@par Hardware and Software environment
 
This example has been tested with STMicroelectronics' STM32H747I-DISCO
platform and can be easily tailored to any other supported device and
development board.

This GUI application has been developed and tested with Embedded Wizard 8.30.


@par Keywords

GUI, HMI, Display, Graphics, Chrom-ART, DMA2D, RGB565, LCD, RGB, Embedded Wizard


@par Directory contents

  - Config           configuration files
  - Core             Main program
  - EmbeddedWizard   GUI project, generated code and target specific 'glue' layer
  - Simulation       Simulation as web application


@note 3rd party software

The following 3rd party software components are used:

1. Two Level Segregated Fit memory allocator, version 3.0.
   Written by Matthew Conte, and placed in the Public Domain.
   http://tlsf.baisoku.org

2. Universal string handler

/*------------------------------------------------------------------------/
/  Universal string handler for user console interface
/-------------------------------------------------------------------------/
/
/  Copyright (C) 2011, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------*/



 */