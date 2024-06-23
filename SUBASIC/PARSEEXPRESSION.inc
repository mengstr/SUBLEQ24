;-----------------------------------------------------------------------------
; PARSEEXPRESSION.inc
;-----------------------------------------------------------------------------

    .proc ParseExpression


expression:     .data 0
@op:            .data 0

ParseExpression:
;        DEBUGPRINT "ParseExpression\r\n" ;debug
        CLR expression
        CLR @op

@loop:
        JBETWEEN tokenPeek ASCII_A ASCII_Z @peekOk
        JBETWEEN tokenPeek ASCII_z ASCII_z @peekOk
        JBETWEEN tokenPeek ASCII_0 ASCII_9 @peekOk
        CMPEQ tokenPeek ASCII_DOUBLEQUOTE @peekOk
        CMPEQ tokenPeek ASCII_PLUS @peekOk
        CMPEQ tokenPeek ASCII_MINUS @peekOk
        CMPEQ tokenPeek ASCII_ASTERISK @peekOk
        CMPEQ tokenPeek ASCII_SLASH @peekOk
        RET

@peekOk:
        MOV pCode @pCodeHold

        CALL GetParsedToken
        CMPEQ tokenType CONST_3 @isKeyword

;       CALL debugToken                 ;debug
;        PRINTCRLF                      ;debug;

        CMPEQ tokenType CONST_1 @isNum          ; Number
        CMPEQ tokenType CONST_2 @isVar          ; Variable
        CMPEQ tokenType CONST_5 @isOp           ; Operand
        RET

@isKeyword:
        MOV @pCodeHold pCode                    ; If the expression is terminated by a keyword we need to
        RET                                     ; roll back that keyword so it can be processsed properly.


@isOp:
        CMPEQ tokenValue ASCII_PLUS @opOk
        CMPEQ tokenValue ASCII_MINUS @opOk
        CMPEQ tokenValue ASCII_ASTERISK @opOk
        CMPEQ tokenValue ASCII_SLASH @opOk
        JMP @loop
@opOk:
        MOV tokenValue @op
        JMP @loop

@isNum:
@isVar:
        CMPEQ @op ASCII_PLUS @addition
        CMPEQ @op ASCII_MINUS @subtraction
        CMPEQ @op ASCII_ASTERISK @multiplication
        CMPEQ @op ASCII_SLASH @division
        MOV tokenValue expression
        JMP @loop

@addition
        ADD tokenValue expression
        CLR @op
        JMP @loop

@multiplication:
        MULU tokenValue expression
        CLR @op
        JMP @loop

@division
        DIVU tokenValue expression
        CLR @op
        JMP @loop

@subtraction:
        SUB tokenValue expression
        CLR @op
        JMP @loop

@pCodeHold: .data 0                     ; To back up to the previous token when required

    .endp
