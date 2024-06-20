;-----------------------------------------------------------------------------
; STACK.inc
;-----------------------------------------------------------------------------

SP:         .data 0                                 ; Stackpointer

stack_:     .data $+1                               ; Address to the stack
stack:      .org $+256                              ;The stack itself - 256 entries long

