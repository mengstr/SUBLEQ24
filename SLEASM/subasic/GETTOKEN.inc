;-----------------------------------------------------------------------------
; GETTOKEN.inc
;-----------------------------------------------------------------------------

token1:     .data $+3                               ; Address of the second character in the token
token:      .data $+1
            .data 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .data 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .data 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .data 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

peekedToken:.data 0
tokenType:  .data 0     ; E=EndOfLine 
                        ; :=EndOfStatement
                        ; ;=Inhibit CRLF after print
                        ; K=Keyword
                        ; O=Operator
                        ; S=String
                        ; N=Numeric
                        ; V=Variable
                        ; M=Miscellaneous
tokenValue: .data 0
tokenAddr:  .data 0

    .proc GetToken

@pToken:    .data 0
@ch:        .data 0

GetParsedToken:
    MOV token @pToken
    CLR token+1
    CLR tokenValue
    CLR tokenAddr
    CLR peekedToken

@LoopSkipSpaces:
    INDEXEDRD pProgram @ch
    CMPEQ @ch CONST_0 @eol
    CMPNE @ch ASCII_SPACE @atBeginningOfToken
    INC pProgram
    JMP @LoopSkipSpaces

@eol:
    INC @pToken                     ; Null-terminate token
    INDEXEDWR CONST_0 @pToken
    MOV ASCII_E tokenType
    RET

@atBeginningOfToken:
    CMPEQ @ch ASCII_DOUBLEQUOTE @collectString
    JBETWEEN @ch ASCII_0 ASCII_9 @collectNumber
    JBETWEEN @ch ASCII_A ASCII_Z @collectAlpha
    JBETWEEN @ch ASCII_a ASCII_z @collectAlpha

@miscCharacter:
    INDEXEDWR @ch @pToken
    INC pProgram
    INC @pToken                     ; Null-terminate token
    INDEXEDWR CONST_0 @pToken
    CMPEQ @ch ASCII_SEMICOLON @isSemicolon
    CMPEQ @ch ASCII_COLON @isColon
    CMPEQ @ch CONST_0 @isZero
    CMPEQ @ch ASCII_PLUS @isOperator
    CMPEQ @ch ASCII_MINUS @isOperator
    CMPEQ @ch ASCII_SLASH @isOperator
    CMPEQ @ch ASCII_ASTERISK @isOperator
    MOV ASCII_M tokenType
    JMP @peekNextToken

@isSemicolon:
    MOV ASCII_SEMICOLON tokenType
    JMP @peekNextToken

@isColon:
    MOV ASCII_COLON tokenType
    JMP @peekNextToken

@isZero:
    MOV ASCII_E tokenType
    JMP @peekNextToken

@isOperator:
    MOV @ch tokenValue
    MOV ASCII_O tokenType
    JMP @peekNextToken


@collectString:
    INDEXEDWR @ch @pToken
    INC @pToken
    INC pProgram
    INDEXEDRD pProgram @ch
    CMPEQ @ch CONST_0 SyntaxError
    CMPNE @ch ASCII_DOUBLEQUOTE @collectString
    INC pProgram                    ; Remove the final quote in the string

    INDEXEDWR CONST_0 @pToken       ; Null-terminate token

    MOV ASCII_S tokenType           ; Return it as a string type to caller
    JMP @peekNextToken

@collectNumber:
    SUB ASCII_0 @ch                 ; Make the ascii digit to a number
    MULU CONST_10 tokenValue        ; Collect the digit into the result
    ADD @ch tokenValue
    INC pProgram
    INDEXEDRD pProgram @ch
    JBETWEEN @ch ASCII_0 ASCII_9 @collectNumber
    MOV ASCII_N tokenType               
    JMP @peekNextToken

@collectAlpha:
    INDEXEDWR @ch @pToken
    INC @pToken
    INC pProgram
    INDEXEDRD pProgram @ch
    JBETWEEN @ch ASCII_0 ASCII_9 @collectAlpha
    JBETWEEN @ch ASCII_A ASCII_Z @collectAlpha
    JBETWEEN @ch ASCII_a ASCII_z @collectAlpha
    CMPEQ @ch ASCII_UNDERSCORE @collectAlpha

;    INC @pToken                     ; Null-terminate token
    INDEXEDWR CONST_0 @pToken

    ;
    ; Check if it is a keyword
    ;

     ; Check if it is a keyword
    MOV keywords_ @k                    ; If we are here, then it is a keyword or a variable
@loopKw:
    INDEXEDRD @k @p
    JEQZ @p @notAKeyword                ; If we reached the end of the keyword table, then it is not a keyword

    PUSH @p                             ; Compare the token with the current keyword from the table
    PUSH token
    CALL CompareStringIgnoreCase
    JNEZ compResult @isKeyword

    INC @k                              ; Nope, try the next keyword
    JMP @loopKw

@isKeyword:
    INDEXEDRD @k @p
    DEC @p
    INDEXEDRD @p tokenValue
    MOV ASCII_K tokenType               ; Return it as a keyword type to caller
    JMP @peekNextToken

@notAKeyword:
    ; Convert it into a number into tokenValue and return the tokenType as V
    MOV vars @p
    ASCII_A @p
    ADD token+1 @p
    MOV @p tokenAddr
    INDEXEDRD tokenAddr tokenValue
    MOV ASCII_V tokenType               
    JMP @peekNextToken

@peekNextToken:
    MOV @pToken @p
@pnt
    INDEXEDRD @p @ch
    INC @p
    CMPEQ @ch ASCII_SPACE @pnt
    MOV @ch peekedToken
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

