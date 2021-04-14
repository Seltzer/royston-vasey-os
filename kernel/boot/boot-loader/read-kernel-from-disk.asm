;;;;;;;;
;;;
;;; Reads kernel (up to 15 sectors) to ES:BX from drive DL
;;;


read_kernel_from_disk:
    ; Tell user we're loading the kernel
    push bx
	mov bx, MSG_LOAD_KERNEL
	call print_string
    pop bx

    ; Prepare to read from disk
    ; int 13H | AH=0x02 => Read sectors from drive
    mov ah, 0x02                
    mov al, [KERNEL_LENGTH] 

    ; First sector is the boot loader, the kernel is contiguously stored from 0/0/2
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02            

    ; Commence, and check for errors
    int 0x13
    jc disk_error

    ; Compare #requested sectors with #read
    
    cmp [KERNEL_LENGTH], al     
    jne disk_error
    
    ; Return
    ret                         

disk_error:
    mov bx, MSG_DISK_ERROR
    call print_string
    jmp $


KERNEL_LENGTH: db 15
MSG_LOAD_KERNEL db "Loading kernel into memory", 13, 10, 0
MSG_DISK_ERROR: db "Disk error!", 0

