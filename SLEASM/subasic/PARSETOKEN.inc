;-----------------------------------------------------------------------------
; PARSETOKEN.inc
;-----------------------------------------------------------------------------

    .proc ParseToken

tokenType:  .data 0                 ; 0=Error
                                    ; 1=EndOfLine 
                                    ; 2=EndOfStatement
                                    ; 3=Keyword
                                    ; 4=Operator
                                    ; 5=String
                                    ; 6=Numeric
                                    ; 7=Variable

tokenValue:     .data 0             ; Value of the token 
tokenVariable:  .data 0             ; Address of the variable if the token is a variable

ParseToken:
    CLR tokenType
    CLR tokenValue
    CLR tokenVariable

    CMPEQ token+1 CONST_0 @isEndOfLine

    CMPEQ token+1 ASCII_QUOTE @isString

    CMPEQ token+1 ASCII_PLUS @isOperator
    CMPEQ token+1 ASCII_MINUS @isOperator
    CMPEQ token+1 ASCII_ASTERISK @isOperator
    CMPEQ token+1 ASCII_SLASH @isOperator
    CMPEQ token+1 ASCII_EQUAL @isOperator

    JBETWEEN token+1 ASCII_0 ASCII_9 @isNumeric


    MOV keywords_ @k                ; If we are here, then it is a keyword or a variable
@loopKw:
    INDEXEDRD @k @p
    JEQZ @p @notAKeyword                ; If we reached the end of the keyword table, then it is not a keyword

    PUSH @p                             ; Compare the token with the current keyword from the table
    PUSH token
    CALL CompareString
    JNEZ val @isKeyword

    INC @k                              ; Nope, try the next keyword
    JMP @loopKw

@isKeyword:
    INDEXEDRD @k @p
    DEC @p
    INDEXEDRD @p tokenValue
    MOV CONST_3 tokenType               ; Return it as a keyword type to caller
    RET


@notAKeyword:
    JBETWEEN token+1 ASCII_A ASCII_Z @isVariable

    RET

@isVariable:
    MOV vars @p
    ASCII_A @p
    ADD token+1 @p
    MOV @p tokenVariable
    INDEXEDRD tokenVariable tokenValue
    MOV CONST_7 tokenType               ; Return it as a variable type to caller
    RET


@isString:
    MOV CONST_5 tokenType
    RET

@isEndOfLine:
    ; Just return the tokenType as 1
    MOV CONST_1 tokenType
    RET

@isOperator:
    ; Copy the operator to the tokenValue and return the tokenType as 4
    MOV token+1 tokenValue
    MOV CONST_4 tokenType
    RET

@isNumeric:
    ; Convert the token to a number and return the tokenType as 6
    PUSH token
    CALL ATOI
    MOV val tokenValue
    MOV CONST_6 tokenType
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
            .data kwEND+1
            .data kwLIST+1
            .data kwNEW+1 
            0

kwREM:      .data 1, "REM"   ,0
kwLET:      .data 2, "LET"   ,0
kwIF:       .data 3, "IF"    ,0
kwTHEN:     .data 4, "THEN"  ,0
kwGOTO:     .data 5, "GOTO"  ,0
kwPRINT:    .data 6, "PRINT" ,0
kwEND:      .data 7, "END"   ,0
kwLIST:     .data 8, "LIST"  ,0
kwNEW:      .data 9, "NEW"   ,0


    .endp
