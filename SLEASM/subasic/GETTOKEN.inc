;-----------------------------------------------------------------------------
; GETTOKEN.inc
;-----------------------------------------------------------------------------

token1: .data $+3                               ; Address of the second character int the token
token:  .data $+1
        .data 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .data 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .data 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .data 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .data 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .data 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .data 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .data 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0


    .proc GetToken

@pToken:    .data 0
@ch:        .data 0

GetToken:
    MOV token @pToken

LoopSkipSpaces:
    INDEXEDRD pProgram @ch
    CMPEQ @ch CONST_0 @eol
    CMPNE @ch ASCII_SPACE @atAcharacter
    INC pProgram
    JMP LoopSkipSpaces

@eol:
    INC pProgram
    INDEXEDWR CONST_0 @pToken
    RET

@atAcharacter:
    CMPEQ @ch ASCII_QUOTE @collectString

    CMPEQ @ch ASCII_PLUS @storeOp
    CMPEQ @ch ASCII_MINUS @storeOp
    CMPEQ @ch ASCII_ASTERISK @storeOp
    CMPEQ @ch ASCII_SLASH @storeOp
    CMPEQ @ch ASCII_EQUAL @storeOp
    CMPEQ @ch ASCII_COLON @storeOp

@loopAtAcharacter
    CMPEQ @ch CONST_0 @done
    CMPEQ @ch ASCII_SPACE @done
    CMPEQ @ch ASCII_PLUS @done
    CMPEQ @ch ASCII_MINUS @done
    CMPEQ @ch ASCII_ASTERISK @done
    CMPEQ @ch ASCII_SLASH @done
    CMPEQ @ch ASCII_EQUAL @done
    CMPEQ @ch ASCII_COLON @done

    INDEXEDWR @ch @pToken
    INC @pToken
    INC pProgram
    INDEXEDRD pProgram @ch
    JMP @loopAtAcharacter

@done:
    INDEXEDWR CONST_0 @pToken
    RET

@storeOp:
    INDEXEDWR @ch @pToken
    INC @pToken
    INC pProgram
    INDEXEDWR CONST_0 @pToken
    RET

@collectString:
    INDEXEDRD pProgram @ch      ; We store the initial quoue in the string
@loopCollectString:
    INDEXEDWR @ch @pToken
    INC @pToken
    INC pProgram
    INDEXEDRD pProgram @ch
    CMPEQ @ch ASCII_QUOTE @collectStringDone
    JMP @loopCollectString

@collectStringDone:
    INC pProgram
    INDEXEDWR CONST_0 @pToken
    RET

    .endp

