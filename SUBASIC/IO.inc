
;-----------------------------------------------------------------------------------------

        .PROC PrintInt

@val:  .data 0  ; Value to print
@t:    .data 0  ; Temporary variable

PrintInteger:
        POP @t                          ; Pop off the return address and parameters
        POP @val
        PUSH @t                         ; Push the return address back onto the stack

;
; Start by checking if the value is zero, negative or positive
;       For zero then explicitly print a zero and return
;       For negative values, print a minus sign and negate the value
;       For positive values, carry on as normal
;

        JNEZ @val @valIsNotZero         ; If the value is zero, print it and return
        OUT ASCII_0                     ; Print ASCII zero
        RET                             ; All done already, return             

@valIsNotZero:  
        Z @val @valIsNeg                ; Check if the value is negative
        JMP @startprint                 ; If it is positive, print it as is        

@valIsNeg:      
        OUT ASCII_MINUS                 ; If is is negative print a minus sign
        NEG @val                        ; ...and negate the number so it can be printed normally

;
; Now we have a positive number to print
; 
; We will print the number by subtracting the largest possible power of 10 from the number
; and printing the number of times we can subtract it. This process is repeated for all
; smaller powers of 10 until we have printed all digits.
;
; The flag @blanking is used to skip printing leading zeros
;

@startprint:    SET1 @blanking                  ; Assume we have a leading zeros and is in blanking mode
                MOV CONST_7 @digitCnt           ; Number of digits to print
                MOV POW10_ADDR @pow10addr       ; Initialize the pointer to the POW10 array

@digitsLoop:    MOV ASCII_0 @digit              ; Initialize counter to ASCII zero so we don't need to convert it later
@l1:            MOV @val @lastPosVal
@pow10addr:     0 @val @l2                      ; Subtract from val, and branch if reached zero or negative
@NotNeg:         INC @digit                      ; We still have more to subtract, so increment the counter
                DEC @blanking                   ; Mark zeroFlag to start printing
                JMP @l1

@l2:            JEQZ @val @NotNeg                  ; If T is not negative yet then we are not done, so continue subtracting
                Z @blanking @print
                JMP @noPrint

@print:         OUT @digit                      ; Print the digit

@noPrint:       MOV @lastPosVal @val            ; Restore the value of val
                INC @pow10addr                  ; Move to the next smaller power of 10
                DEC @digitCnt                   ; Decrement the digit counter
                Z @digitCnt @done               ; If we have printed all digits, we are done
                JMP @digitsLoop                 ; Otherwise, continue with the next digit

@done:          RET

@blanking:      .data 0                         ; <=0: print, >0: skip printing as we're in zero blanking mode
@digitCnt:      .data 0                         ; Number of digits to print
@digit:         .data 0                         ; ASCII digit to be printed
@lastPosVal:    .data 0                         ; Last positive value of val, sp it can be restored when we're counted up enough
        .endp


;-----------------------------------------------------------------------------------------

    .proc PrintString
;
@p:    .data 0  ; Pointer to the string to print
@t:    .data 0  ; Temporary variable
;
PrintString:
    POP @t              ; Pop off the return address and parameters
    POP @p
    PUSH @t             ; Push the return address back onto the stack
@loop:
    INDEXEDRD @p @t     ; Get the next character
    JEQZ @t @stop       ; If it's a null terminator, we're done
    OUT @t              ; Otherwise, print the character
    INC @p              ; Move to the next character
    JMP @loop           ; Repeat
@stop:
    RET
    .endp


;
; Powers of 10 used by the PrintInteger routine
;

POW10:          .data 1000000,100000,10000,1000,100,10,1
POW10_ADDR:     .data POW10

;
; Powers of 16 used by the PrintHex routine
;

POW16:          .data 0x100000,0x10000,0x1000,0x100,0x10,0x1
POW16_ADDR:     .data POW16


;-----------------------------------------------------------------------------------------

        .PROC PrintHex

@val:  .data 0  ; Value to print
@t:    .data 0  ; Temporary variable

PrintHex:
        POP @t                          ; Pop off the return address and parameters
        POP @val
        PUSH @t                         ; Push the return address back onto the stack

        JNEZ @val @valIsNotZero         ; If the value is zero, print it and return
        OUT ASCII_0                     ; Print ASCII zero
        OUT ASCII_0                     ; Print ASCII zero
        OUT ASCII_0                     ; Print ASCII zero
        OUT ASCII_0                     ; Print ASCII zero
        OUT ASCII_0                     ; Print ASCII zero
        OUT ASCII_0                     ; Print ASCII zero
        RET                             ; All done already, return             

@valIsNotZero:  
; 
; We will print the number by subtracting the largest possible power of 16 from the number
; and printing the number of times we can subtract it. This process is repeated for all
; smaller powers of 16 until we have printed all digits.
;

@startprint:    MOV CONST_6 @digitCnt           ; Number of digits to print
                MOV POW16_ADDR @pow16addr       ; Initialize the pointer to the POW10 array

@digitsLoop:    MOV ASCII_0 @digit              ; Initialize counter to ASCII zero so we don't need to convert it later
@l1:            MOV @val @lastPosVal
@pow16addr:     0 @val @l2                      ; Subtract from val, and branch if reached zero or negative
@NotNeg:         INC @digit                      ; We still have more to subtract, so increment the counter
                JMP @l1

@l2:            JEQZ @val @NotNeg               ; If T is not negative yet then we are not done, so continue subtracting
@print:         JLT @digit ASCII_9+1 @print1       ; If the digit is less than 10, print it as is
                ADD CONST_7 @digit        ; Otherwise, convert it to a letter
@print1:        OUT @digit                      ; Print the digit
                MOV @lastPosVal @val            ; Restore the value of val
                INC @pow16addr                  ; Move to the next smaller power of 10
                DEC @digitCnt                   ; Decrement the digit counter
                Z @digitCnt @done               ; If we have printed all digits, we are done
                JMP @digitsLoop                 ; Otherwise, continue with the next digit

@done:          RET

@digitCnt:      .data 0                         ; Number of digits to print
@digit:         .data 0                         ; ASCII digit to be printed
@lastPosVal:    .data 0                         ; Last positive value of val, sp it can be restored when we're counted up enough
        .endp


;-----------------------------------------------------------------------------------------
