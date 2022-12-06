/*
******************************************************************************
**

**  File        : LinkerScript.ld
**
**
**  Abstract    : Linker script for STM32H7 series
**                1024Kbytes FLASH and 192Kbytes RAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**  Distribution: The file is distributed as is, without any warranty
**                of any kind.
**
*****************************************************************************
** @attention
**
** Copyright (c) 2019 STMicroelectronics.
** All rights reserved.
**
** This software is licensed under terms that can be found in the LICENSE file
** in the root directory of this software component.
** If no LICENSE file comes with this software, it is provided AS-IS.
**
******************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = 0x20020000;    /* end of RAM */
/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x1000   /*  4 Kb */;      /* required amount of heap  */
_Min_Stack_Size = 0x2000   /*  8 Kb */; /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
FLASH_STEW (rx)      : ORIGIN = 0x08000000, LENGTH = 4K
FLASH_STEW_2 (rx)    : ORIGIN = 0x08041000, LENGTH = 437K
xtx_ctrl (xrw)       : ORIGIN = 0x10040000, LENGTH = 0x04
xtx_data (xrw)       : ORIGIN = 0x10040004, LENGTH = 0x04
xrx_ctrl (xrw)       : ORIGIN = 0x10040008, LENGTH = 0x04
xrx_data (xrw)       : ORIGIN = 0x1004000c, LENGTH = 0x04
DTCMRAM (xrw)        : ORIGIN = 0x20000000, LENGTH = 128K
RAM (xrw)            : ORIGIN = 0x24000000, LENGTH = 512K
SDRAM (xrw)          : ORIGIN = 0xD0000000, LENGTH = 32768K
QSPI_STEW (rx)       : ORIGIN = 0x90800000, LENGTH = 30720K
}

/* Define output sections */
SECTIONS
{
  /* The startup code goes first into FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASH_STEW

  _siQSPI = LOADADDR(.QSPISection);

  .ExtThirdPartyHeaderSection : 
   {   . = ALIGN(4);
   KEEP(*(.ExtThirdPartyHeaderSection)) 
  . = ALIGN(4);
  } >QSPI_STEW

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .QSPISection : {
    . = ALIGN(4);
    _sQSPI = .;        /* create a global symbol at data start */
    
   *ST_Fonts.o (.rodata .rodata*)
   *FComic24B_ASCII.o (.rodata .rodata*)
   *F13B_1.o (.rodata .rodata*)
   *F13B_ASCII.o (.rodata .rodata*)
   *F13_ASCII.o (.rodata .rodata*)
   *F13_1.o (.rodata .rodata*)
   *F13_ASCII.o (.rodata .rodata*)
   *F16B_1.o (.rodata .rodata*)
   *F16B_ASCII.o (.rodata .rodata*)
   *F16_ASCII.o (.rodata .rodata*)
   *F20B_1.o (.rodata .rodata*)
   *F20B_ASCII.o (.rodata .rodata*)
   *F32B_1.o (.rodata .rodata*)
   *F32B_ASCII.o (.rodata .rodata*)
   *Arial32.o (.rodata .rodata*)
    _eQSPI = .;          
 } >QSPI_STEW 
 
  .IntThirdPartyHeaderSection : 
   {   . = ALIGN(4);
   KEEP(*(.IntThirdPartyHeaderSection)) 
       . = ALIGN(4);
  } >FLASH_STEW_2
  
  /* The program code and other data goes into FLASH */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH_STEW_2
  
  /* Constant data goes into FLASH */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >FLASH_STEW_2

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH_STEW_2
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH_STEW_2

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH_STEW_2
  
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH_STEW_2
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH_STEW_2

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data : 
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >DTCMRAM AT> FLASH_STEW_2

  
  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >DTCMRAM

  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM

  demo_icon_section :
  {
    demo_icon = .;
    KEEP(*(demo_icon_section));
    demo_icon_end = .;
  } >QSPI_STEW
 
  demo_logo_section :
  {
    demo_logo = .;
    KEEP(*(demo_logo_section ));
    demo_logo_end = .;
  } >QSPI_STEW

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
  xtx_ctrl_Section : { *(xtx_ctrl_Section) } >xtx_ctrl
  xtx_data_Section : { *(xtx_data_Section) } >xtx_data
  xrx_ctrl_Section : { *(xrx_ctrl_Section) } >xrx_ctrl
  xrx_data_Section : { *(xrx_data_Section) } >xrx_data
  heap_mem  : { *(heap_mem) } >RAM
  .sdio_heap  : { *(.sdio_heap) } >RAM
  .STemWinMemPool (NOLOAD) : { *(.STemWinMemPool) } >SDRAM
  .framebuffer  (NOLOAD) : { *(.framebuffer) } >SDRAM
  .audio_buffers  : { *(.audio_buffers) } >RAM
  .avi_video_buffers : { *(.avi_video_buffers) } >RAM
  .jpeg_decoder_buffers  : { *(.jpeg_decoder_buffers) } >RAM
  .rgb_video_buffers (NOLOAD) : { *(.rgb_video_buffers) } >SDRAM
  .gfx_buffers  (NOLOAD) : { *(.gfx_buffers) } >SDRAM
  .viz_front_buffer  (NOLOAD) : { *(.viz_front_buffer) } >SDRAM
  .viz_back_buffer (NOLOAD) : { *(.viz_back_buffer) } >SDRAM
  .viz_bitmap_buffer  (NOLOAD) : { *(.viz_bitmap_buffer) } >SDRAM
  .viz_deform_x_buffer  (NOLOAD) : { *(.viz_deform_x_buffer) } >SDRAM
  .viz_deform_y_buffer  (NOLOAD) : { *(.viz_deform_y_buffer) } >SDRAM
  .ExtQSPIFlashSection  : { *(.ExtQSPIFlashSection) } >QSPI_STEW
}
