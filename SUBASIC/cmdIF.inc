;-----------------------------------------------------------------------------
cmdIF:        .proc cmdIF
;-----------------------------------------------------------------------------
;
; Execute statements conditionally
; 
; IF A>5 THEN PRINT "A is larger"
;
;-------|-----------------------------------|-------------------------------------

       CALL ParseExpression                             ; First value
       MOV expression @var1

       CALL GetParsedToken                              ; comparison token
       CMPNE tokenType CONST_5 SyntaxError
       MOV tokenValue @cmp
       CMPNE tokenValue ASCII_LESS @notCompound
       CMPNE tokenPeek ASCII_GREATER @notCompound
       CALL GetParsedToken                              ; compound comparison token
       MOV ASCII_DOUBLEDAGGER @cmp

@notCompound:
       CALL ParseExpression                             ; second value
       MOV expression @var2

       CMPNE @cmp ASCII_EQUAL @notEq
       CMPEQ @var1 @var2 @true
       JMP @skip

@notEq:CMPNE @cmp ASCII_GREATER @notGt
       JGT @var1 @var2 @true
       JMP @skip

@notGt:CMPNE @cmp ASCII_LESS @notLe
       JLT @var1 @var2 @true
       JMP @skip

@notLe:CMPNE @cmp ASCII_DOUBLEDAGGER @notDi
       CMPNE @var1 @var2 @true
       JMP @skip

@notDi: JMP SyntaxError


@true:
       CALL GetParsedToken                       ; THEN
       JMP Statement                             ; Execute the statements on the rest of the line

@skip:
       MOV addrNextLine pCode                    ; Comparison is false, skip to the next line
       JMP ProgramLine


@var1:        .data 0
@var2:        .data 0
@cmp:         .data 0

        .endp
