;-----------------------------------------------------------------------------
cmdIF:        .proc cmdIF
;-----------------------------------------------------------------------------
;
; Execute statements conditionally
; 
; IF A=1 THEN PRINT "A is 1"
;
;-------|-----------------------------------|-------------------------------------

;        OUT ASCII_AT           ;debug
;        CALL ParseExpression
;        MOV expression @var1

;        OUT ASCII_AT           ;debug
;        CALL GetToken
;        CALL ParseToken

;        OUT ASCII_AT           ;debug
;        CALL ParseExpression
;        MOV expression @var2

;        OUT ASCII_AT           ;debug
;        PUSH @var1        ;debug
;        CALL PrintInteger        ;debug
;        OUT ASCII_SPACE        ;debug
;        PUSH @var2        ;debug
;        CALL PrintInteger        ;debug
;        PRINTCRLF        ;debug

       JMP ProgramLine

@var1:  .data 0
@var2:  .data 0


        .endp
