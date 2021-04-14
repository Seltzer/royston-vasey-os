;;;;;;;;
;;;
;;; Global Descriptor Table data structure
;;;


gdt_start:

; 8 byte null descriptor @ 0x00
gdt_null:	
	dd 0x0	
	dd 0x0

; Code segment descriptor (8 bytes total @ 0x08)
gdt_code_descriptor:	
	dw 0xffff			; Limit (0-15)
	dw 0x0				; Base (0-15)
	db 0x0				; Base (16-23)
	db 10011010b		; Segment present = 1 | DPL = 00 | Descriptor type = 1 | Code = 1 | Conforming = 0 | Readable = 1 | Accessed = 0
	db 11001111b		; Granularity = 1 | Default op size = 1 | 64 bit code segment = 0| Available for use by sys soft = 1 | Limit (16-19) = 1111
	db 0x0				; Base (24-31)

; Data segment descriptor (almost identical, 8 bytes @ 0x10)
gdt_data_descriptor:
	dw 0xffff			; Limit (0-15)
	dw 0x0				; Base (0-15)
	db 0x0				; Base (16-23)
	db 10010010b		; Segment present = 1 | DPL = 00 | Descriptor type = 1 | Code = 0 | Conforming = 0 | Readable = 1 | Accessed = 0
	db 11001111b		; Granularity = 1 | Default op size = 1 | 64 bit code segment = 0| Available for use by sys soft = 1 | Limit (16-19) = 1111
	db 0x0				; Base (24-31)


gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start - 1
	dd gdt_start

; Constants for segment offsets within GDT
CODE_SEG_OFFSET equ gdt_code_descriptor - gdt_start
DATA_SEG_OFFSET equ gdt_data_descriptor - gdt_start