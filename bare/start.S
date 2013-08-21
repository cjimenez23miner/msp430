#define PC	r0
#define SP	r1
#define SR	r2
#define CG1	r2
#define CG2	r3
	
	.text

RESET_ISR:
	mov #0x5A80, &0x120		; disable watchdog interrupt
	mov #_stack_start, SP	; setup stack

	;; Clear BSS
	mov #_bss_start, r4
_clear_bss:
	cmp #_bss_end, r4
	jeq _start
	clr	0(r4)
	add	#2, r4
	jmp _clear_bss

_start:
	call #msp_main

	.global __stop_progExec__
__stop_progExec__:
	jmp __stop_progExec__

UNEXPECTED_ISR:
	reti


; Assume MCLK = 21M
; The actual delay cycle is 21005x+14 where x is ms
	.global delay_ms
delay_ms:				; call 4 cycles
	push r14			; 3 cycles
_delay_ms_l1:			; outer loop: (21000+5)x cycles
	mov #7000, r14		; 2 cycles
_delay_ms_l2:			; inner loop: 7000*3 cycles
	dec r14				; 1 cycle			
	jnz _delay_ms_l2	; 2 cycles
	dec r15				; 1 cycle
	jnz _delay_ms_l1	; 2 cycles
	pop r14				; 3 cycles
	ret					; 4 cycles

; Actual delay cycle = 3x+8
	.global delay_cycle
delay_cycle:
	dec r15
	jnz delay_cycle
	ret

	.section ".vectors", "ax", @progbits
	.word UNEXPECTED_ISR    ;0xffe0 slot  0  0
	.word UNEXPECTED_ISR    ;0xffe2 slot  1  2
	.word UNEXPECTED_ISR    ;0xffe4 slot  2  4 (PORT1_VECTOR)
	.word UNEXPECTED_ISR    ;0xffe6 slot  3  6 (PORT2_VECTOR)
	.word UNEXPECTED_ISR    ;0xffe8 slot  4  8 
	.word UNEXPECTED_ISR    ;0xffea slot  5  A (ADC10_VECTOR)
	.word UNEXPECTED_ISR    ;0xffec slot  6  C (USCIAB0TX_VECTOR) 
	.word UNEXPECTED_ISR    ;0xffee slot  7  E (USCIAB0RX_VECTOR)
	.word UNEXPECTED_ISR    ;0xfff0 slot  8 10 (TIMER0_A1_VECTOR)
	.word UNEXPECTED_ISR    ;0xfff2 slot  9 12 (TIMER0_A0_VECTOR)
	.word UNEXPECTED_ISR    ;0xfff4 slot 10 14 (WDT_VECTOR)
	.word UNEXPECTED_ISR    ;0xfff6 slot 11 16 (COMPARATORA_VECTOR)
	.word UNEXPECTED_ISR    ;0xfff8 slot 12 18 (TIMER1_A1_VECTOR)
	.word UNEXPECTED_ISR    ;0xfffa slot 13 1a (TIMER1_A0_VECTOR)
	.word UNEXPECTED_ISR    ;0xfffc slot 14 1c (NMI_VECTOR)
	.word RESET_ISR         ;0xfffe slot 15 1e (RESET_VECTOR)
	.end

