;-----------------------------------------------------------------------------
cmdREM:
;-----------------------------------------------------------------------------
;
; Ignores the rest of the line as a comment 
;
;-------|-----------------------------------|-------------------------------------

        DEBUGPRINT "cmdREM:\r\n"


        MOV addrNextLine pCode           ; addrNextLine is calculated when the line is initially
        JMP ProgramLine                     ; parsed. It points to the start of the next line
                                            ; in the program code. This is where we want to jump
                                            ; to ignore the rest of the line

