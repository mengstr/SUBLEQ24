;-----------------------------------------------------------------------------
    .proc Editor
;-----------------------------------------------------------------------------

@t:             .data 0                 ; Temporary hold variable
@s:     .data 0     ; Source for moving
@d:     .data 0     ; Destination for moving
@p:     .data 0     ; Pointer for moving
@ch:    .data 0     ; Character for moving
@len:   .data 0     ; Length of the line to be inserted

@arraylen: .data 0  ; Length of the array
@blocklen: .data 0  ; Length of the block to be manipulated

@msgAtLine: .data $+1,"\r\nAt line\r\n",0
@msgReplace: .data $+1,"\r\nReplace line\r\n",0
@msgAtEnd: .data $+1,"\r\nInsert at end\r\n",0


CalculateArrayLength:
        PUSH CONST_MAXINT       ; Calculate the length of the program array in use by searching for the last
        CALL FindLine           ; possible line number resulting in a pointer to the final "zero" line number
        MOV pFound @arraylen    ; The length is the found location minus the start location plus 1
        program_ @arraylen
        INC @arraylen
        RET


CalculateLineLength:
        POP @t                          ; Pop off the return address and parameters
        CALL FindLine                   ; Here I still have the target line number on the stack
        PUSH @t                         ; Push the return address back onto the stack

        MOV pFound @t                   ; Calculate the length of the line.
        INC @t                          ; Do this by taking the offset to the next line and add two
        INDEXEDRD @t @blocklen          ; ... to ajust for the line number and the offset itself 
        ADD CONST_2 @blocklen

        RET


CalculateBufLength:
    MOV pCode @t
    CLR @blocklen
@loopBL:
    INDEXEDRD @t @ch
    CMPEQ @ch CONST_0 @doneBL
    INC @t
    INC @blocklen
    JMP @loopBL        
@doneBL:
    RET

    ;-----------------------------------------------------------------------------
    ; Editor
    ;-----------------------------------------------------------------------------

Editor:
;        PRINTCRLF

        PUSH tokenValue                         ; Check if the specified line number already exists
        CALL FindLine

        INDEXEDRD pFound lineNo                 ; The FindLine can get the next larger number if the exact
                                                ; number is not found so we need to load the actual found number 
        CMPNE tokenValue lineNo @insert         ; If the line number is not found, the user wants insert a new line  

        ; If the line number is found, the user wants to replace or delete it.
        ; Start by deleting the line and then insert the new line if the user wants to replace it.

        CALL CalculateArrayLength               

        PUSH lineNo
        CALL CalculateLineLength

        MOV pFound @t
        SUB program_ @t

        PUSH program                ; Point to start of the array to manipulate
        PUSH @arraylen              ; Total length of the array
        PUSH @t                     ; Offset into the array where the manipulated block starts
        PUSH @blocklen              ; Length of the manipulated block
        CALL DeleteBlock

        ; The old line is now deleted. So now we can insert the new line if the user wants to replace it.
        ; But we first check if the new line is empty, we then just return to the prompt.
@insert:
        CALL CalculateArrayLength               

        CALL CalculateBufLength
        CMPEQ @blocklen CONST_0 @done

        PUSH tokenValue                         ; Check if the specified line number already exists
        CALL FindLine

        ADD CONST_3 @blocklen               ; Add three to the length to make room for the line number and the offset to the next line
        MOV pFound @t
        SUB program_ @t

        PUSH program             ; Point to start of the array to manipulate
        PUSH @arraylen           ; Total length of the array
        PUSH @t                  ; Offset into the array where the manipulated block starts
        PUSH @blocklen           ; Length of the manipulated block
        CALL InsertBlock

        INDEXEDWR tokenValue pFound     ; Write the line number
        INC pFound
        DEC @blocklen                   ; Adjust the length/offset
        DEC @blocklen
        INDEXEDWR @blocklen pFound      ; Write the offset to the next line
        INC pFound

        MOV pCode @t
        DEC @blocklen
@loopIB:
        INDEXEDRD @t @ch
        INDEXEDWR @ch pFound
        INC @t
        INC pFound
        JNEZ @ch @loopIB

@done:
        JMP Prompt

    .endp

;-----------------------------------------------------------------------------
    .proc DeleteBlock
;-----------------------------------------------------------------------------

@t:             .data 0                 ; Temporary hold variable
@arrayStart:    .data 0
@arraySize:     .data 0
@delStartOffset:.data 0
@delLen:        .data 0
@ch:            .data 0 
@pD:             .data 0
@pS:             .data 0
@cnt:            .data 0

DeleteBlock:
        POP @t                          ; Pop off the return address and parameters
        POP @delLen
        POP @delStartOffset
        POP @arraySize
        POP @arrayStart
        PUSH @t                         ; Push the return address back onto the stack

        MOV program_ @pD
        ADD @delStartOffset @pD
        MOV @pD @pS
        ADD @delLen @pS
        MOV @arraySize @cnt
        @delStartOffset @cnt
        @delLen @cnt

@delLoop:
        INDEXEDRD @pS @ch
        INDEXEDWR @ch @pD
        INC @pS
        INC @pD
        DEC @cnt
        JNEZ @cnt @delLoop

        MOV @delLen @cnt
@fill0Loop:
        INDEXEDWR CONST_0 @pD
        INC @pD
        DEC @cnt
        JNEZ @cnt @fill0Loop

        RET
    .endp


;-----------------------------------------------------------------------------
    .proc InsertBlock
;-----------------------------------------------------------------------------

@t:             .data 0                 ; Temporary hold variable
@arrayStart:    .data 0
@arraySize:     .data 0
@insStartOffset:.data 0
@insLen:        .data 0
@ch:            .data 0 
@pD:             .data 0
@pS:             .data 0
@cnt:            .data 0

InsertBlock:
        POP @t                          ; Pop off the return address and parameters
        POP @insLen
        POP @insStartOffset
        POP @arraySize
        POP @arrayStart
        PUSH @t                         ; Push the return address back onto the stack

        MOV program_ @pD            ; destination = startOfArray + lengthOfArray + lengthOfInsert
        ADD @arraySize @pD
        ADD @insLen @pD

        MOV program_ @pS            ; source = startOfArray + lengthOfArray
        ADD @arraySize @pS

        MOV @arraySize @cnt         ; length = lengthOfArray - startOfInsert + 1
        @insStartOffset @cnt
        INC @cnt

@insLoop:
        INDEXEDRD @pS @ch
        INDEXEDWR @ch @pD
        DEC @pS
        DEC @pD
        DEC @cnt
        JNEZ @cnt @insLoop

        MOV @insLen @cnt
@fill0Loop:
        INDEXEDWR CONST_x555 @pD
        DEC @pD
        DEC @cnt
        JNEZ @cnt @fill0Loop

        RET
    .endp
