;--------------------------------------------------------------------------------------------

    .macro HALT 
        Z Z (-1)            ; Jump to -1 which halts execution
    .endm

;--------------------------------------------------------------------------------------------

    .macro CLRZ
        Z Z 
    .endm

;--------------------------------------------------------------------------------------------

    .macro CLR addr
        addr addr
    .endm

;--------------------------------------------------------------------------------------------

    .macro SET1 addr
        CLR addr
        INC addr
    .endm

;--------------------------------------------------------------------------------------------

    .macro OUT addr
        addr (-1)
    .endm

;--------------------------------------------------------------------------------------------

    .macro IN addr
        (-1) addr
    .endm

;--------------------------------------------------------------------------------------------

    .macro INC addr
        MINUS1 addr
    .endm

;--------------------------------------------------------------------------------------------

    .macro DEC addr
        PLUS1 addr
    .endm

;--------------------------------------------------------------------------------------------

    .macro ADD a b         ; Add a to b and store in b
        a Z                ; Z = -a
        Z b                ; b = b - Z = b - -a = b + a
        CLRZ
    .endm

;--------------------------------------------------------------------------------------------

    .macro ADD3 a b c      ; Add a to b and store in c
        MOV a c
        ADD b c
    .endm

;--------------------------------------------------------------------------------------------

    .macro MOV src dest      ; Copy value at src to dest using temp as a temporary location
        CLR dest
        src Z
        Z dest
        CLRZ
    .endm
    
;--------------------------------------------------------------------------------------------

    .macro JMP addr
        Z Z addr
    .endm

;--------------------------------------------------------------------------------------------

    .macro NEG src
        CLR T
        src T
        MOV T src
    .endm

;--------------------------------------------------------------------------------------------

    .macro JEQZ src addr    ; Jump to addr if src is zero
            Z src @lte
            Z Z @done
@lte:       src Z @gte
            Z Z @done
@gte:       Z Z addr
@done:
    .endm


;--------------------------------------------------------------------------------------------

    .macro JNEZ src addr    ; Jump to addr if src is not zero
        Z src @lte
        Z Z addr
@lte:   src Z @gte
        Z Z addr
@gte:   Z Z
    .endm

;--------------------------------------------------------------------------------------------

    .macro JGEZ src addr    ; Jump to addr if src is greater than or equal to zero
        src Z @gte
        Z Z @done
@gte:   Z Z addr
@done:  Z Z
    .endm

;--------------------------------------------------------------------------------------------

    .macro JNEG src addr    ; Jump to addr if src is negative
        JGEZ src @done
        JMP addr
@done:
    .endm      
;--------------------------------------------------------------------------------------------

    .macro CALLP addr paramcnt
        CLR addr-(1+paramcnt)       ; Zero the return address 
        $+6 addr-(1+paramcnt)       ; Copy the return address (the .data directive)
        JMP addr         ; Call the subroutine, it will jump back to the return address
        .data 0-$-1      ; The next instruction is the return address
    .endm

;--------------------------------------------------------------------------------------------

    .macro PRINTSTRING addr
        MOV addr Print_String+1
        MOV addr Print_String+3
        CALLP Print_String 0
    .endm

;--------------------------------------------------------------------------------------------

    .macro PRINTINTEGER addr
        MOV addr Print_Integer-1
        CALLP Print_Integer 1
    .endm

;--------------------------------------------------------------------------------------------

    .macro PRINTCRLF 
        OUT CONST_CR
        OUT CONST_LF
    .endm

;--------------------------------------------------------------------------------------------


    .macro MULU a b       ; Multiply a by b and store in b;
            MOV  a tmpA
            MOV  b tmpB
            CLR  b
            INC   tmpA
@loop:      CONST_1 tmpA @done         ; Subtract until aC <= 0
            ; Add a to b
            tmpB Z
            Z b
            Z Z loop
@done:
    .endm

