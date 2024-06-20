;-----------------------------------------------------------------------------
; PARSETOKEN.inc
;-----------------------------------------------------------------------------

    .proc ParseToken

tokenType:  .data 0     ; !=Error
                        ; E=EndOfLine 
                        ; :=EndOfStatement
                        ; ;=Inhibit CRLF after print
                        ; K=Keyword
                        ; O=Operator
                        ; S=String
                        ; N=Numeric
                        ; V=Variable

tokenValue:     .data 0             ; Value of the token 
tokenVariable:  .data 0             ; Address of the variable if the token is a variable

ParseToken:
    MOV ASCII_EXCLAMATION tokenType
    CLR tokenValue
    CLR tokenVariable

    CMPEQ token+1 CONST_0 @isEndOfLine
    CMPEQ token+1 ASCII_COLON @isColon
    CMPEQ token+1 ASCII_DOUBLEQUOTE @isString
    JBETWEEN token+1 ASCII_0 ASCII_9 @isNumeric

    ; Check if it is a keyword
    MOV keywords_ @k                    ; If we are here, then it is a keyword or a variable
@loopKw:
    INDEXEDRD @k @p
    JEQZ @p @notAKeyword                ; If we reached the end of the keyword table, then it is not a keyword

    PUSH @p                             ; Compare the token with the current keyword from the table
    PUSH token
    CALL CompareStringIgnoreCase
    JNEZ val @isKeyword

    INC @k                              ; Nope, try the next keyword
    JMP @loopKw

@isKeyword:
    INDEXEDRD @k @p
    DEC @p
    INDEXEDRD @p tokenValue
    MOV ASCII_K tokenType               ; Return it as a keyword type to caller
    RET

@notAKeyword:
    JBETWEEN token+1 ASCII_A ASCII_Z @isVariable

; Check if it is an operator
    CMPEQ token+1 ASCII_PLUS @isOperator
    CMPEQ token+1 ASCII_MINUS @isOperator
    CMPEQ token+1 ASCII_ASTERISK @isOperator
    CMPEQ token+1 ASCII_SLASH @isOperator
    CMPEQ token+1 ASCII_EQUAL @isOperator
    CMPEQ token+1 ASCII_GREATER @isOperator
    CMPEQ token+1 ASCII_SEMICOLON @isSemicolon
    CMPEQ token+1 ASCII_LESS handleLessThan
    PUSH msgInvalidOperator
    JMP SyntaxError

handleLessThan:
    ; The < symbol can either be a "less than" operator or a "differs" operator
    CALL GetToken
    CMPEQ token+1 ASCII_GREATER @differsOp
    MOV ASCII_LESS tokenValue
    MOV ASCII_O tokenType
    RET

@differsOp:
    MOV ASCII_TILDE tokenValue
    MOV ASCII_O tokenType
    RET

@isSemicolon:
    ; Return the tokenType as ;
    MOV ASCII_SEMICOLON tokenType
    RET

@isOperator:
    ; Copy the operator to the tokenValue and return the tokenType as O
    MOV token+1 tokenValue
    MOV ASCII_O tokenType
    RET




@isVariable:
    ; Convert it into a number into tokenValue and return the tokenType as V
    MOV vars @p
    ASCII_A @p
    ADD token+1 @p
    MOV @p tokenVariable
    INDEXEDRD tokenVariable tokenValue
    MOV ASCII_V tokenType               
    RET


@isString:
    ; Return the tokenType as S
    MOV ASCII_S tokenType
    RET


@isEndOfLine:
    ; Return the tokenType as E
    MOV ASCII_E tokenType
    RET

@isColon:
    ; Return the tokenType as :
    MOV ASCII_COLON tokenType
    RET



@isNumeric:
    ; Convert the token to a number and return the tokenType as N
    PUSH token
    CALL ATOI
    MOV val tokenValue
    MOV ASCII_N tokenType
    RET


@k:         .data 0
@p:         .data 0

keywords_:  .data $+1
keywords:   .data kwREM+1
            .data kwLET+1
            .data kwIF+1
            .data kwTHEN+1
            .data kwGOTO+1 
            .data kwPRINT+1 
            .data kwGOSUB+1
            .data kwRETURN+1
            .data kwFOR+1
            .data kwTO+1
            .data kwSTEP+1
            .data kwNEXT+1
            .data kwEND+1
            .data kwLIST+1
            .data kwNEW+1 
            .data kwRUN+1 
            0

kwREM:      .data 1, "REM"   ,0
kwLET:      .data 2, "LET"   ,0
kwIF:       .data 3, "IF"    ,0
kwTHEN:     .data 4, "THEN"  ,0
kwGOTO:     .data 5, "GOTO"  ,0
kwPRINT:    .data 6, "PRINT" ,0
kwGOSUB:    .data 7, "GOSUB" ,0
kwRETURN:   .data 8, "RETURN",0
kwFOR:      .data 9, "FOR"   ,0
kwTO:       .data 10, "TO"   ,0
kwSTEP:     .data 11, "STEP" ,0
kwNEXT:     .data 12, "NEXT" ,0
kwEND:      .data 13, "END"   ,0
kwLIST:     .data 14, "LIST"  ,0
kwNEW:      .data 15, "NEW"   ,0
kwRUN:      .data 16, "RUN"   ,0


    .endp
