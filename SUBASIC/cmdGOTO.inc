;-----------------------------------------------------------------------------
cmdGOTO:        .proc cmdGOTO
;-----------------------------------------------------------------------------
;
; Goto the specified location and continue execution from there
; 
; GOTO 100
; GOTO 100+A
; GOTO A
;
;-------|-----------------------------------|-------------------------------------

        CALL ParseExpression                ; Get the target line number

        PUSH expression                     ; Find the target line number in the program
        CALL FindLine                       

        MOV  pFound pCode                ; And set the execution to continue from there
        JMP ProgramLine

        .endp
