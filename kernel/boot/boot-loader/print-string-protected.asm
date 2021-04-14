;;;;;;;;
;;;
;;; Prints string at [ebx]
;;;


[bits 32]

print_string_protected:
	pusha
	mov edx, 0xb8000			; Video memory address

print_string_protected_loop:
	; Load word into eax 
	mov al, [ebx]				; Low byte = char		
	mov ah, 0x0f				; High byte = white text on black background

	; If this char is a terminator, exit
	cmp al, 0					
	je print_string_protected_exit

	; Copy word to edx
	mov [edx], ax

	; Increment input string pointer and output video pointer
	add ebx, 1
	add edx, 2

	jmp print_string_protected_loop

print_string_protected_exit:
	popa
	ret