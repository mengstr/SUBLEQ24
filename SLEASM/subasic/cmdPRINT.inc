;-----------------------------------------------------------------------------
; PRINT
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

cmdPRINT:
    CALL ParseExpression
    PUSH expression
    CALL PrintInteger

cmdPRINTdone:
    RET

; -----------------------------------------------------------------------------
;cmdPRINT:
; -----------------------------------------------------------------------------
;printMore:
;        OUT ASCII_AT
;        CALL ParseExpression
;        CMPEQ tokenType ASCII_E cmdPrintDoneCRLF
;        JMP printMore;
;
;        CMPEQ tokenType ASCII_S cmdPRINTstring ;
;
;        PUSH expression
;        CALL PrintInteger
;
;        CMPEQ tokenType ASCII_SEMICOLON semi1 
;    ;    CMPEQ token+1 ASCII_SEMICOLON semi1     ; If it's a semicolon, don't print a CRLF
;        PRINTCRLF
;        JMP Statement
;
;semi1:  
;        JMP printMore
;
;cmdPRINTstring:
;        PUSH token1             ; Push the string address, but start 1 character in
;        CALL PrintString;
;
;        CMPEQ ch ASCII_SEMICOLON semi2     ; If it's a semicolon, don't print a CRLF
;        PRINTCRLF
;        JMP Statement
;
;semi2:  JMP printMore
;
;cmdPrintDoneCRLF:
;        CALL PrintString
;        JMP Statement


