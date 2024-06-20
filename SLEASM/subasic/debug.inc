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
;    PUSH cyan           ;debug
;    CALL PrintString       ;debug
;                           ;debug
;    OUT ASCII_T                    ;debug
;    OUT ASCII_o                    ;debug
;    OUT ASCII_k                    ;debug
;    OUT ASCII_e                    ;debug
;    OUT ASCII_n                    ;debug
;    OUT ASCII_EQUAL                    ;debug
;    OUT ASCII_QUOTE                    ;debug
;    PUSH token             ;debug
;    CALL PrintString       ;debug
;    OUT ASCII_QUOTE                    ;debug
;    OUT ASCII_SPACE        ;debug
;                           ;debug
;    OUT ASCII_T                    ;debug
;    OUT ASCII_y                    ;debug
;    OUT ASCII_p                    ;debug
;    OUT ASCII_e                    ;debug
;    OUT ASCII_EQUAL                    ;debug
;    OUT tokenType          ;debug
;    OUT ASCII_SPACE        ;debug
;                           ;debug
;    PUSH tokenValue        ;debug
;    CALL PrintInteger      ;debug
;    OUT ASCII_SPACE        ;debug
;                           ;debug
;    PUSH tokenAddr         ;debug
;    CALL PrintHex          ;debug
;    OUT ASCII_SPACE        ;debug
;                           ;debug
;    PUSH white             ;debug
;    CALL PrintString       ;debug
;    PRINTCRLF              ;debug
;    RET                    ;debug





;-----------------------------------------------------------------------------
debugCyanCh:
;-----------------------------------------------------------------------------
;    PUSH cyan                          ;debug
;    CALL PrintString                   ;debug
;    OUT ch                             ;debug
;    PUSH white                         ;debug
;    CALL PrintString                   ;debug
;    RET


;-----------------------------------------------------------------------------
debugCRLF:
;-----------------------------------------------------------------------------
;    OUT ASCII_CR       ;debug
;    OUT ASCII_LF       ;debug
;    RET                ;debug

