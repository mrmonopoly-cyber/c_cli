#include <stdarg.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

//public API ============================================================================

//macros
#define CCLI_STRINGIFY(STR) #STR

#ifndef CCLI_MAX_LEN_PROG_NAME
#define CCLI_MAX_LEN_PROG_NAME 64
#endif // !CCLI_MAX_LEN_PROG_NAME

#ifndef CCLI_CHARS_IN_TAB
#define CCLI_CHARS_IN_TAB 8
#endif // !CCLI_CHARS_IN_TAB

#ifndef CCLI_MAX_NUM_ARGS
#define CCLI_MAX_NUM_ARGS 8
#endif // !CCLI_MAX_NUM_ARGS

#ifndef CCLI_SLAH
#define CCLI_SLAH '/'
#endif // !CCLI_SLAH

#define CCLI_END_LINE "\n\r"

#define CCLI_NO_TAB ""
#define CCLI_1_TAB "\t"
#define CCLI_2_TAB CCLI_1_TAB CCLI_1_TAB
#define CCLI_3_TAB CCLI_1_TAB CCLI_2_TAB
#define CCLI_4_TAB CCLI_1_TAB CCLI_3_TAB

//flags names
#define CCLI_LONG_FLAG(NAME) "--"#NAME
#define CCLI_SHORT_FLAG(NAME) "-"#NAME

//flag args
#define CCLI_NEW_ARG(name, type) {#name, type}
#define CCLI_NO_ARG {{NULL, 0}}

//flag parsers

#define CCLI_PARSER_NAME(NAME)CCliParser_##NAME

#define CCLI_PARSER_DECLARE_FULL(NAME, args, ctx)                                               \
    CCliActionReturn CCLI_PARSER_NAME(NAME)(                                                    \
            struct CCliUserArgs* const restrict args,                                           \
            void* const restrict ctx)                                                           \

#define CCLI_PARSER_DECLARE(NAME) CCLI_PARSER_DECLARE_FULL(NAME, args, ctx)

#define CCLI_PARSE_NEXT_ARG_DECLARE(TYPE)                                                         \
static inline CCliActionReturn c_cli_parse_next_arg_##TYPE(                                       \
        void* const restrict ctx, TYPE* const restrict out)                                       \

#define CCLI_PARSE_INTEGER_TEMPLATE(TYPE)                                                            \
CCLI_PARSE_NEXT_ARG_DECLARE(TYPE)                                                                 \
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

//types

#ifndef CCLI_TYPES
#define CCLI_TYPES

typedef enum{
    CCliArgVoid =0  ,

    CCliArgBool     ,

    CCliArgChar     ,
    CCliArgStr      ,

    CCliArgU8       ,
    CCliArgU16      ,
    CCliArgU32      ,
    CCliArgU64      ,

    CCliArgS8       ,
    CCliArgS16      ,
    CCliArgS32      ,
    CCliArgS64      ,

    __Count__CCliArg,
}CCliArgType;

typedef enum
{
    CCliActionOK = 0,
    CCliActionMissingInput,
    CCliActionInvalidInput,
}CCliActionReturn;

struct CCliUserArgs;

typedef CCliActionReturn (*CCliParser)(
        struct CCliUserArgs* const restrict args,
        void* const restrict ctx);

typedef void (*CCliDefaultSetter)(struct CCliUserArgs* const restrict args);

typedef struct{
    const char* name;
    const CCliArgType type;
}CCliArgSpec;

typedef struct{
    const char* f_long;
    const char* f_short;
    const CCliArgSpec f_args[CCLI_MAX_NUM_ARGS];
    char* f_description;
    const CCliParser f_parser;
}CCliArgDef;

typedef struct CCliParseCtx{
    int* i;
    int argc;
    char **argv;
}CCliParseCtx;

#endif // !CCLI_TYPES

//declarations

static bool c_cli_parse(
        const CCliArgDef* defs,
        const size_t n_defs,
        struct CCliUserArgs* const restrict args,
        const int argc,
        char** argv,
        CCliDefaultSetter def_set);

//args
static inline const char* c_cli_arg_type_to_str(const CCliArgType arg_type);
static inline const char* c_cli_str_arg_to_str(const char* const restrict arg);

//pretty printers
static inline const char* c_cli_bool_to_str(const bool val);
static inline const char* c_cli_str_arg_to_str(const char* const restrict arg);

//parsing general utility functions
static inline const char* c_cli_next_arg(void* const restrict ctx);

//parsing specialized utility functions
static CCliActionReturn c_cli_parse_nex_arg_str(void* const restrict ctx, const char** out);
static CCliActionReturn c_cli_parse_nex_arg_dig(void* const restrict ctx, size_t* const restrict out);

CCLI_PARSE_NEXT_ARG_DECLARE(uint8_t);
CCLI_PARSE_NEXT_ARG_DECLARE(uint16_t);
CCLI_PARSE_NEXT_ARG_DECLARE(uint32_t);
CCLI_PARSE_NEXT_ARG_DECLARE(uint64_t);

CCLI_PARSE_NEXT_ARG_DECLARE(int8_t);
CCLI_PARSE_NEXT_ARG_DECLARE(int16_t);
CCLI_PARSE_NEXT_ARG_DECLARE(int32_t);
CCLI_PARSE_NEXT_ARG_DECLARE(int64_t);

//C_CLI INTERNAL DEFS ============================================================================

#ifdef CCLI_DEPLOY

CCLI_DEPLOY
//types
typedef enum {
    CCliCheckInputDefsRet_Found =0,
    CCliCheckInputDefsRet_NotFound,
    CCliCheckInputDefsRet_Error,
}CCliCheckInputDefsRet;

struct __CCliAlignSizes{
    size_t s_to_l;
    size_t l_to_d;
};

typedef struct{
    const CCliArgDef* addr; 
    const size_t size;
}__CCliBaseDefInfo;

//declarations

static void __c_cli_find_correct_align(
        struct __CCliAlignSizes* align,
        const CCliArgDef* const restrict defs,
        const size_t n_defs);

static size_t __c_cli_fprint_all_args(
        FILE* const restrict out,
        const CCliArgSpec* const restrict f_args);

static void __c_cli_print_defs_help(
        const CCliArgDef* const restrict defs,
        const size_t n_defs,
        struct __CCliAlignSizes* aligns,
        FILE* const restrict out
        );

static inline void __c_cli_print_help_full(
        const CCliArgDef* const restrict defs,
        const size_t n_defs,
        const char* const argv_0,
        FILE* const restrict out);

static CCliCheckInputDefsRet __c_cli_check_input_defs(
        const char* const restrict input,
        const CCliArgDef* defs,
        const size_t n_defs,
        struct CCliUserArgs* const restrict args,
        CCliParseCtx* const restrict ctx
        );

static const char* __c_cli_get_prog_name(const char* const restrict argv_0);

static inline __CCliBaseDefInfo __c_cli_get_base_flags(void);

static size_t __c_cli_f_writer(void* f, char* fmt, ...);
static size_t __c_cli_s_writer(void* f, char* fmt, ...);

static size_t __c_cli_fprint_all_args(
        FILE* const restrict out,
        const CCliArgSpec* const restrict f_args);

static inline size_t __c_cli_sprint_all_args(
        char* const restrict out,
        const CCliArgSpec* const restrict f_args);

static void __c_cli_find_correct_align(
        struct __CCliAlignSizes* align,
        const CCliArgDef* const restrict defs, const size_t n_defs);

static void __c_cli_print_defs_help(
        const CCliArgDef* const restrict defs,
        const size_t n_defs,
        struct __CCliAlignSizes* aligns,
        FILE* const restrict out);

static CCliCheckInputDefsRet __c_cli_check_input_defs(
        const char* const restrict input,
        const CCliArgDef* defs,
        const size_t n_defs,
        struct CCliUserArgs* const restrict args,
        CCliParseCtx* const restrict ctx);

static CCLI_PARSER_DECLARE_FULL(verbose, args, ctx);
static CCLI_PARSER_DECLARE_FULL(help, args, ctx);

//implementations



static inline void __c_cli_print_help_full(
        const CCliArgDef* const restrict defs,
        const size_t n_defs,
        const char* const argv_0,
        FILE* const restrict out)
{
    const char* prog_name = __c_cli_get_prog_name(argv_0);
    const __CCliBaseDefInfo base_flags = __c_cli_get_base_flags();
    struct __CCliAlignSizes aligns = {0};

    __c_cli_find_correct_align(&aligns, defs, n_defs);
    __c_cli_find_correct_align(&aligns, base_flags.addr, base_flags.size);

    fprintf(out, "usge %s [opts]:" CCLI_END_LINE, prog_name);

    //user defs
    __c_cli_print_defs_help(defs, n_defs, &aligns, out);

    //base defs
    __c_cli_print_defs_help(base_flags.addr, base_flags.size, &aligns, out);
}

static size_t __c_cli_write_all_args(
        const CCliArgSpec* const restrict f_args,
        void* dst,
        size_t (*writer)(void* dst, char* fmt, ...))
{
    const CCliArgSpec* arg;
    bool empty = true;
    size_t written=0;
    

    for(size_t i=0; i<CCLI_MAX_NUM_ARGS; i++)
    {
        arg = &f_args[i];

        if(arg->name != NULL)
        {
            if(empty)
            {
                written += writer(dst, "[");
                empty = false;
            }

            if(i>0)
            {
                written += writer(dst, ", ");
            }

            written += writer(dst, "%s:%s", arg->name, c_cli_arg_type_to_str(arg->type));
        }
    }

    if(!empty)
    {
        written += writer(dst, "]");
    }

    return written;
}

static size_t __c_cli_f_writer(void* f, char* fmt, ...)
{
    size_t res;
    va_list vars;

    va_start(vars, fmt);
    res = vfprintf(f, fmt, vars);
    va_end(vars);

    return res;
}

static size_t __c_cli_s_writer(void* f, char* fmt, ...)
{
    size_t res;
    va_list vars;

    va_start(vars, fmt);
    res = vsprintf(f, fmt, vars);
    va_end(vars);

    return res;
}

static size_t __c_cli_fprint_all_args(
        FILE* const restrict out,
        const CCliArgSpec* const restrict f_args)
{
    return __c_cli_write_all_args(f_args, out, __c_cli_f_writer);
}

static inline size_t __c_cli_sprint_all_args(
        char* const restrict out,
        const CCliArgSpec* const restrict f_args)
{
    return __c_cli_write_all_args(f_args, out, __c_cli_s_writer);
}

static void __c_cli_find_correct_align(
        struct __CCliAlignSizes* align,
        const CCliArgDef* const restrict defs, const size_t n_defs)
{
    static char temp_buffer[256] = {0};

    const CCliArgSpec *args;
    size_t args_len, f_len, tot_len, n_tabs;

    for(size_t i=0; i<n_defs; i++)
    {
        args = defs[i].f_args;

        args_len = __c_cli_write_all_args(args, temp_buffer, __c_cli_s_writer);

        f_len = strlen(defs[i].f_short);
        tot_len = args_len + f_len;
        n_tabs = (tot_len / CCLI_CHARS_IN_TAB) + ((tot_len % CCLI_CHARS_IN_TAB) > 0);
        if(n_tabs > align->l_to_d) align->s_to_l = n_tabs;

        f_len = strlen(defs[i].f_long);
        tot_len = args_len + f_len;
        n_tabs = (tot_len / CCLI_CHARS_IN_TAB) + ((tot_len % CCLI_CHARS_IN_TAB) > 0);
        if(n_tabs > align->l_to_d) align->l_to_d = n_tabs;
    }

}

static void __c_cli_print_defs_help(
        const CCliArgDef* const restrict defs,
        const size_t n_defs,
        struct __CCliAlignSizes* aligns,
        FILE* const restrict out)
{
    const CCliArgDef* def;

    for(size_t i=0; i<n_defs; i++)
    {
        size_t written=0, to_write;
        to_write = CCLI_CHARS_IN_TAB * aligns->s_to_l;
        def = &defs[i];

        fprintf(out, CCLI_2_TAB);

        written += fprintf(out, "%s ", def->f_long);                        // --help
        written += __c_cli_fprint_all_args(out, def->f_args);               // [...]

        while(written < to_write)
        {
            written += fprintf(out, " ");                                   // long padding
        }

        written =0;
        to_write = CCLI_CHARS_IN_TAB * aligns->l_to_d;
        fprintf(out, CCLI_1_TAB);
        written += fprintf(out, "%s ", def->f_short);                       // -h
        written += __c_cli_fprint_all_args(out, def->f_args);               // [...]
        while(written < to_write)
        {
            written += fprintf(out, " ");                                   // short padding
        }

        fprintf(out, CCLI_1_TAB"%s", def->f_description);                   // "description"
        fprintf(out, CCLI_END_LINE);
    }
}

static CCliCheckInputDefsRet __c_cli_check_input_defs(
        const char* const restrict input,
        const CCliArgDef* defs,
        const size_t n_defs,
        struct CCliUserArgs* const restrict args,
        CCliParseCtx* const restrict ctx
        )
{
    const CCliArgDef* user_def;
    CCliActionReturn act_res = CCliActionOK;

    for(size_t j=0; j<n_defs; j++)
    {
        user_def = &defs[j];

        if(!strcmp(user_def->f_long, input) || !strcmp(user_def->f_short, input))
        {
            act_res = user_def->f_parser(args, ctx);

            if(act_res == CCliActionOK)
            {
                return CCliCheckInputDefsRet_Found;
            }
            switch (act_res)
            {
                case CCliActionOK:
                    assert(0 && "unreachable");
                    break;
                case CCliActionMissingInput:
                    {
                        fprintf(stderr, "missing args for flag %s OR %s, expected: ",
                                user_def->f_long, user_def->f_short);
                    }
                    break;
                case CCliActionInvalidInput:
                    {
                        fprintf(stderr, "invalid arg %s for flag %s OR %s, expected: ",
                                ctx->argv[*ctx->i], user_def->f_long, user_def->f_short);
                    }
                    break;
            }

            __c_cli_fprint_all_args(stderr, user_def->f_args);
            fprintf(stderr, "\n");
            return CCliCheckInputDefsRet_Error;
        }
    }

    return CCliCheckInputDefsRet_NotFound;
}

static const char* __c_cli_get_prog_name(const char* const restrict argv_0)
{
    static char prog_name[CCLI_MAX_LEN_PROG_NAME] = {0};

    if(argv_0 && !*prog_name)
    {
        const size_t argv_0_len = strlen(argv_0);
        const char* p_prog_name = &argv_0[argv_0_len-1];

        while(p_prog_name > argv_0 && *p_prog_name != CCLI_SLAH)
        {
            p_prog_name--;
        }

        if(*p_prog_name == CCLI_SLAH) p_prog_name++;

        strncpy(prog_name, p_prog_name, sizeof(prog_name));
    }

    return prog_name;
}

#ifdef CCLI_DEPLOY
static CCLI_PARSER_DECLARE_FULL(verbose, args, ctx)
{
    (void) ctx;
    args->verbose = true;
    return CCliActionOK;
}

static CCLI_PARSER_DECLARE_FULL(help, args, ctx)
{
    (void) ctx;
    args->help= true;
    return CCliActionOK;
}
#endif //!CCLI_DEPLOY

static inline CCLI_PARSER_DECLARE_FULL(__ignore_flag, args, ctx)
{
    (void) args;
    (void) ctx;
    return CCliActionOK;
}

CCLI_PARSE_INTEGER_TEMPLATE(uint8_t)
CCLI_PARSE_INTEGER_TEMPLATE(uint16_t)
CCLI_PARSE_INTEGER_TEMPLATE(uint32_t)
CCLI_PARSE_INTEGER_TEMPLATE(uint64_t)

CCLI_PARSE_INTEGER_TEMPLATE(int8_t)
CCLI_PARSE_INTEGER_TEMPLATE(int16_t)
CCLI_PARSE_INTEGER_TEMPLATE(int32_t)
CCLI_PARSE_INTEGER_TEMPLATE(int64_t)

static inline __CCliBaseDefInfo __c_cli_get_base_flags(void)
{
    static const CCliArgDef base_flags[] =
    {
        { //--verbose, -v
            .f_long = CCLI_LONG_FLAG(verbose),
            .f_short = CCLI_SHORT_FLAG(v),
            .f_args = CCLI_NO_ARG,
            .f_description = "print verbose output",
            .f_parser = CCLI_PARSER_NAME(verbose),
        },

        {//--help, -h
            .f_long = CCLI_LONG_FLAG(help),
            .f_short = CCLI_SHORT_FLAG(h),
            .f_args = CCLI_NO_ARG,
            .f_description = "print this help",
            .f_parser = CCLI_PARSER_NAME(help),
        },
    };

    return (__CCliBaseDefInfo) {
        .addr = base_flags,
        .size = sizeof(base_flags)/sizeof(base_flags[0]),
    };
}

#ifdef CCLI_DEPLOY
static bool c_cli_parse(
        const CCliArgDef* defs,
        const size_t n_defs,
        struct CCliUserArgs* const restrict args,
        const int argc,
        char** argv,
        CCliDefaultSetter def_set)
{
    const char* input;
    const __CCliBaseDefInfo base_flags = __c_cli_get_base_flags();
    const char* prog_name = __c_cli_get_prog_name(argv[0]);
    CCliParseCtx ctx = {
        .i=NULL,
        .argc = argc,
        .argv = argv,
    };

    for(int i=1;i <argc; i++)
    {
        ctx.i = &i;
        input = argv[i];

        switch (__c_cli_check_input_defs(input, defs, n_defs, args, &ctx))
        {
            case CCliCheckInputDefsRet_Found: continue;
            case CCliCheckInputDefsRet_NotFound: break;
            case CCliCheckInputDefsRet_Error: return false;
        }

        switch(__c_cli_check_input_defs(input, base_flags.addr, base_flags.size, args, &ctx))
        {
            case CCliCheckInputDefsRet_Found: continue;
            case CCliCheckInputDefsRet_NotFound: break;
            case CCliCheckInputDefsRet_Error: return false;
        }

        fprintf(stderr, "%s: unrecognized flag: %s" CCLI_END_LINE, prog_name, input);
    }

    if(args->help)
    {
        __c_cli_print_help_full(defs, n_defs, argv[0], stderr);
        return false;
    }

    for(size_t i=0; i<sizeof(*args); i++)
    {
        if( ((const char*)args)[i] )
        {
            return true;
        }
    }

    if(def_set)
    {
        printf("%s: no arguments provide. Applying default values\n", prog_name);
        def_set(args);
        return true;
    }

    __c_cli_print_help_full(defs, n_defs, argv[0], stderr);

    return false;
}
#endif //!CCLI_DEPLOY

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

//pretty printers

static inline const char* c_cli_bool_to_str(const bool val)
{
    return val ? "True" : "False";
}

static inline const char* c_cli_str_arg_to_str(const char* const restrict arg)
{
    return arg ? arg : "(Nill)";
}


//parsing general utility functions

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

//parsing specialized utility functions

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
#endif // CCLI_DEPLOY
