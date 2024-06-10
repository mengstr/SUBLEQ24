;-----------------------------------------------------------------------------------------

        .PROC PrintInt

Print_Integer_Return:
                JMP 0                           ; Jump/Return instrcution to be modified by caller
@val:           .data 0                         ; Value to be printed       

;
; Start by checking if the value is zero, negative or positive
;       For zero then explicitly print a zero and return
;       For negative values, print a minus sign and negate the value
;       For positive values, carry on as normal
;

Print_Integer:
                JNEZ @val @valIsNotZero         ; If the value is zero, print it and return
                OUT CONST_ASCII_0               ; Print ASCII zero
                JMP Print_Integer_Return        ; ...and exit

@valIsNotZero:  Z @val @valIsNeg                ; Check if the value is negative
                JMP @startprint                 ; If it is positive, print it as is        

@valIsNeg:      OUT CONST_MINUS                 ; If is is negative print a minus sign
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

@digitsLoop:    MOV CONST_ASCII_0 @digit        ; Initialize counter to ASCII zero so we don't need to convert it later
@l1:            MOV @val @lastPosVal
@pow10addr:     0 @val @l2                      ; Subtract from val, and branch if reached zero or negative
NotNeg:         INC @digit                      ; We still have more to subtract, so increment the counter
                DEC @blanking                   ; Mark zeroFlag to start printing
                JMP @l1

@l2:            JEQZ @val NotNeg                  ; If T is not negative yet then we are not done, so continue subtracting
                Z @blanking @print
                JMP @noPrint

@print:         OUT @digit                      ; Print the digit

@noPrint:       MOV @lastPosVal @val            ; Restore the value of val
                INC @pow10addr                  ; Move to the next smaller power of 10
                DEC @digitCnt                   ; Decrement the digit counter
                Z @digitCnt Print_Integer_Return; If we have printed all digits, we are done
                JMP @digitsLoop                 ; Otherwise, continue with the next digit

@blanking:      .data 0                         ; <=0: print, >0: skip printing as we're in zero blanking mode
@digitCnt:      .data 0                         ; Number of digits to print
@digit:         .data 0                         ; ASCII digit to be printed
@lastPosVal:    .data 0                         ; Last positive value of val, sp it can be restored when we're counted up enough
        .endp


;-----------------------------------------------------------------------------------------

Print_String_Return:
        JMP 0                   ; Jump/Return instrcution to be modified by caller
Print_String:
        Z 0 Print_String_Return
        OUT 0
        INC Print_String+1
        INC Print_String+3
        JMP Print_String


;
; Powers of 10 used by the Print_Integer routine
;

POW10:          .data 1000000,100000,10000,1000,100,10,1
POW10_ADDR:     .data POW10
