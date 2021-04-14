;;;;;;;;;;;
;;; Top level boot loader
;;;



; BIOS loads our boot loader to 0x7c00
[org 0x7c00]
	; Memory offset to where we'll load our kernel
	KERNEL_OFFSET equ 0x1000

	; BIOS stores the boot drive in DL. Hold onto it
	mov [BOOT_DRIVE], dl

	; Someone set us up the stack
	mov bp, 0x9000									
	mov sp, bp

	; Tell user we're in 16 bit real mode
	mov bx, MSG_REAL_MODE						
	call print_string

	; Load kernel into memory
	mov bx, KERNEL_OFFSET			; TODO
	mov dl, [BOOT_DRIVE]			
	call read_kernel_from_disk

	; Enter protected mode - we'll end up in on_protected_mode_initialised afterwards
	call enter_protected_mode


;;; Includes
%include "print-string.asm"
%include "read-kernel-from-disk.asm"
%include "enter-protected-mode.asm"

;;; 32 bit stuff
[bits 32]

%include "print-string-protected.asm"

; Handler which is invoked after we enter 32 bit protected mode
on_protected_mode_initialised:
	; Tell user we're booting 
	mov ebx, MSG_BOOTING_KERNEL
	call print_string_protected

	; Jump to the address where we earlier loaded the kernel
	call KERNEL_OFFSET

	; Infinitely loop and loop infinitely.
	jmp $


;;; Globals

; First drive (e.g. BOCHS floppy)
BOOT_DRIVE db 0

MSG_REAL_MODE db "Started in 16-bit real mode", 13, 10, 0
MSG_BOOTING_KERNEL db "Booting into Royston Vasey kernel", 13, 10, 0

; Zerofill everything up until the last two bytes
times 510-($-$$) db 0

; Insert two byte MBR signature
dw 0xaa55