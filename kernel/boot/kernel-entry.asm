;;;;;;;;
;;;
;;; Kernel entry point - this gets shoved at the start of our kernel image
;;;


[bits 32]
[extern _start]

call _start
jmp $