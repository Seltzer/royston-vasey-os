bits 32

; Declare globals and externs
global load_idt
global noop_handler

[extern handle_interrupt] 


; Define macro for generating an interrupt handler

%macro gen_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword 0                         ; Error code
    push dword %1                     
    jmp generic_interrupt_handler   
%endmacro


; Generate interrupt handler functions
gen_interrupt_handler 0x20       
gen_interrupt_handler 0x21       



generic_interrupt_handler:               
    ; Save everything except CS, IP and flags
    push esp
    push ebp
    push esi
    push edi
    push edx
    push ecx
    push ebx
    push eax
    
    ; Invoke interrupt handler defined in C
    call handle_interrupt
    
    ; Restore everything
    pop eax
    pop ebx
    pop ecx
    pop edx
    pop edi
    pop esi
    pop ebp
    pop esp

    ; Return to generated handler
    add esp, 8
    iret


; We map all interrupts we don't care about to this
noop_handler:
    iret



load_idt:
    ; Grab pointer to IDT (passed in from C code) and invoke lidt
	mov edx, [esp + 4]
	lidt [edx]
	
    ; Enable interrupts
    sti 				

	ret

