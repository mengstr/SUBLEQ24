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

        CALL GetToken                           ; Get the variable name
;;        CALL debugShowToken  ;debug

        CALL ParseToken
;;        CALL debugShowParsedToken  ;debug

        MOV tokenAddr @dest

        CALL GetToken                           ; Get the equals sign
;;        CALL debugShowToken  ;debug

        CALL ParseToken
;;        CALL debugShowParsedToken  ;debug

         CALL ParseExpression                   ; Get the value expression
         INDEXEDWR expression @dest             ; Store the value in the variable

         JMP Statement

@dest:  .data 0

        .endp
