#pragma once

#include <stdarg.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "c_cli_defs.h"
#include "c_cli_utils.h"

static bool c_cli_parse(
        const CCliArgDef* defs,
        const size_t n_defs,
        struct CCliUserArgs* const restrict args,
        const int argc,
        char** argv,
        CCliDefaultSetter def_set);


//C_CLI INTERNAL DEFS

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

static CCLI_PARSER_DECLARE_FULL(verbose, args, ctx)
{
    (void) ctx;
    args->base.verbose = true;
    return CCliActionOK;
}

static CCLI_PARSER_DECLARE_FULL(help, args, ctx)
{
    (void) ctx;
    args->base.help= true;
    return CCliActionOK;
}

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

    if(args->base.help)
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
