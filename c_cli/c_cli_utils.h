#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

#include "c_cli_defs.h"

#define CCLI_STRINGIFY(STR) #STR

#define CCLI_LONG_FLAG(NAME) "--"#NAME
#define CCLI_SHORT_FLAG(NAME) "-"#NAME

#define CCLI_NEW_ARG(name, type) {#name, type}
#define CCLI_NO_ARG {CCLI_NEW_ARG(NULL, 0)}

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

static inline const char* c_cli_arg_type_to_str(const CCliArgType arg_type)
{
    const char* const restrict c_cli_args_types[__Count__CCliArg] =
    {
        [CCliArgVoid]   = "void",

        [CCliArgBool]   = "bool",

        [CCliArgChar]   = "char",
        [CCliArgStr]    = "str",

        [CCliArgU8]     = "u8",
        [CCliArgU16]    = "u16",
        [CCliArgU32]    = "u32",
        [CCliArgU64]    = "u64",

        [CCliArgS8]     = "s8",
        [CCliArgS16]    = "s16",
        [CCliArgS32]    = "s32",
        [CCliArgS64]    = "s64",
    };

    if(arg_type < __Count__CCliArg)
    {
        return c_cli_args_types[arg_type];
    }

    return NULL;
}

static CCliActionReturn c_cli_parse_nex_arg_str(void* const restrict ctx, const char** out)
{
    const char* raw_arg = c_cli_next_arg(ctx);

    if(raw_arg)
    {
        *out = raw_arg;
        return CCliActionOK;
    }

    return CCliActionMissingInput;
}

static CCliActionReturn c_cli_parse_nex_arg_dig(void* const restrict ctx, size_t* const restrict out)
{
    const char* raw_arg = c_cli_next_arg(ctx);

    if(raw_arg)
    {
        *out = strtoull(raw_arg, NULL, 10);
        return CCliActionOK;
    }

    return CCliActionMissingInput;
}

#define CCLI_PARSE_UINT_TEMPLATE(TYPE)                                                            \
static inline CCliActionReturn c_cli_parse_next_arg_##TYPE(                                       \
        void* const restrict ctx, TYPE* const restrict out)                                       \
{                                                                                                 \
    size_t res =0;                                                                                \
    const size_t type_max_val = (const TYPE) (~0);                                                \
    const CCliActionReturn act_ret = c_cli_parse_nex_arg_dig(ctx, &res);                          \
                                                                                                  \
    if(act_ret != CCliActionOK) return act_ret;                                                   \
                                                                                                  \
    if( res > type_max_val) return CCliActionInvalidInput;                                        \
    *out = (const TYPE) res;                                                                      \
    return CCliActionOK;                                                                          \
}                                                                                                 \

CCLI_PARSE_UINT_TEMPLATE(uint8_t)
CCLI_PARSE_UINT_TEMPLATE(uint16_t)
CCLI_PARSE_UINT_TEMPLATE(uint32_t)
CCLI_PARSE_UINT_TEMPLATE(uint64_t)

CCLI_PARSE_UINT_TEMPLATE(int8_t)
CCLI_PARSE_UINT_TEMPLATE(int16_t)
CCLI_PARSE_UINT_TEMPLATE(int32_t)
CCLI_PARSE_UINT_TEMPLATE(int64_t)
