#include <stdbool.h>
#include <stdint.h>

typedef struct CCliUserArgs{
    bool help;
    bool verbose;
    uint8_t dig;
}Args;

static inline bool parse_args(Args* args, const int argc, char** argv);


int main(int argc, char *argv[])
{
    Args args = {0};

    if(!parse_args(&args, argc, argv))
    {
        return -1;
    }

    return 0;
}


#define CCLI_IMPLEMENTATION
#include "c_cli.h"

CCLI_PARSER_DECLARE(digit);

static CCliArgDef defs[]= 
{
    //--digit, -i
    {
        .f_long = CCLI_LONG_FLAG(digit),
        .f_short = CCLI_SHORT_FLAG(d),
        .f_args = 
        {
            CCLI_NEW_ARG(digit, CCliArgU8),
        },
        .f_description = "random digit",
        .f_parser = CCLI_PARSER_NAME(digit),
    },
};

static inline bool parse_args(Args* args, const int argc, char** argv)
{
    return c_cli_parse(
            defs,
            CCLI_ARRAYSIZE(defs),
            args,
            argc,
            argv,
            NULL);
}

CCLI_PARSER_DECLARE_FULL(digit, args, ctx)
{
    return c_cli_parse_next_arg_uint8_t(ctx, &args->dig);
}
