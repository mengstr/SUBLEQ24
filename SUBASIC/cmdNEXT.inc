;-----------------------------------------------------------------------------
cmdNEXT:        .proc cmdNEXT
;-----------------------------------------------------------------------------
;
; Loops back to the start of the current FOR loop
;
;-------|-----------------------------------|-------------------------------------

        POP @var1                                ; Pop the loop address
        POP @var5                                ; Pop the end value
        POP @var6                                ; Pop the variable address

        INDEXEDRD @var6 @value
        INC @value
        INDEXEDWR @value @var6

        JGT @value @var5 @done

        PUSH @var6                                ; Push the variable address
        PUSH @var5                                ; Push the end value
        PUSH @var1                                ; Push the loop address

        MOV @var1 pCode
        JMP Statement

@done:
        JMP Statement
;        MOV addrNextLine pCode                    ; Comparison is false, skip to the next line
;        JMP ProgramLine



@value: .data 0
@var1:  .data 0
@var2:  .data 0
@var3:  .data 0
@var4:  .data 0
@var5:  .data 0
@var6:  .data 0
@var7:  .data 0

    .endp


