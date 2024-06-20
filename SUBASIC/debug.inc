;-----------------------------------------------------------------------------
debugLine:
;-----------------------------------------------------------------------------
;    PRINTCRLF              ;debug
;    PUSH magenta           ;debug
;    CALL PrintString       ;debug
;                           ;debug
;    PUSH lineNo            ;debug
;    CALL PrintInteger      ;debug
;    OUT ASCII_SPACE        ;debug
;    PUSH pProgram          ;debug
;    CALL PrintString       ;debug
;                           ;debug
;    PUSH white             ;debug
;    CALL PrintString       ;debug
;    PRINTCRLF              ;debug
;    RET                    ;debug

;-----------------------------------------------------------------------------
debugToken:
;-----------------------------------------------------------------------------
;    PUSH cyan              ;debug
;    CALL PrintString       ;debug
;                           ;debug
;    DEBUGPRINT "token='"   ;debug
;    PUSH token             ;debug
;    CALL PrintString       ;debug
;    DEBUGPRINT "' Type="   ;debug
;    OUT tokenType          ;debug
;    DEBUGPRINT " Value="   ;debug
;    PUSH tokenValue        ;debug
;    CALL PrintInteger      ;debug
;    DEBUGPRINT " Addr=0x" ;debug
;    PUSH tokenAddr         ;debug
;    CALL PrintHex          ;debug
;    DEBUGPRINT "' peekedToken="    ;debug
;    OUT peekedToken        ;debug
;                           ;debug
;    PUSH white             ;debug
;    CALL PrintString       ;debug
;    PRINTCRLF              ;debug
;    RET                    ;debug

