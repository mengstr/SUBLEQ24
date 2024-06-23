;--------------------------------------------------------------------------------------------

    .macro HALT 
        ; MACRO START - HALT
        Z Z (-1)            ; Jump to -1 which halts execution
        ; MACRO END - HALT
    .endm

;--------------------------------------------------------------------------------------------

    .macro CLRZ
        ; MACRO START - CLRZ
        Z Z 
        ; MACRO END - CLRZ
    .endm

;--------------------------------------------------------------------------------------------

    .macro CLR addr
        ; MACRO START - CLR
        addr addr
        ; MACRO END - CLR
    .endm

;--------------------------------------------------------------------------------------------

    .macro SET1 addr
        ; MACRO START - SET1
        CLR addr
        INC addr
        ; MACRO END - SET1
    .endm

;--------------------------------------------------------------------------------------------

    .macro OUT addr
        ; MACRO START - OUT
        addr (-1)
        ; MACRO END - OUT
    .endm

;--------------------------------------------------------------------------------------------

    .macro IN addr
        ; MACRO START - IN
        (-1) addr
        ; MACRO END - IN
    .endm

;--------------------------------------------------------------------------------------------

    .macro INC addr
        ; MACRO START - INC
        MINUS1 addr
        ; MACRO END - INC
    .endm

;--------------------------------------------------------------------------------------------

    .macro DEC addr
        ; MACRO START - DEC
        PLUS1 addr
        ; MACRO END - DEC
    .endm

;--------------------------------------------------------------------------------------------

    .macro SUB a b         ; Subtract a from b and store in b
        ; MACRO START - SUB
        a b
        ; MACRO END - SUB
    .endm

;--------------------------------------------------------------------------------------------

    .macro ADD a b         ; Add a to b and store in b
        ; MACRO START - ADD
        a Z                ; Z = -a
        Z b                ; b = b - Z = b - -a = b + a
        CLRZ
        ; MACRO END - ADD
    .endm

;--------------------------------------------------------------------------------------------

    .macro ADD3 a b c      ; Add a to b and store in c
        ; MACRO START - ADD3
        MOV a c
        ADD b c
        ; MACRO END - ADD3
    .endm

;--------------------------------------------------------------------------------------------

    .macro MOV src dest      ; Copy value at src to dest
        ; MACRO START - MOV
        CLR dest
        src Z
        Z dest
        CLR Z
        ; MACRO END - MOV
    .endm
    
;--------------------------------------------------------------------------------------------

    .macro JMP addr         ; Ucondtional JUMP
        ; MACRO START - JMP
        Z Z addr
        ; MACRO END - JMP
    .endm

;--------------------------------------------------------------------------------------------

    .macro NEG src          ; Negate src
        ; MACRO START - NEG
        CLR tmpA
        src tmpA
        MOV tmpA src
        ; MACRO END - NEG
    .endm

;--------------------------------------------------------------------------------------------

    .macro JEQZ src addr      ; Jump to addr if src is zero
        ; MACRO START - JEQZ
        Z src @lte
        Z Z @done
@lte:   src Z @gte
        Z Z @done
@gte:   Z Z addr
@done:
        ; MACRO END - JEQZ
    .endm

;--------------------------------------------------------------------------------------------

    .macro JNEZ src addr    ; Jump to addr if src is not zero
        ; MACRO START - JNEZ
        Z src @lte
        Z Z addr
@lte:   src Z @gte
        Z Z addr
@gte:   Z Z
        ; MACRO END - JNEZ
    .endm

;--------------------------------------------------------------------------------------------

    .macro JGEZ src addr    ; Jump to addr if src is greater than or equal to zero
        ; MACRO START - JGEZ
        MOV src tmpA 
        CLR tmpB
        tmpA tmpB addr
        ; MACRO END - JGEZ
    .endm

;--------------------------------------------------------------------------------------------

    .macro JNEG src addr    ; Jump to addr if src is negative
        ; MACRO START - JNEG
        JGEZ src @done
        JMP addr
@done:
        ; MACRO END - JNEG
    .endm      

;--------------------------------------------------------------------------------------------

    .macro JPOS src addr    ; Jump to addr if src is positive ( >0 )
        ; MACRO START - JPOS
        JNEG src @done
        JEQZ src @done
        JMP addr
@done:
        ; MACRO END - JPOS
    .endm      

;-------------------------------------------------------------------------------------------

    .macro CMPEQ v1 v2 addr ; Compare and jump to addr if v1 = v2
        ; MACRO START - CMPEQ
        MOV v2 tmpA
        v1 tmpA
        JEQZ tmpA addr
        ; MACRO END - CMPEQ
    .endm

;--------------------------------------------------------------------------------------------

    .macro CMPNE v1 v2 addr ; Compare and jump to addr if v1 != v2
        ; MACRO START - CMPNE
        MOV v1 tmpA
        v2 tmpA
        JNEZ tmpA addr
        ; MACRO END - CMPNE
    .endm

;--------------------------------------------------------------------------------------------

    .macro CMPGEQ v1 v2 addr ; Compare and jump to addr if v1 >= v2
        ; MACRO START - CMPGEQ
        MOV v1 tmpC
        v2 tmpC
        JEQZ tmpC addr
        JPOS tmpC addr
        ; MACRO END - CMPGEQ
    .endm

;--------------------------------------------------------------------------------------------


    .macro CMPLEQ v1 v2 addr ; Compare and jump to addr if v1 <= v2
        ; MACRO START - CMPGEQ
        MOV v1 tmpC
        v2 tmpC
        JEQZ tmpC addr
        JNEG tmpC addr
        ; MACRO END - CMPGEQ
    .endm

;--------------------------------------------------------------------------------------------

    .macro PRINTCRLF    
        ; MACRO START - PRINTCRLF
        OUT ASCII_CR
        OUT ASCII_LF
        ; MACRO END - PRINTCRLF
    .endm

;--------------------------------------------------------------------------------------------

    .macro MULU a b       ; Unsigned multiply a by b and store in b;
        ; MACRO START - MULU        
        MOV a tmpA
        MOV b tmpB
        CLR b
        INC   tmpA
@loop:  CONST_1 tmpA @done         ; Subtract until aC <= 0
        ; Add a to b
        tmpB Z
        Z b
        JMP @loop
@done:
        ; MACRO END - MULU
    .endm

;--------------------------------------------------------------------------------------------

    .macro DIVU a b                ; Unsigned divide b by a
        ; MACRO START - DIVU        
        CLR tmpA
@loop3:   
        a b @endLoop        ; Subtract until <= 0
        INC   tmpA
        JMP @loop3
@endLoop:    
        CONST_NEG1 b @done         ; If < 0
        INC tmpA
@done:  
        MOV tmpA b
        ; MACRO END - DIVU
    .endm

;--------------------------------------------------------------------------------------------

    .macro INDEXEDRD srcP dest   ; Copy [srcP] to dest
        ; MACRO START - INDEXEDRD
        MOV srcP @p
        CLR dest        ; The next four instructions are a MOV macro expanded here inline
@p:     0 Z
        Z dest
        CLRZ
        ; MACRO END - INDEXEDRD
    .endm

;--------------------------------------------------------------------------------------------

    .macro INDEXEDWR src destP   ; Copy src to [destP]
        ; MACRO START - INDEXEDWR
        MOV destP (@p1+0) 
        MOV destP (@p1+1)
        MOV destP (@p2+1)

@p1:    0 0                     ; The next instructions are a MOV macro expanded here inline
        src Z
@p2     Z 0
        CLRZ
        ; MACRO END - INDEXEDWR
    .endm

;--------------------------------------------------------------------------------------------

    .macro TOGGLE01 dst
        ; MACRO START - TOGGLE01
        Z dst @zorm             ; If 0 (or negative)
        CLR dst
        JMP @done
@zorm:  CLR dst
        INC dst
@done:
        ; MACRO END - TOGGLE01
    .endm

    ;--------------------------------------------------------------------------------------------

    .macro JGE src val addr    ; Jump to addr if src is greater than or equal to val
        ; MACRO START - JGE
        MOV src tmpA
        MOV val tmpB
        tmpA tmpB addr
        ; MACRO END - JGE
    .endm

    ;--------------------------------------------------------------------------------------------

    .macro JGT src val addr    ; Jump to addr if src is greater than val
        ; MACRO START - JGT        
        MOV src tmpA
        MOV val tmpB
        INC tmpB
        tmpA tmpB addr
        ; MACRO END - JGT
    .endm

    ;--------------------------------------------------------------------------------------------

    .macro JLTE src val addr       ; Jump if src is less than or equal to val
        ; MACRO START - JLTE
        JGT src val @done
        JMP addr
@done:
        ; MACRO END - JLTE
    .endm

    ;--------------------------------------------------------------------------------------------

    .macro JLT src val addr       ; Jump if src is less than val
        ; MACRO START - JLT
        JGT src val @done
        CMPEQ src val @done
        JMP addr
@done:
    .endm


    ;--------------------------------------------------------------------------------------------
    .macro JBETWEEN val A B addr    ; Jump to addr if val is between A and B (inclusive)
    ; MACRO START - JBETWEEN
    JLT val A @done
    JGT val B @done
    JMP addr
@done:
    ; MACRO END - JBETWEEN    
    .endm

    ;--------------------------------------------------------------------------------------------

   .macro PUSH var
        ; MACRO START - PUSH
        INDEXEDWR var SP
        INC SP
        ; MACRO END
    .endm

    ;--------------------------------------------------------------------------------------------

    .macro POP var
        ; MACRO START - POP
        DEC SP
        INDEXEDRD SP var
        ; MACRO END
    .endm

    ;--------------------------------------------------------------------------------------------

  .macro CALL addr
        ; MACRO START - CALL
        PUSH @ra
        JMP addr
@ra:    .data $+1
        ; MACRO END
    .endm

    ;--------------------------------------------------------------------------------------------

    .macro RET
        ; MACRO START - RET
        POP @j
        .data 0,0
@j:     .data 0x555555
        ; MACRO END
    .endm

    ;--------------------------------------------------------------------------------------------


    .macro DEBUGPRINT str
;        PUSH cyan                   ;debug
;        CALL PrintString           ;debug
;        PUSH @yo                   ;debug
;        CALL PrintString           ;debug
;        JMP @bo                    ;debug
;@yo:    .data $+1                  ;debug
;        .data str                  ;debug
;        .data 0                    ;debug
;@bo:                               ;debug
;        PUSH white                   ;debug
;        CALL PrintString           ;debug
    .endm


    .macro cyanChar c
;    PUSH cyan          ;debug
;    CALL PrintString   ;debug
;    OUT c              ;debug
;    PUSH white         ;debug
;    CALL PrintString   ;debug
    .endm
