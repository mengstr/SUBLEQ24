;-----------------------------------------------------------------------------
    .proc Editor
;-----------------------------------------------------------------------------

@s:     .data 0     ; Source for moving
@d:     .data 0     ; Destination for moving
@p:     .data 0     ; Pointer for moving
@ch:    .data 0     ; Character for moving
@len:   .data 0     ; Length of the line to be inserted

@msgAtLine: .data $+1,"\r\nAt line\r\n",0
@msgReplace: .data $+1,"\r\nReplace line\r\n",0
@msgAtEnd: .data $+1,"\r\nInsert at end\r\n",0

Editor:
        PRINTCRLF
        PUSH tokenValue
        CALL FindLine
        INDEXEDRD pFound lineNo
        CMPEQ lineNo tokenValue replace
        CMPEQ lineNo CONST_0 insertAtEnd

insertAtLine:
        PUSH @msgAtLine
        CALL PrintString
        JMP Prompt

replace:
        PUSH @msgReplace
        CALL PrintString
        JMP Prompt        

insertAtEnd:
        PUSH @msgAtEnd
        CALL PrintString

        MOV pFound @d

        CLR @len
@loop:
        CALL GetToken
        CALL ParseToken
        CMPEQ tokenType CONST_1 @endOfLine
        CMPEQ tokenType CONST_3 @keyword
        CMPEQ tokenType CONST_4 @operator
        CMPEQ tokenType CONST_5 @string
        CMPEQ tokenType CONST_6 @numeric
        CMPEQ tokenType CONST_7 @variable
        OUT ASCII_SPACE
        PUSH tokenValue
        CALL PrintHex
        PRINTCRLF
        JMP @loop


@endOfLine:
        JMP Prompt


@variable:
    MOV token+1 @ch
    OUT @ch
    OUT ASCII_SPACE
    JMP @loop

@operator:
    OUT tokenValue
    OUT ASCII_SPACE
    JMP @loop

@string:
    PUSH token
    CALL PrintString
    OUT ASCII_QUOTE
    OUT ASCII_SPACE
    JMP @loop

@numeric:
    PUSH tokenValue
    CALL PrintInteger
    OUT ASCII_SPACE
    JMP @loop

@keyword:
        MOV keywords_ @p
        ADD tokenValue @p
        DEC @p
        INDEXEDRD @p @p
@kwLoop:
        INDEXEDRD @p @ch
        CMPEQ @ch CONST_0 @kwEnd
        INC @p
        OUT @ch
        JMP @kwLoop
@kwEnd: OUT ASCII_SPACE
        JMP @loop

        INDEXEDWR tokenValue @d; Write the line number
        INC @d
        INDEXEDWR CONST_4 @d   ; Write the offset to the next line
        INC @d
        INDEXEDWR ASCII_R @d; Write a REM statement
        INC @d
        INDEXEDWR ASCII_E @d
        INC @d
        INDEXEDWR ASCII_M @d
        INC @d
        INDEXEDWR CONST_0 @d   ; Write a null terminator
        INC @d
        INDEXEDWR CONST_0 @d   ; Write the end of source (lineNo zero)

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
