;-----------------------------------------------------------------------------
cmdNEXT:        .proc cmdNEXT
;-----------------------------------------------------------------------------
;
; Loops back to the start of the current FOR loop
;
;-------|-----------------------------------|-------------------------------------

        CALL GetToken                           ; Get the variable name
        CALL ParseToken
        MOV tokenAddr @var1

;        OUT ASCII_N        ;debug
;        OUT ASCII_E        ;debug
;        OUT ASCII_X        ;debug
;        OUT ASCII_T        ;debug
;        OUT ASCII_SPACE        ;debug

        JMP ProgramLine

@var1:  .data 0
@var2:  .data 0
@var3:  .data 0
@var4:  .data 0
@var5:  .data 0
@var6:  .data 0
@var7:  .data 0

    .endp


