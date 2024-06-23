;-----------------------------------------------------------------------------
; GETPARSEDTOKEN.inc
;
; This module provides a function to get the next token from the program source.
;
; Input:
;   pCode points to the current position in the program source. I.E the character 
;   to read and check next.
;
; Outputs:
;   pCode - pointing to the character in the program source to be parsed the next time.
;
;   tokenType  - Type of the token.
;                   0 = EOL
;                   1 = Number
;                   2 = Variable
;                   3 = Keyword
;                   4 = String
;                   5 = Miscellaneous (operator, semicolon, colon, etc)
;
;   tokenValue - Value of the token. 
;                   For numbers, this is the value of the number.
;                   For variables, this is the current value of the variable.
;
;   tokenAddr  - Address of the variable
;
;   tokenStr   - The token as a string
;                   For numbers, this is the string representation of the number.
;                   For variables, this is the name of the variable.
;                   For keywords, this is the keyword.  
;                   For strings, this is the string with the quotes removed
;                   For miscellaneous, this is the character itself
;
;
; Start by skipping any leading spaces.
;
; If the first non-space is EOL, return EOL
;
; If the first non-space chacater is a digit, then start collecting a number.
;   As long as we have digits, we the current value by 10 and add the digit.
;   Also collect them into tokenStr
;   If we reach a non-digit, then return the token as a number
;
; If the first non-space character is a double quote, then start collecting a string.
;   As long as we have characters not a double quote, collect them into tokenStr
;
; If the first non-space charecter is a letter, then start collecting a keyword or a variable.
;   As long as we have letters, digits or underscore collect them into tokenStr
;
; If the first non-space character is anything else then put just that character into tokenStr
;   and return as a miscellaneous token
;
;-----------------------------------------------------------------------------

tokenType:  .data 0     
tokenValue: .data 0
tokenAddr:  .data 0
tokenPeek:  .data 0

tokenStr:   .data $+1
            .data 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .data 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .data 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .data 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
pTokenStr   .data 0

pCode:      .data 0                 ; Pointer to the current position in the program source
ch:         .data 0                 ; Current character being processed

      .proc SkipSpaces
SkipSpaces:
            INDEXEDRD pCode ch
            CMPEQ ch CONST_0 @done
            CMPNE ch ASCII_SPACE @done
            INC pCode
            JMP SkipSpaces             
@done:      RET
      .endp

      .proc GetToken
GetParsedToken: 
            CLR tokenType
            CLR tokenValue
            CLR tokenAddr
            CLR tokenPeek
            MOV tokenStr pTokenStr
            CLR tokenStr+1

            CALL SkipSpaces               ; Skip any leading spaces

            CMPEQ ch CONST_0 @atEOL       ; Reached End-of-Line
            CMPEQ ch ASCII_DOUBLEQUOTE @string        ; Collect a String
            JBETWEEN ch ASCII_0 ASCII_9 @number       ; Collect a Number
            JBETWEEN ch ASCII_A ASCII_Z @kwVar        ; Collect a Keyword or Variable
            JBETWEEN ch ASCII_a ASCII_z @kwVar        ; Collect a Keyword or Variable

            MOV ch tokenValue                         ; Store in both tokenValue

            INDEXEDWR ch pTokenStr                     ; and in tokenString
            INC pTokenStr
            INDEXEDWR CONST_0 pTokenStr          ; Terminate the collected tokenString

            INC pCode                           ; Gobble up the character
            CALL SkipSpaces                     ; Skip trailing spaces
            MOV ch tokenPeek                    ; Store the next non-space character as peek

            MOV CONST_5 tokenType               ; Return with type as miscellaneous (5)
            RET

@atEOL:     INC pCode
            MOV CONST_0 tokenType               ; Return with type as EOL (0)
            RET

@string:    INC pCode                           ; Skip the initial quote
            INDEXEDRD pCode ch
            CMPEQ ch ASCII_DOUBLEQUOTE @stringDone
            CMPEQ ch CONST_0 SyntaxError
            INDEXEDWR ch pTokenStr
            INC pTokenStr
            JMP @string
@stringDone:INC pCode                           ; Skip the trailing quote
            INDEXEDWR CONST_0 pTokenStr          ; Terminate the collected tokenString

            CALL SkipSpaces                     ; Skip trailing spaces
            MOV ch tokenPeek                    ; Store the next non-space character as peek

            MOV CONST_4 tokenType               ; Return with type as string (4)
            RET

@number:    INDEXEDWR ch pTokenStr               ; Append digit to string
            INC pTokenStr

            MULU CONST_10 tokenValue           ; Compute the current number collected do far
            ADD ch tokenValue
            SUB ASCII_0 tokenValue

            INC pCode                           ; Get next digit and continue collecting if
            INDEXEDRD pCode ch                  ; it's a digit
            JBETWEEN ch ASCII_0 ASCII_9 @number

            INDEXEDWR CONST_0 pTokenStr         ; Terminate string

            CALL SkipSpaces                     ; Skip trailing spaces
            MOV ch tokenPeek                    ; Store the next non-space character as peek

            MOV CONST_1 tokenType               ; Return with type as numeric (1)
            RET


@kwVar:     INDEXEDWR ch pTokenStr               ; Append character to string
            INC pTokenStr

            INC pCode                           ; Get the next character
            INDEXEDRD pCode ch

            JBETWEEN ch ASCII_A ASCII_Z @kwVar  ; Keep collecting as long we have A-Z, a-z 0-9 or underscore 
            JBETWEEN ch ASCII_a ASCII_z @kwVar  
            JBETWEEN ch ASCII_0 ASCII_9 @kwVar      
            CMPEQ ch ASCII_UNDERSCORE @kwVar      

            INDEXEDWR CONST_0 pTokenStr          ; Terminate the collected tokenString

            CALL SkipSpaces                     ; Skip trailing spaces
            MOV ch tokenPeek                    ; Store the next non-space character as peek

            ;
            ; Check if it is a keyword
            ;

            MOV keywords @k                     ; If we are here, then it is a keyword or a variable
@loopKw:    
            INDEXEDRD @k @p
            JEQZ @p @notAKeyword                ; If we reached the end of the keyword table, then it is not a keyword

            PUSH @p                             ; Compare the token with the current keyword from the table
            PUSH tokenStr
            CALL CompareStringIgnoreCase
            JNEZ compResult @isKeyword

            INC @k                              ; Nope, try the next keyword
            INC tokenValue
            JMP @loopKw

@isKeyword:
            INC tokenValue                      ; Adjust the tokenValue

            CALL SkipSpaces                     ; Skip trailing spaces
            MOV ch tokenPeek                    ; Store the next non-space character as peek

            MOV CONST_3 tokenType               ; Return with type as keyword (3)
            RET

@notAKeyword:
            MOV vars @p
            ASCII_A @p
            ADD tokenStr+1 @p
            MOV @p tokenAddr
            INDEXEDRD tokenAddr tokenValue

            CALL SkipSpaces                     ; Skip trailing spaces
            MOV ch tokenPeek                    ; Store the next non-space character as peek

            MOV CONST_2 tokenType               ; Return with type as Variable (3)
            RET

@k          .data 0
@p          .data 0

keywords:   .data $+1
            .data kwREM       + 1
            .data kwLET       + 1
            .data kwIF        + 1
            .data kwTHEN      + 1
            .data kwGOTO      + 1 
            .data kwPRINT     + 1 
            .data kwGOSUB     + 1
            .data kwRETURN    + 1
            .data kwFOR       + 1
            .data kwTO        + 1
            .data kwSTEP      + 1
            .data kwNEXT      + 1
            .data kwEND       + 1
            .data kwLIST      + 1
            .data kwNEW       + 1 
            .data kwRUN       + 1 
            .data kwINPUT     + 1 
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
kwINPUT:    .data 17, "INPUT" ,0

    .endp

