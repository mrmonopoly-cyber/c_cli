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


struct __CliAlignSizes{
    size_t s_to_l;
    size_t l_to_d;
};
static struct __CliAlignSizes __c_cli_find_correct_align(
        const CCliArgDef* const restrict defs, const size_t n_defs);

static size_t __c_cli_fprint_all_args(
        FILE* const restrict out,
        const CCliArgSpec* const restrict f_args);


static inline void c_cli_print_help_full(
        const CCliArgDef* const restrict defs,
        const size_t n_defs,
        const char* const argv_0, FILE* const restrict out)
{
    const size_t n_chars_in_tabs = 8;
    const size_t argv_0_len = strlen(argv_0);
    const char* prog_name = &argv_0[argv_0_len-1];
    const CCliArgDef* def;

    while(prog_name > argv_0 && *prog_name != CCLI_SLAH)
    {
        prog_name--;
    }

    if(*prog_name == CCLI_SLAH) prog_name++;

    struct __CliAlignSizes aligns = __c_cli_find_correct_align(defs, n_defs);

    fprintf(out, "usge %s [opts]:" CCLI_END_LINE, prog_name);

    for(size_t i=0; i<n_defs; i++)
    {
        size_t written=0, to_write;
        to_write = n_chars_in_tabs * aligns.s_to_l;
        def = &defs[i];

        fprintf(out, CCLI_2_TAB);

        written += fprintf(out, "%s ", def->f_long);                        // --help
        written += __c_cli_fprint_all_args(out, def->f_args);               // [...]

        while(written < to_write)
        {
            written += fprintf(out, " ");                                   // long padding
        }

        written =0;
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

static inline void c_cli_print_help(
        const CCliArgDef* const restrict defs,
        const size_t n_defs,
        const char* const argv_0)
{
    c_cli_print_help_full(defs, n_defs, argv_0, stderr);
}

static int c_cli_parse(
        const CCliArgDef* defs,
        const size_t n_defs,
        struct CCliUserArgs* const restrict args,
        const int argc,
        char** argv)
{
    const char* input;
    const CCliArgDef* user_def;
    CCliParseCtx ctx = {
        .i=NULL,
        .argc = argc,
        .argv = argv,
    };

    CCliActionReturn act_res = CCliActionOK;

    for(int i=0;i <argc; i++)
    {
        ctx.i = &i;
        input = argv[i];

        for(size_t j=0; j<n_defs; j++)
        {
            user_def = &defs[j];

            if(!strcmp(user_def->f_long, input) || !strcmp(user_def->f_short, input))
            {
                act_res = user_def->f_parser(args, &ctx);

                if(act_res == CCliActionOK)
                {
                    break;
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
                                    argv[i], user_def->f_long, user_def->f_short);
                        }
                        break;
                }

                __c_cli_fprint_all_args(stderr, user_def->f_args);
                fprintf(stderr, "\n");
                return -1;
            }
        }
    }

    for(size_t i=0; i<sizeof(*args); i++)
    {
        if( ((const char*)args)[i] )
        {
            return 0;
        }
    }

    c_cli_print_help(defs, n_defs, argv[0]);

    return -1;
}

//C_CLI INTERNAL DEFS

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

static struct __CliAlignSizes __c_cli_find_correct_align(
        const CCliArgDef* const restrict defs, const size_t n_defs)
{
    static char temp_buffer[256] = {0};

    const size_t n_chars_in_tabs = 8;

    struct __CliAlignSizes res= {0};
    const CCliArgSpec *args;
    size_t args_len, f_len, tot_len, n_tabs;

    for(size_t i=0; i<n_defs; i++)
    {
        args = defs[i].f_args;

        args_len = __c_cli_write_all_args(args, temp_buffer, __c_cli_s_writer);

        f_len = strlen(defs[i].f_short);
        tot_len = args_len + f_len;
        n_tabs = (tot_len / n_chars_in_tabs) + ((tot_len % n_chars_in_tabs) > 0);
        if(n_tabs > res.l_to_d) res.s_to_l = n_tabs;

        f_len = strlen(defs[i].f_long);
        tot_len = args_len + f_len;
        n_tabs = (tot_len / n_chars_in_tabs) + ((tot_len % n_chars_in_tabs) > 0);
        if(n_tabs > res.l_to_d) res.l_to_d = n_tabs;
    }

    return res;
}
