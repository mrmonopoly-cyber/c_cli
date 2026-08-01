#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "c_cli_defs.h"
#include "c_cli_utils.h"

static inline void __c_cli_print_all_args(FILE* const restrict out, char* const restrict f_args);

static inline void c_cli_print_help_full(
        const CCliArgDef* const restrict defs,
        const size_t n_defs,
        const char* const argv_0, FILE* const restrict out)
{
    const size_t argv_0_len = strlen(argv_0);
    const char* prog_name = &argv_0[argv_0_len-1];
    const CCliArgDef* def;

    while(prog_name > argv_0 && *prog_name != CCLI_SLAH)
    {
        prog_name--;
    }

    if(*prog_name == CCLI_SLAH) prog_name++;


    fprintf(out, "usge %s [opts]:" CCLI_END_LINE, prog_name);

    for(size_t i=0; i<n_defs; i++)
    {
        def = &defs[i];
        fprintf(out, CCLI_2_TAB "%s ", def->f_long);             // --help
        __c_cli_print_all_args(out, def->f_args);               // [...]
        fprintf(out, "%s", def->l_pad);                         // long padding
        fprintf(out, CCLI_1_TAB "%s ", def->f_short);            // -h
        __c_cli_print_all_args(out, def->f_args);               // [...]
        fprintf(out, "%s", def->s_pad);                         // short padding
        fprintf(out, CCLI_1_TAB "%s", def->f_description);      // "description"
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
                        fprintf(stderr, "missing args");
                        break;
                    case CCliActionInvalidInput:
                        fprintf(stderr, "invalid args");
                        break;
                }

                fprintf(stderr, "for flag %s OR %s, expected: ",
                        user_def->f_long, user_def->f_short);

                __c_cli_print_all_args(stderr, user_def->f_args);
                fprintf(stderr, "\n");
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
static inline void __c_cli_print_all_args(FILE* const restrict out, char* const restrict f_args)
{
    static char temp_buffer[256] = {0};
    strncpy(temp_buffer, f_args, sizeof(temp_buffer));

    const char* token = strtok(temp_buffer, CCLI_ARG_SEPARATOR);
    bool first = true;

    if(token)
    {
        fprintf(out, "[");
        while(token)
        {
            if(!first) fprintf(out, ", ");
            fprintf(out, "%s", token);
            first = false;
            token = strtok(NULL, CCLI_ARG_SEPARATOR);

        }
        fprintf(out, "]");
    }

}
