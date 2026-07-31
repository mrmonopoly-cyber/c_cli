#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#define CCLI_NO_STRING
#define CCLI_NO_PAD CCLI_NO_STRING
#define CCLI_NO_ARG CCLI_NO_STRING

#define CCLI_1_PAD "\t"
#define CCLI_2_PAD CCLI_1_PAD CCLI_1_PAD
#define CCLI_3_PAD CCLI_1_PAD CCLI_2_PAD
#define CCLI_4_PAD CCLI_1_PAD CCLI_3_PAD
#define CCLI_5_PAD CCLI_1_PAD CCLI_4_PAD

typedef struct CCliUserArgs CCliUserArgs;

typedef enum
{
    CCliActionOK = 0,
    CCliActionMissingInput,
    CCliActionInvalidInput,
}CCliActionReturn;

#define CCLI_PARSE_FLAG_FUN_NAME(NAME) _parse_bool_flag_##NAME

#define CCLI_PARSE_BOOL_FLAG_FUN_TEMPLATE(NAME)         \
static CCliActionReturn CCLI_PARSE_FLAG_FUN_NAME(NAME)( \
        CCliUserArgs* const restrict args,              \
        int *const restrict i,                          \
        const int argc,                                 \
        char** argv)                                    \
{                                                       \
    (void) i;                                           \
    (void) argv;                                        \
    (void) argc;                                        \
    args->NAME = true;                                  \
    return CCliActionOK;                                \
}

#define CCLI_PARSE_IGNORE_FLAG_FUN_TEMPLATE(NAME)       \
static bool CCLI_PARSE_FLAG_FUN_NAME(NAME)(             \
        CCliUserArgs* const restrict args,              \
        int *const restrict i,                          \
        const int argc,                                 \
        char** argv)                                    \
{                                                       \
    (void) i;                                           \
    (void) argv;                                        \
    (void) argc;                                        \
    (void) args;                                        \
    return CCliActionOK;                                \
}

static inline const char* c_cli_next_arg(
        const int argc,
        char** argv,
        int* const restrict i)
{
    (*i)++;
    if((*i) < argc)
    {
        return argv[*i];
    }

    return NULL;
}

static void c_cli_print_help_to(const char* const prog_name, FILE* const restrict file);

static inline void c_cli_print_help(const char* const prog_name)
{
    c_cli_print_help_to(prog_name, stderr);
}

static inline const char* bool_to_str(const bool flag)
{
    return flag ? "True" : "False";
}

static inline const char* str_flag_to_str(const char* const restrict str_flag)
{
    return str_flag ? str_flag : "(Nill)";
}
