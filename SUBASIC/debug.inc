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
;    PUSH pCode             ;debug
;    CALL PrintString       ;debug
;                           ;debug
;    PUSH white             ;debug
;    CALL PrintString       ;debug
;    PRINTCRLF              ;debug
;    RET                    ;debug

;-----------------------------------------------------------------------------
debugToken:
    .proc debugToken
;-----------------------------------------------------------------------------
;    PUSH cyan                  ;debug
;    CALL PrintString           ;debug
;                               ;debug
;    DEBUGPRINT "token='"       ;debug
;    PUSH tokenStr              ;debug
;    CALL PrintString           ;debug
;    DEBUGPRINT "' Type="       ;debug
;       CMPNE tokenType CONST_0 @n0  ;debug
;       DEBUGPRINT "EOL"        ;debug
;       JMP @nn                 ;debug
;@n0:   CMPNE tokenType CONST_1 @n1  ;debug
;       DEBUGPRINT "Num"        ;debug
;       JMP @nn                 ;debug
;@n1:   CMPNE tokenType CONST_2 @n2  ;debug
;       DEBUGPRINT "Var"        ;debug
;       JMP @nn                 ;debug
;@n2:   CMPNE tokenType CONST_3 @n3   ;debug
;       DEBUGPRINT "Keyw"       ;debug
;       JMP @nn                 ;debug
;@n3:   CMPNE tokenType CONST_4 @n4   ;debug
;       DEBUGPRINT "Str"        ;debug
;       JMP @nn                 ;debug
;@n4;   CMPNE tokenType CONST_5 @n5  ;debug
;       DEBUGPRINT "Misc"       ;debug
;       JMP @nn                 ;debug
;@n5:   DEBUGPRINT "???"        ;debug
;@nn:   DEBUGPRINT " Value="       ;debug
;       PUSH tokenValue            ;debug
;       CALL PrintInteger          ;debug
;       DEBUGPRINT " Addr=0x"      ;debug
;       PUSH tokenAddr             ;debug
;       CALL PrintHex              ;debug
;       DEBUGPRINT "' tokenPeek="  ;debug
;       OUT tokenPeek              ;debug
;       OUT ASCII_SPACE            ;debug
;       OUT ASCII_LEFT_PAREN       ;debug
;       PUSH tokenPeek             ;debug
;       CALL PrintInteger          ;debug
;       OUT ASCII_RIGHT_PAREN      ;debug
;                                  ;debug
;       PUSH white                 ;debug
;       CALL PrintString           ;debug
;       PRINTCRLF                  ;debug
;       RET                        ;debug
    .endp
