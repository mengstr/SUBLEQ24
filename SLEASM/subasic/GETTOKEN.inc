    .proc GETTOKEN
;
; Modifies pProgram and pToken
;

@return:
    JMP 0

GETTOKEN:
        MOV token_ pToken               ; Reset the token pointer
        CLR @inQuote                    ; Assume we are not in a quote

@loop:
    INDEXEDRD pProgram ch               ; Get a char from the program line
    JEQZ ch @done                       ; If the char is null, we are done

    CONST_QUOTE ch                      ; If the char is a quote, toggle the inQuote flag
    JNEZ ch @notAquote                  ; If the char is not a quote, continue
    TOGGLE01 @inQuote                   ; Toggle the inQuote flag
@notAquote:
    ADD CONST_QUOTE ch                  ; Restore the char to its original value after comparisons

    JNEZ @inQuote @skipSpaceCheck       ; If we are in a quote, dont check for spaces    

    CONST_SPACE ch @done                ; If the char is a space, we are done
    ADD CONST_SPACE ch                  ; Restore the char to its original value after comparisons

@skipSpaceCheck:
    INC pProgram
    INDEXEDWR ch pToken                 ; Write the character to the token buffer
    INC pToken  
    JMP @loop
@done:
    INDEXEDWR Z pToken                  ; Null terminate the token buffer
    MOV token_ pToken                   ; Reset the token pointer before returning so we can start using it directly

    JMP @return

@inQuote:   .data 0                     ; 0: not in quote, 1: in quote

    .endp
