#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "c_cli_defs.h"

#define CCLI_PARSER_NAME(NAME)CCliParser_##NAME

#define CCLI_PARSER_DECLARE_FULL(NAME, args, ctx)                                               \
    CCliActionReturn CCLI_PARSER_NAME(NAME)(                                                    \
            struct CCliUserArgs* const restrict args,                                           \
            void* const restrict ctx)                                                           \

#define CCLI_PARSER_DECLARE(NAME) CCLI_PARSER_DECLARE_FULL(NAME, args, ctx)

static inline CCLI_PARSER_DECLARE_FULL(__ignore_flag, args, ctx)
{
    (void) args;
    (void) ctx;
    return CCliActionOK;
}

static inline const char* c_cli_bool_to_str(const bool val)
{
    return val ? "True" : "False";
}

static inline const char* c_cli_str_arg_to_str(const char* const restrict arg)
{
    return arg ? arg : "(Nill)";
}

static inline const char* c_cli_next_arg(void* const restrict ctx)
{
    CCliParseCtx* p_ctx = ctx;
    if(*p_ctx->i < p_ctx->argc)
    {
        (*p_ctx->i)++;
        return p_ctx->argv[*p_ctx->i];
    }

    return NULL;
}
