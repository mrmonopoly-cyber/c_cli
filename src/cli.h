#pragma once

#include "cli_defs.h"

// CCLI_X(LONG, L_PAD, SHORT, ARGS, S_PAD, ACTION, DESCR)
#define CCLI_ARGS_LIST \
    CCLI_X("--verbose"  ,   NO_PAD,  "-v",  NO_ARG,    ONE_PAD , CCLI_PARSE_FLAG_FUN_NAME(verbose)  , "print verbose output")      \
    CCLI_X("--help"     ,   ONE_PAD, "-h",  NO_ARG,    ONE_PAD , CCLI_PARSE_FLAG_FUN_NAME(help)     , "print this help")           \
    CCLI_X("--path"     ,   NO_PAD, "-p",  "[path]",    NO_PAD , CCLI_PARSE_FLAG_FUN_NAME(path)     , "print this help")           \

#include "c_cli.h"

static inline int cli_parse(CCliUserArgs* const restrict args, int argc, char** argv)
{
    return c_cli_parse(args, argc, argv);
}

static inline void cli_print_args(const CCliUserArgs* const restrict args)
{
    printf("verbose: %s\n", bool_to_str(args->verbose));
    printf("help : %s\n", bool_to_str(args->help));
    printf("path: %s\n", str_flag_to_str(args->path));
}
