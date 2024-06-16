;-----------------------------------------------------------------------------
; KEYBOARD.inc
;-----------------------------------------------------------------------------

    .proc InputString

@ch:            .data 0                 ; Character read from the keyboard
@cnt:           .data 0                 ; Character count
@p:             .data 0                 ; Pointer to the buffer


InputString:
        CLR @cnt                        
        INDEXEDWR CONST_0 buf            ; Clear the buffer

@loop:
        IN @ch                          ; Read a character from the keyboard
        JNEG @ch @loop                  ; Continue looping until a character is read

        CMPEQ @ch ASCII_CR @done        ; Exit the loop if the character is a carriage return
        CMPEQ @ch ASCII_DEL @delete     ; Remove character in buffer if delete or backspace is pressed
        CMPEQ @ch ASCII_BS @delete

        MOV buf @p                          ; Calculate the position in the buffer
        ADD @cnt @p
        INDEXEDWR @ch @p                    ; Store the character in the buffer
        INC @p                              ; Store a null terminator in the buffer
        INDEXEDWR CONST_0 @p
        INC @cnt                            ; Increment the character count
        OUT @ch
        JMP @loop                           ; Continue looping

@delete:
        CMPEQ @cnt CONST_0 @loop            ; Don't backspace past the start of the buffer
        DEC @cnt                            ; Decrement the character count
        OUT ASCII_BS                        ; Clear the character from the screen
        OUT ASCII_SPACE
        OUT ASCII_BS
        JMP @loop                           ; Continue looping

@done:
    RET

    .endp
