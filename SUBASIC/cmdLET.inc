;-----------------------------------------------------------------------------
cmdLET:        .proc cmdLET
;-----------------------------------------------------------------------------
;
; Assign an expression to a variable
; 
; LET A = 1
; LET A = A + 1
;
;-------|-----------------------------------|-------------------------------------

        DEBUGPRINT "cmdLET:\r\n"

        CALL GetParsedToken                 ; Get the variable name
        MOV tokenAddr @dest

        CALL GetParsedToken                 ; Get the equals sign

        CALL ParseExpression                ; Get the value expression
        INDEXEDWR expression @dest          ; Store the value in the variable

        JMP Statement

@dest:  .data 0

        .endp
