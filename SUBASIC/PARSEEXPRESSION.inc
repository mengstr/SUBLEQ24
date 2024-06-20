;-----------------------------------------------------------------------------
; PARSEEXPRESSION.inc
;-----------------------------------------------------------------------------

    .proc ParseExpression


expression:     .data 0
@op:            .data 0

ParseExpression:
        DEBUGPRINT "ParseExpression\r\n"
        CLR expression
        CLR @op
@loop:

        CMPEQ peekedToken ASCII_E @isDone
        CMPEQ peekedToken ASCII_SEMICOLON @isDone
        CMPEQ peekedToken ASCII_COLON @isDone

        CALL GetParsedToken
;       CALL debugToken                 ;debug
;        PRINTCRLF                      ;debug;

        CMPEQ tokenType ASCII_N @isNum
        CMPEQ tokenType ASCII_S @isStr
        CMPEQ tokenType ASCII_O @isOp
        CMPEQ tokenType ASCII_V @isVar
        PUSH msgInvalidTokenType
        JMP SyntaxError

@isDone
     RET

@isStr:
        RET

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

    .endp
