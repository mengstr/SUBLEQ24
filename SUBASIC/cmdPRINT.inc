;-----------------------------------------------------------------------------
cmdPRINT:       .proc cmdPRINT
;-----------------------------------------------------------------------------
;
; Print the value of one or more expressions or string litterals.
;
; PRINT 1
; PRINT A
; PRINT "Hello"
; PRINT 2;
; PRINT B;
; PRINT "World";
; PRINT A;3;"foo";B;4;"bar"
;
;-----------------------------------------------------------------------------

    ; Check for an empty print statement first
    CMPEQ peekedToken ASCII_E @doneCrlf       ; If the token is an EOL, we're done
    CMPEQ peekedToken ASCII_COLON @doneCrlf   ; If the token is an colon, we're done

    CALL ParseExpression

    CMPEQ tokenType ASCII_E @doneCrlf       ; If the token is an EOL, we're done
    CMPEQ tokenType ASCII_COLON @doneCrlf   ; If the token is a colon, we're done
    CMPEQ tokenType ASCII_S @string         ; If the token is a string, print it

    PUSH expression                         ; Else print the integer (constant/variable)
    CALL PrintInteger
    JMP @continue

@string:
    PUSH token1
    CALL PrintString
    JMP @continue

@continue:
    CMPEQ peekedToken ASCII_SEMICOLON @semi1

@doneCrlf
    PRINTCRLF                               ; Else print a CRLF and continue with the next statement
    JMP Statement

@semi1:
    CALL GetParsedToken                     ; Gobble up the semicolon
    CMPEQ peekedToken ASCII_E Statement           ; If the next token is an EOL, we're done
    CMPNE peekedToken ASCII_COLON cmdPRINT    ; If the next token is not a colon, continute printing here
 
    JMP Statement

    .endp 
