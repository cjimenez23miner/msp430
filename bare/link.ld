MEMORY {
	FLASH (rx) : ORIGIN = 0xf800, LENGTH = 2K
	RAM (xrw)	: ORIGIN = 0x200, LENGTH = 0x40
	STACK (xrw) : ORIGIN = 0x240, LENGTH = 0x40
	VECTOR (rx)	: ORIGIN = 0xffe0, LENGTH = 0x20
}

_stack_start = ORIGIN(STACK) + LENGTH(STACK);
_stack_end = ORIGIN(STACK);

SECTIONS {
	.data : {
		* (.data);
		* (.data*);
	} >RAM

	.bss : {
		_bss_start = .;
		* (.bss); 
		* (.bss*); 
		_bss_end = .;
	} >RAM

	.text : {
		. = ALIGN(2);
		* (.text); 
		* (.text*); 
		* (.rodata);
		* (.rodata*);
		. = ALIGN(2);
	} >FLASH

	".vectors" : { * (".vectors"); } >VECTOR
}

