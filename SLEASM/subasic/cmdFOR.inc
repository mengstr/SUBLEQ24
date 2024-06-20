;-----------------------------------------------------------------------------
cmdFOR:        .proc cmdFOR
;-----------------------------------------------------------------------------
;
; Implement a FOR-loop
; 
; FOR A = 1 TO 10
;
;-------|-----------------------------------|-------------------------------------

;        OUT ASCII_AT       ;debug

        CALL GetToken                           ; Get the variable name
        CALL ParseToken
        MOV tokenAddr @var1

;        OUT ASCII_AT       ;debug

        CALL GetToken                           ; Get the equals sign
        CALL ParseToken
        MOV tokenAddr @var2

;        OUT ASCII_AT       ;debug

        CALL ParseExpression                      ; Get the start value
        MOV expression @var3

;        OUT ASCII_AT       ;debug

        CALL GetToken                           ; Get the TO keyword
        CALL ParseToken
        MOV tokenAddr @var4

;        OUT ASCII_AT       ;debug

        CALL ParseExpression                      ; Get the end value
        MOV expression @var5

;        OUT ASCII_AT       ;debug

        ; CALL GetToken                           ; Get the STEP keyword
        ; CALL ParseToken
        ; MOV tokenAddr @var6

        ; CALL ParseExpression                      ; Get the step value
        ; MOV expression @var7


;        OUT ASCII_V       ;debug
;        OUT ASCII_a       ;debug
;        OUT ASCII_r       ;debug
;        OUT ASCII_EQUAL       ;debug
;        PUSH @var1       ;debug
;        CALL PrintInteger       ;debug
;        OUT ASCII_SPACE       ;debug

;        OUT ASCII_S        ;debug
;        OUT ASCII_t        ;debug
;        OUT ASCII_a        ;debug
;        OUT ASCII_EQUAL        ;debug
;        PUSH @var3        ;debug
;        CALL PrintInteger        ;debug
;        OUT ASCII_SPACE        ;debug

;        OUT ASCII_E        ;debug
;        OUT ASCII_n        ;debug
;        OUT ASCII_d        ;debug
;        OUT ASCII_EQUAL        ;debug
;        PUSH @var5        ;debug
;        CALL PrintInteger        ;debug
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


