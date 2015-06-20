#ifndef _AVRKIT_CLI_PARSER_H_
#define _AVRKIT_CLI_PARSER_H_

#include <stdint.h>
#include "settings.h"

#define CLI_MAX_LENGTH_PAD      CLI_MAX_LENGTH+1
#define CLI_ERROR_MAX_ARGS      -1
#define CLI_ERROR_MAX_LENGTH    -2

struct CLI_Parser_Context{
    uint8_t currentArg;
    uint8_t currentMode;
    uint8_t currentSize;
    char args[CLI_MAX_ARGS][CLI_MAX_LENGTH_PAD];
};

void    CLI_Parser_Clear        (struct CLI_Parser_Context * ctx);
int8_t  CLI_Parse_Character     (struct CLI_Parser_Context * ctx, char ch);

#endif
