#include <stdbool.h>

typedef struct CCliUserArgs{
    bool help;
    bool verbose;
    const char* path;
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


#define CCLI_DEPLOY
#include "c_cli.h"

CCLI_PARSER_DECLARE(very_long_flag);

static const CCliArgDef cli_def[] = 
{
    {
        .f_long = CCLI_LONG_FLAG(very_long_flag),
        .f_short = CCLI_SHORT_FLAG(vllf),
        .f_args = 
        {
            CCLI_NEW_ARG(long_path, CCliArgStr),
            CCLI_NEW_ARG(big_uint, CCliArgU64),
            CCLI_NEW_ARG(big_int, CCliArgS64),
        },
        .f_description = "very long flag with a long arg",
        .f_parser = CCLI_PARSER_NAME(very_long_flag),
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
    return c_cli_parse_nex_arg_str(ctx, &args->path);
}
