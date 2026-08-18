#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct CCliUserArgs{
    bool help;
    bool verbose;
    uint64_t big_uint;
    int64_t big_int;
}Args;

static inline bool parse_args(Args* args, const int argc, char** argv);


int main(int argc, char *argv[])
{
    Args args = {0};

    if(!parse_args(&args, argc, argv))
    {
        return -1;
    }

    printf("big_uint: %lu, big_int: %ld\n", args.big_uint, args.big_int);

    return 0;
}


#define CCLI_DEPLOY
#include "c_cli.h"

CCLI_PARSER_DECLARE(very_long_flag);

static const CCliArgDef cli_def[] = 
{
    {
        .f_long = CCLI_LONG_FLAG(flag),
        .f_short = CCLI_SHORT_FLAG(f),
        .f_args = 
        {
            CCLI_NEW_ARG(big_uint, CCliArgU64),
            CCLI_NEW_ARG(big_int, CCliArgS64),
        },
        .f_description = "very long flag with a long arg",
        .f_parser = CCLI_PARSER_NAME(very_long_flag),
        .f_attributes = CCliFlagAttribute_ArgsList,
    },
};

static inline bool parse_args(Args* args, const int argc, char** argv)
{
    return c_cli_parse(
            cli_def,
            CCLI_ARRAYSIZE(cli_def),
            args,
            argc,
            argv,
            NULL);
}

CCLI_PARSER_DECLARE_FULL(very_long_flag, args, ctx)
{
    CCliActionReturn res = CCliActionMissingInput;

    if((res = c_cli_parse_next_arg_uint64_t(ctx, &args->big_uint))) goto end;
    if((res = c_cli_parse_next_arg_int64_t(ctx, &args->big_int))) goto end;

end:
    return res;
}
