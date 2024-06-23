;-----------------------------------------------------------------------------
cmdFOR:        .proc cmdFOR
;-----------------------------------------------------------------------------
;
; Implement a FOR-loop
; 
; FOR A = 1 TO 10
;
;-------|-----------------------------------|-------------------------------------


        CALL GetParsedToken                       ; Get the variable name
        MOV tokenAddr @var1

        CALL GetParsedToken                       ; Get the equals sign
        MOV tokenAddr @var2

        CALL ParseExpression                      ; Get the start value
        MOV expression @var3

        INDEXEDWR @var3 @var1                     ; Store the start value in the variable

        CALL GetParsedToken                       ; Get the TO keyword
        MOV tokenAddr @var4

        CALL ParseExpression                      ; Get the end value
        MOV expression @var5

        PUSH @var1                                ; Push the variable address
        PUSH @var5                                ; Push the end value
        PUSH pCode                                ; Push the loop address 

        JMP Statement

@var1:  .data 0
@var2:  .data 0
@var3:  .data 0
@var4:  .data 0
@var5:  .data 0
@var6:  .data 0
@var7:  .data 0

        .endp


