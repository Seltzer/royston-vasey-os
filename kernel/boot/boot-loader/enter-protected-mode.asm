;;;;;;;;
;;;
;;; Enters protected mode and resets registers and stack afterwards. Invokes on_protected_mode_initialised handler at end
;;;

;;; Includes
%include "gdt.asm"


;;; 16 bit entry point
[bits 16]
enter_protected_mode:
	; Tell user we're entering protected mode
    push bx
	mov bx, MSG_ENTERING_PROTECTED_MODE
	call print_string
    pop bx

	; Clear interrupts
	cli

	; Load our GDT
	lgdt [gdt_descriptor]

	; Request switch by setting protected mode bit of cr0 control register 
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax

	; Execute far jump in order to flush CPU pipeline
	jmp CODE_SEG_OFFSET:init_protected_mode


;;; Post-switch 32 bit entry point
[bits 32]
init_protected_mode:
	; Reset segment registers
	mov ax, DATA_SEG_OFFSET
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; Reset stack
	mov ebp, 0x90000
	mov esp, ebp

	; Invoke handler
	call on_protected_mode_initialised


MSG_ENTERING_PROTECTED_MODE db "Entering 32 bit protected mode", 13, 10, 0