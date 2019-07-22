
_STACKSIZE = 0x1000;
_HEAPSIZE = 0xC000;

MEMORY
{
    CODE (rx)   : ORIGIN = 0x08000000, LENGTH = 1024K
    RAM (rw)    : ORIGIN = 0x24000000, LENGTH = 512K
    QUADSPI (r) : ORIGIN = 0x90000000, LENGTH = 64M
    DTCM(rw)    : ORIGIN = 0x20000000, LENGTH = 128K
}

/* Section Definitions */

ENTRY(Reset_Handler)

SECTIONS
{
	/* first section is .text which is used for code */
	.text :
	{
		CREATE_OBJECT_SYMBOLS
		KEEP(*(.isr_vectors))
		*(.text .text.*)
		*(.gnu.linkonce.t.*)
		*(.glue_7t) *(.glue_7) *(.vfp11_veneer)
		KEEP(*(.fini))
		*(.gcc_except_table)
		. = ALIGN(0x4);
	} >CODE = 0
	. = ALIGN(4);

	/* .ctors .dtors are used for c++ constructors/destructors */
	
	.ctors :
	{
		PROVIDE(__ctors_start__ = .);
		KEEP(*(SORT(.ctors.*)))
		KEEP(*(.ctors))
		PROVIDE(__ctors_end__ = .);
	} >CODE

	.dtors :
	{
		PROVIDE(__dtors_start__ = .); 
		KEEP(*(SORT(.dtors.*)))
		KEEP(*(.dtors))
		PROVIDE(__dtors_end__ = .);
	} >CODE
	
	/* .rodata section which is used for read-only data (constants) */

	.rodata :
	{
		*(.rodata .rodata.*)
		*(.gnu.linkonce.r.*)
	} >CODE
	. = ALIGN(4);

	.init_array :
	{
		*(.init)
        *(.fini)
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
		PROVIDE_HIDDEN (__fini_array_end = .);
	} >CODE

	. = ALIGN(4);

	/* .ARM.exidx is sorted, so has to go in its own output section.  */
	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} >CODE
	__exidx_end = .;


 	_etext = .;
	PROVIDE (etext = .);

	.data : AT (_etext)
	{
		__data_start = .;
		*(.data .data.*)
		*(.gnu.linkonce.d.*)
		SORT(CONSTRUCTORS)
		. = ALIGN(4);
		*(.fastrun .fastrun.*)
	} >RAM
	. = ALIGN(4);
	
	_edata = .;
	PROVIDE (edata = .);

	/* .bss section which is used for uninitialized data */

	.bss :
	{
		__bss_start = .;
		__bss_start__ = .;
		*(.bss .bss.*)
		*(.gnu.linkonce.b.*)
		*(COMMON)
		. = ALIGN(4);
	} >RAM
	__bss_end__ = .;
	
	_end = .;
	PROVIDE(end = .);

	/* .heap section which is used for memory allocation */
	
	.heap (NOLOAD) :
	{
		__heap_start__ = .;
		*(.heap)
		. = MAX(__heap_start__ + _HEAPSIZE , .);
	} >RAM
	__heap_end__ = __heap_start__ + SIZEOF(.heap);
	
	/* .stack section - user mode stack */
	
	.stack (__heap_end__ + 3) / 4 * 4 (NOLOAD) :
	{
		. = ALIGN(8);
		__stack_start__ = .;
		*(.stack)
		. = ALIGN(8);
		. = MAX(__stack_start__ + _STACKSIZE , .);
	} >RAM
	__stack_end__ = __stack_start__ + SIZEOF(.stack);

	/* Stabs debugging sections.  */
	.stab          0 : { *(.stab) }
	.stabstr       0 : { *(.stabstr) }
	.stab.excl     0 : { *(.stab.excl) }
	.stab.exclstr  0 : { *(.stab.exclstr) }
	.stab.index    0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment       0 : { *(.comment) }
	/* DWARF debug sections.
		Symbols in the DWARF debugging sections are relative to the beginning
		of the section so we begin them at 0.  */
	/* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }	

	ExtFlashSection :
	{
		*(ExtFlashSection ExtFlashSection.*)
		*(.gnu.linkonce.r.*)
        . = ALIGN(0x4);
	} >QUADSPI

	CCMRAM :
	{
		*(CCMRAM CCMRAM.*)
        . = ALIGN(0x4);
	} >DTCM

	DTCMSection :
	{
		*(DTCMSection DTCMSection.*)
	. = ALIGN(0x4);
	} >DTCM
}

 __valid_user_code_checksum = 0 - (__stack_end__ + Reset_Handler + 1 + NMI_Handler + 1 + HardFault_Handler + 1 + MemManage_Handler + 1 + BusFault_Handler + 1 + UsageFault_Handler + 1);
