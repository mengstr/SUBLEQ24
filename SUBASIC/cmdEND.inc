;-----------------------------------------------------------------------------
; END
;-----------------------------------------------------------------------------
;
; Terminates the running of the interpreter and returns to the command prompt.
;
;-----------------------------------------------------------------------------
cmdEND:
    PUSH  msgHaltedByEnd
    CALL  PrintString

    JMP Prompt

