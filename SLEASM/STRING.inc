    .proc ATOI

@p:         .data 0  ; Pointer to the string to convert
@t:         .data 0  ; Temporary hold variable
@digit:     .data 0

ATOI:
    POP @t              ; Pop off the return address and parameters
    POP @p
    PUSH @t             ; Push the return address back onto the stack

    CLR atoiVal

@loop:
    INDEXEDRD @p @digit
    JBETWEEN @digit ASCII_0 ASCII_9 @valid ; If the character is not a digit, we are done
    RET

@valid:
    ASCII_0 @digit              ; Make the ascii digit to a number
    MULU CONST_10 atoiVal          ; Collect the digit into the result
    ADD @digit atoiVal

    INC @p                      ; Point to the next character
    JMP @loop                   ; Loop back and continue processing digits

    .endp



    .proc CompareString
@t:     .data 0
@s1:    .data 0
@s2:    .data 0
@ch1:   .data 0
@ch2:   .data 0

CompareString:
    POP @t              ; Pop off the return address and parameters
    POP @s2
    POP @s1
    PUSH @t             ; Push the return address back onto the stack

    CLR compResult

@loop:
    INDEXEDRD @s1 @ch1
    INDEXEDRD @s2 @ch2
    CMPNE @ch1 @ch2 @fail
    JNEZ @ch1 @next
    JNEZ @ch2 @next
    INC compResult
    RET

@next:
    INC @s1
    INC @s2
    JMP @loop

@fail:
    RET

    .endp


    .proc CompareStringIgnoreCase
@t:     .data 0
@s1:    .data 0
@s2:    .data 0
@ch1:   .data 0
@ch2:   .data 0

CompareStringIgnoreCase:
        POP @t              ; Pop off the return address and parameters
        POP @s2
        POP @s1
        PUSH @t             ; Push the return address back onto the stack

        CLR compResult
@loop:
        INDEXEDRD @s1 @ch1
        INDEXEDRD @s2 @ch2

        JBETWEEN @ch1 ASCII_a ASCII_z @uc1  ; If the character is a lower case letter, convert it to upper case
        JMP @uc2
@uc1:   CONST_32 @ch1
@uc2:   JBETWEEN @ch2 ASCII_a ASCII_z @uc3
        JMP @uc4
@uc3:   CONST_32 @ch2
@uc4:   

        CMPNE @ch1 @ch2 @fail
        JNEZ @ch1 @next
        JNEZ @ch2 @next
        INC compResult
        RET

@next:
        INC @s1
        INC @s2
        JMP @loop

@fail:
        RET
    .endp
