
#include "cli_parser.h"

#include <string.h>
#include <stdio.h>

void CLI_Parser_Clear(struct CLI_Parser_Context * ctx){
    ctx->currentArg = 0;
    ctx->currentMode = 0;
    ctx->currentSize = 0;
    for(uint8_t i = 0;i < CLI_MAX_ARGS;i++)
        memset(ctx->args[i],0,CLI_MAX_LENGTH_PAD);
}

int8_t CLI_Parse_Character(struct CLI_Parser_Context * ctx, char ch)
{
    if(ctx->currentArg >= CLI_MAX_ARGS)
        return CLI_ERROR_MAX_ARGS;
    
    if(ch == ' ' || ch == '\t'){
        if(ctx->currentMode == 0){
            ctx->currentArg++;
            ctx->currentSize = 0;
            ctx->currentMode = 1;
        }
    }
    else{
        if(ctx->currentSize >= CLI_MAX_LENGTH)
            return CLI_ERROR_MAX_LENGTH;
        ctx->args[ctx->currentArg][ctx->currentSize++] = ch;
        ctx->args[ctx->currentArg][ctx->currentSize] = '\0';
        ctx->currentMode = 0;
    }
    
    return ctx->currentArg;
}
