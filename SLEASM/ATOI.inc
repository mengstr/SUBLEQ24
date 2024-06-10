    .proc ATOI

@return:
    JMP 0

@resultP: .data 0
@sourceP: .data 0

ATOI:
    INDEXEDRD @sourceP @src
    CLR @res

@loop:
    INDEXEDRD @src @digit

    CLRZ
    CONST_SLASH @digit @done    ; If ouside of 0-9 then we are done
    CONST_10 @digit @valid
    JMP @done

@valid: 
    ADD CONST_9 @digit          ; Re-adjust the digit after the testing

    MULU CONST_10 @res          ; Collect the digit into the result
    ADD @digit @res

    INC @src                    ; Point to the next character
    JMP @loop                   ; Loop back and continue processing digits

@done:
    INDEXEDWR @src @sourceP
    INDEXEDWR @res @resultP
    JMP @return

@res:       .data 0
@src:       .data 0
@digit:     .data 0

    .endp

