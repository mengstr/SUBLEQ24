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

    DEBUGPRINT "cmdPRINT:\r\n"

@loop:
    CMPEQ tokenPeek CONST_0 @doneCrlf       ; If the token is an EOL, we're done
    CMPEQ tokenPeek ASCII_COLON @doneCrlf   ; If the token is an colon, we're done

    CALL ParseExpression
    CMPEQ tokenType CONST_4 @string               ; If the token is a string, print it
    PUSH expression                         ; Else print the integer (constant/variable)
    CALL PrintInteger
    JMP @continue

@string:
    PUSH tokenStr
    CALL PrintString

@continue:
    CMPNE tokenPeek ASCII_SEMICOLON @doneCrlf
    CALL GetParsedToken                     ; Gobble up the semicolon
    JMP @loop                               ; ...and do more printing if required


@doneCrlf
    CMPNE tokenType CONST_5 @crlf
    CMPEQ tokenValue ASCII_SEMICOLON Statement
@crlf:
    PRINTCRLF                               ; Else print a CRLF and continue with the next statement
    JMP Statement

    .endp 
