;-----------------------------------------------------------------------------
cmdRETURN:        .proc cmdRETURN
;-----------------------------------------------------------------------------
;
; Return from a GOSUB subroutine call and continue execution from the previous location
; 
; RETURN
;
;-------|-----------------------------------|-------------------------------------

        POP pProgram                        ; Restore the previous location from the stack  
        JMP Statement                       ; And continue execution from there

        .endp
