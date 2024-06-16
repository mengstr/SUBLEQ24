;-----------------------------------------------------------------------------
; PARSEEXPRESSION.inc
;-----------------------------------------------------------------------------

    .proc ParseExpression


expression:     .data 0
@op:            .data 0

ParseExpression:
        CLR expression
        CLR @op
@loop:
        CALL GetToken
        CALL ParseToken
        CMPEQ tokenType CONST_1 @isDone
        CMPEQ tokenType CONST_5 @isStr
        CMPEQ tokenType CONST_4 @isOp
        CMPEQ tokenType CONST_6 @isNum
        CMPEQ tokenType CONST_7 @isVar
        JMP Syntax

@isDone:
        RET

@isStr:
;        PUSH token1
;        CALL PrintString
        RET

@isOp:
        MOV tokenValue @op
        JMP @loop

@isNum:
@isVar:
        ADD tokenValue expression
        CLR @op
        JMP @loop


    .endp
