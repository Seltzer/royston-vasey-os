;;;;;;;;
;;;
;;; Prints string at [bx] in teletype mode
;;;


print_string:
    pusha
    mov ah, 0x0e            ; int 10H | AH=0e => BIOS teletype

loop:
    ; Dereference bx and print it
    mov al, [bx]
    
    ; If this char is a terminator, exit
    cmp al, 0
    je exit
        
    ; Print character and increment pointer
    int 0x10
    add bx, 1

    jmp loop
       
exit:
    ; Restore register and return
    popa
    ret