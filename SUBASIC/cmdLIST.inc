;-----------------------------------------------------------------------------
cmdLIST:        .proc cmdLIST
;-----------------------------------------------------------------------------
;
; List the BASIC program code
;
;-------|-----------------------------------|-------------------------------------

        MOV program_ @p                     ; Point to start of source code
        PRINTCRLF

@loop1: 
        INDEXEDRD @p @lineNo                ; Get Linenumber of current line as a binary value
        INC @p
        JEQZ @lineNo @done                  ; Reached the end of the code?

        PUSH @lineNo                        ; Print the line number and a space
        CALL PrintInteger
        OUT ASCII_SPACE

        INC @p                              ; Skip the offset to the next line

@loop2:
        INDEXEDRD @p @ch                    ; Get Linenumber of current line as a binary value
        INC @p
        JEQZ @ch @eol                       ; Reached the end of the line?
        OUT @ch                             ; Print the character
        JMP @loop2      

@eol:
        PRINTCRLF                           ; Print a CRLF and go print the next line
        JMP @loop1

@done:
        RET

@p:             .data 0                     ; Pointer to current location in the program
@lineNo:        .data 0                     ; Line number;
@ch:            .data 0                     ; Tempporary storage for the character

    .endp




;-----------------------------------------------------------------------------
PromptLIST:
;-----------------------------------------------------------------------------
        CALL cmdLIST
        JMP Restart


