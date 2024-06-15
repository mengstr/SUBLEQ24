;-----------------------------------------------------------------------------
; GETTOKEN.inc
;-----------------------------------------------------------------------------

    .proc GetToken

tokenType:  .data 0                     ; 0=Error
                                        ; 1=EndOfLine 
                                        ; 2=EndOfStatement
                                        ; 3=Keyword
                                        ; 4=Operator
                                        ; 5=String
                                        ; 6=Numeric
                                        ; 7=Variable

tokenNum:   .data 0                     ; Value of the token if it is numeric or the kwXXXX value if it is a keyword
tokenVar:   .data 0                     ; Address of the variable if the token is a variable
tokenStr_:  .data $+1                   ; address to the buffer if the token is a string
tokenStr:   .org $+132                  ; buffer to hold the token if it is a string

@tokenStrP: .data 0
@p:         .data 0

GetToken:
    CLR tokenType
    CLR tokenNum
    CLR tokenVar
    MOV tokenStr_ @tokenStrP
    CLR @inQuote

    OUT ASCII_SPACE                 ;DEBUG
    OUT ASCII_SPACE                 ;DEBUG
    OUT ASCII_SPACE                 ;DEBUG
    OUT ASCII_SPACE                 ;DEBUG


@loopLeadningSpaces:
    INDEXEDRD pProgram ch               ; Get the first character of the token, but ignore leadning spaces
    CMPEQ ch CONST_0 @endOfLineX         ; End of line reached
    CMPNE ch ASCII_SPACE @notAspace     ; Not a space  
    INC pProgram
    JMP @loopLeadningSpaces

@endOfLineX:
    OUT ASCII_AT
    JMP @endOfLine


@notAspace:
    CMPEQ ch ASCII_QUOTE @collectString     ; If the first character is a quote, then we are collecting a string
                                            ; so start collecting the string in a separate routne


    CMPEQ ch ASCII_EQUAL @doneOp            ; is it an operator?
    CMPEQ ch ASCII_PLUS @doneOp             ; End of token reached


@loopCollectToken:                          ; Start collecting the token
    INDEXEDRD pProgram ch                   ; Get a character of the token
    CMPEQ ch CONST_0 @done                  ; End of line reached
    CMPEQ ch ASCII_SPACE @done              ; End of token reached
    CMPEQ ch ASCII_EQUAL @done              ; End of token reached
    CMPEQ ch ASCII_PLUS @done               ; End of token reached

    CALL debugCyanCh

    INDEXEDWR ch @tokenStrP
    INC @tokenStrP
    INC pProgram
    JMP @loopCollectToken


    ; We have something that can be either a keyword, a number or a variable
@done:
    INDEXEDWR CONST_0 @tokenStrP        ; Terminate the token string

    ; If the string starts with a digit, then it is a number
    JBETWEEN tokenStr ASCII_0 ASCII_9 @doneIsNum

    ; See if it is a keyword from the keyword table
    MOV keywords_ @k

@loopKw:
    INDEXEDRD @k @p
    JEQZ @p @notAKeyword                ; If we reached the end of the keyword table, then it is not a keyword

    PUSH @p                             ; Compare the token with the current keyword from the table
    PUSH tokenStr_
    CALL CompareString
    JNEZ val @foundTheKeyword

    INC @k                              ; Nope, try the next keyword
    JMP @loopKw

@foundTheKeyword:
    INDEXEDRD @k @p
    DEC @p
    INDEXEDRD @p tokenNum
    MOV CONST_3 tokenType               ; Return it as a keyword type to caller
    RET


@notAKeyword:
    JBETWEEN tokenStr ASCII_A ASCII_Z @isVar

    RET


@isVar:
    MOV vars P
    ADD tokenStr P
    ASCII_A P
    MOV P tokenVar
    INDEXEDRD tokenVar tokenNum
    MOV CONST_7 tokenType               ; Return it as a variable type to caller
    CALL debugTokenType
    RET

@doneIsNum:
    PUSH tokenStr_
    CALL ATOI
    MOV val tokenNum
    MOV CONST_6 tokenType               ; Return it as a numeric type to caller
    CALL debugTokenType
    RET


@doneOp:
    CALL debugCyanCh
    INDEXEDWR ch @tokenStrP            ; Store the operator as a zero-terminated string
    INC @tokenStrP
    INDEXEDWR CONST_0 @tokenStrP
    INC pProgram                        ; Gobble up the operator    
    MOV CONST_4 tokenType               ; Return it as an operand type to caller
    CALL debugTokenType
    RET


@endOfLine:
    MOV CONST_1 tokenType               ; Return an end-of-line type to caller
    INC pProgram                        ; Gobble up the end-of-line
    RET

@collectString:
    CALL debugCyanCh
    INC pProgram
@loopCollectString:
    INDEXEDRD pProgram ch
    CALL debugCyanCh
    CMPEQ ch ASCII_QUOTE @endOfQuote
    INDEXEDWR ch @tokenStrP
    INC @tokenStrP
    INC pProgram
    JMP @loopCollectString 
@endOfQuote:
    INDEXEDWR CONST_0 @tokenStrP
    MOV CONST_5 tokenType               ; Return a string type to caller
    CALL debugTokenType
    INC pProgram                        ; Gobble up the last quote
    RET


@inQuote:   .data 0                     ; 0: not in quote, 1: in quote
@k:         .data 0

    .endp


keywords_:  .data $+1
keywords:   .data kwLET+1, kwIF+1, kwTHEN+1, kwGOTO+1, kwPRINT+1, kwEND+1, 0


kwLET:      .data 10, "LET"   ,0
kwIF:       .data 11, "IF"    ,0
kwTHEN:     .data 12, "THEN"  ,0
kwGOTO:     .data 13, "GOTO"  ,0
kwPRINT:    .data 14, "PRINT" ,0
kwEND:      .data 15, "END"   ,0
