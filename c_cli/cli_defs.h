#pragma once

#include "c_cli_helper.h"

#define EMPTY   CCLI_NO_STRING
#define NO_PAD  CCLI_NO_PAD
#define NO_ARG  CCLI_NO_ARG
#define ONE_PAD CCLI_1_PAD
#define TWO_PAD CCLI_2_PAD

struct CCliUserArgs{
    bool verbose;
    bool help;
    const char* path;
};

CCLI_PARSE_BOOL_FLAG_FUN_TEMPLATE(verbose)
CCLI_PARSE_IGNORE_FLAG_FUN_TEMPLATE(help)
static inline CCliActionReturn CCLI_PARSE_FLAG_FUN_NAME(path)(
        CCliUserArgs* const restrict args,
        int *const restrict i,
        const int argc,
        char** argv)
{
    const char* path_arg = c_cli_next_arg(argc, argv, i);

    if (!path_arg)
    {
        return CCliActionMissingInput;
    }
    else
    {
        args->path = path_arg;
    }

    return CCliActionOK;
}
