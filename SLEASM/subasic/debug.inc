;-----------------------------------------------------------------------------
debugTokenType:
;-----------------------------------------------------------------------------
    PUSH green
    CALL PrintString

tt0:            CMPNE CONST_0 tokenType tt1
                PUSH TokenType0
                CALL PrintString
                JMP ttt

tt1:            CMPNE CONST_1 tokenType tt2
                PUSH TokenType1
                CALL PrintString
                JMP ttt

tt2:            CMPNE CONST_2 tokenType tt3
                PUSH TokenType2
                CALL PrintString
                JMP ttt

tt3:            CMPNE CONST_3 tokenType tt4
                PUSH TokenType3
                CALL PrintString
                JMP ttt

tt4:            CMPNE CONST_4 tokenType tt5
                PUSH TokenType4
                CALL PrintString
                JMP ttt

tt5:            CMPNE CONST_5 tokenType tt6
                PUSH TokenType5
                CALL PrintString
                JMP ttt

tt6:            CMPNE CONST_6 tokenType tt7
                PUSH TokenType6
                CALL PrintString
                OUT ASCII_LEFT_PAREN
                PUSH tokenNum
                CALL PrintInteger
                OUT ASCII_RIGHT_PAREN
                OUT ASCII_SPACE
                JMP ttt

tt7:            CMPNE CONST_7 tokenType tt8
                PUSH TokenType7
                CALL PrintString
                OUT ASCII_LEFT_BRACKET
                PUSH tokenVar
                CALL PrintInteger
                OUT ASCII_RIGHT_BRACKET
                OUT ASCII_SPACE
                OUT ASCII_i
                OUT ASCII_s
                OUT ASCII_SPACE
                PUSH tokenNum
                CALL PrintInteger
                JMP ttt

tt8:
ttt:
                OUT ASCII_SPACE
                OUT ASCII_p
                OUT ASCII_P
                OUT ASCII_r
                OUT ASCII_o
                OUT ASCII_g
                OUT ASCII_r
                OUT ASCII_a
                OUT ASCII_m
                OUT ASCII_EQUAL
                OUT ASCII_0
                OUT ASCII_x
                PUSH pProgram
                CALL PrintHex
                PUSH white
                CALL PrintString
                 PRINTCRLF

                RET

TokenType0: .data $+1, " ERROR ",0
TokenType1: .data $+1, " EOL ",0
TokenType2: .data $+1, " COLON ",0
TokenType3: .data $+1, " KEYWORD ",0
TokenType4: .data $+1, " OPERATOR ",0
TokenType5: .data $+1, " STRING ",0
TokenType6: .data $+1, " NUMERIC ",0
TokenType7: .data $+1, " VARIABLE ",0


;-----------------------------------------------------------------------------
debugShowLine:
;-----------------------------------------------------------------------------
    PRINTCRLF
    PUSH magenta
    CALL PrintString
    PUSH lineNo
    CALL PrintInteger
    OUT ASCII_SPACE
    PUSH pProgram
    CALL PrintString
    PUSH white
    CALL PrintString
    PRINTCRLF
    RET



;-----------------------------------------------------------------------------
debugCyanCh:
;-----------------------------------------------------------------------------
    PUSH cyan                   ;DEBUG
    CALL PrintString            ;DEBUG
    OUT ch                     ;DEBUG
    PUSH white                  ;DEBUG
    CALL PrintString            ;DEBUG
    RET


;-----------------------------------------------------------------------------
debugCRLF:
;-----------------------------------------------------------------------------
    OUT ASCII_CR
    OUT ASCII_LF
    RET

