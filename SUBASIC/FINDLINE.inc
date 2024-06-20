;-----------------------------------------------------------------------------
.proc FindLine
;-----------------------------------------------------------------------------

@target:        .data 0                 ; Target line number from caller
@lineNo:        .data 0                 ; Line number of current line
@p:             .data 0
@t:             .data 0                 ; Temporary hold variable

FindLine:
        POP @t                          ; Pop off the return address and parameters
        POP @target
        PUSH @t                         ; Push the return address back onto the stack

        MOV program_ @p                 ; Point to start of source code

@loop:
        INDEXEDRD @p @lineNo            ; Get Linenumber of current line as a binary value
        JEQZ @lineNo @notFound          ; If we reach the end of the program, the line was not found
        INC @p
    
        INDEXEDRD @p offsetNextLine     ; Get offset to next line
        INC @p
    
        CMPLEQ @target @lineNo @found    ; Are we at the (or higher) target line number?

        ADD offsetNextLine @p           ; Otherwise, skip to the next line
        JMP @loop

@found:
        DEC @p                          ; Back up to beginning of line 
        DEC @p
        MOV @p pFound                   ; ... and set the program pointer to the start of the line
        RET

@notFound:
        MOV @p pFound                   ; ... and set the program pointer to the start of the line
;        OUT ASCII_SPACE        ;Xdebug
;        PUSH pFound            ;Xdebug
;        CALL PrintHex          ;Xdebug
;        OUT ASCII_SPACE        ;Xdebug
        RET
    .endp
