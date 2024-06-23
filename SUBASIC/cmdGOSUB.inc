;-----------------------------------------------------------------------------
cmdGOSUB:        .proc cmdGOSUB
;-----------------------------------------------------------------------------
;
; Go and execute the code in the specified location and remember the current
; location so that we can return to it later with a RETURN command
; 
; GOSUB 100
; GOSUB 100+A
; GOSUB A
;
;-------|-----------------------------------|-------------------------------------

        CALL ParseExpression                ; Get the target line number

        PUSH pCode                       ; Save the current location on the stack for
                                            ; the later RETURN command

        PUSH expression                     ; Find the target line number in the program
        CALL FindLine                       

        MOV  pFound pCode                ; And set the execution to continue from there
        JMP ProgramLine

        .endp
