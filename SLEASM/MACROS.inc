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
        CLR tmpA
        src tmpA
        MOV tmpA src
    .endm

;--------------------------------------------------------------------------------------------

    .macro JEQZ src addr      ; Jump to addr if src is zero
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
    .macro JEQ src val addr ; JUMP to addr if src = val
        MOV src tmpA
        val tmpA
        JEQZ tmpA addr
    .endm

;--------------------------------------------------------------------------------------------

    .macro CALLP addr paramcnt
        CLR addr-(1+paramcnt)       ; Zero the return address 
        $+6 addr-(1+paramcnt)       ; Copy the return address (the .data directive)
        JMP addr         ; Call the subroutine, it will jump back to the return address
        .data 0-$-1      ; The next instruction is the return address
    .endm

;--------------------------------------------------------------------------------------------

    .macro ATOI pStr val
        MOV pStr Atoi-2
        MOV val Atoi-1
        CALLP Atoi 2
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

    .macro MULU a b       ; Unsigned multiply a by b and store in b;
            MOV a tmpA
            MOV b tmpB
            CLR b
            INC   tmpA
@loop:      CONST_1 tmpA @done         ; Subtract until aC <= 0
            ; Add a to b
            tmpB Z
            Z b
            JMP @loop
@done:
    .endm

;--------------------------------------------------------------------------------------------

    .macro DIVU a b                ; Unsigned divide b by a
        CLR tmpA
@loop3:   
        a b @endLoop        ; Subtract until <= 0
        INC   tmpA
        JMP @loop3
@endLoop:    
;        CONST_NEG1 b @done         ; If < 0
;        INC tmpA
@done:  
        MOV tmpA b
    .endm

;--------------------------------------------------------------------------------------------

    .macro INDEXEDRD srcP dest   ; Copy [srcP] to dest
        MOV srcP @p

        CLR dest        ; The next four instructions are a MOV macro expanded here inline
@p:     0 Z
        Z dest
        CLRZ
    .endm

;--------------------------------------------------------------------------------------------

    .macro INDEXEDWR src destP   ; Copy src to [destP]
        MOV destP (@p1+0) 
        MOV destP (@p1+1)
        MOV destP (@p2+1)

@p1:    0 0                     ; The next instructions are a MOV macro expanded here inline
        src Z
@p2     Z 0
        CLRZ
    .endm

;--------------------------------------------------------------------------------------------

    .macro TOGGLE01 dst
        Z dst @zorm             ; If 0 (or negative)
        CLR dst
        JMP @done
@zorm:  CLR dst
        INC dst
@done:

    .endm

    ;--------------------------------------------------------------------------------------------
