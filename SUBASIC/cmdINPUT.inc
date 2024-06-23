;-----------------------------------------------------------------------------
cmdINPUT:        .proc cmdINPUT
;-----------------------------------------------------------------------------
;
; Read a value from the user and store it in a variable
;
; INPUT A
;
;-------|-----------------------------------|-------------------------------------

       CALL GetParsedToken
       MOV tokenAddr @var1     ; Get the address of the variable to receive the input data

       CALL InputString
       PUSH buf
       CALL ATOI
       INDEXEDWR atoiVal @var1

       PRINTCRLF
       JMP Statement

@var1:        .data 0

        .endp
