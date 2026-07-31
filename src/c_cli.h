#pragma once

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "c_cli_helper.h"

#ifndef CCLI_PREFIX
#define CCLI_PREFIX static
#endif // !CCLI_PREFIX

/*
 * Before including this library the user has to define in the compilation unit the
 * following element:
 *
 * - struct CCliUserArgs: the struct containing all the cli's options
 * - a list of user defined functions to manage each flag which have to respect
 *   the following convention:.
 *           void (*) (
 *              CCliUserArgs* const restrict args,
 *              size_t* const restrict i,
 *              const int argc 
 *              const char ** argv
 *              )
 *   (macros for easy cases are already defined in c_cli_helper.h)
 *
 * - CCLI_ARGS_LIST : An X macro containing all the cli's arguments using the following
 *   convention:
 *           CCLI_X(LONG, L_PAD, SHORT, ARGS, S_PAD, ACTION, DESCR)
 *
 *           ALL the element of the X_MACRO are static strings EXCEPT ACTION
 *           which is a function pointer to a function, defined by the user,
 *           with the previously defined signature.
 *
 *
 *
 */


//INTERNAL DEFINITIONS =================================================================

#define CCLI_END_LINE "\n\r"
#define CCLI_SLASH '/'

CCLI_PREFIX void c_cli_print_help_to(const char* const prog_name, FILE* file)
{
    const size_t len = strlen(prog_name);
    const char* cursor = &prog_name[len-1];

    while( cursor > prog_name && *cursor != CCLI_SLASH )
    {
        cursor--;
    }

    if(*cursor == CCLI_SLASH) cursor++;

#define CCLI_X(LONG, L_PAD, SHORT, ARGS, S_PAD, ACTION, DESCR) \
   CCLI_2_PAD LONG" "ARGS CCLI_1_PAD L_PAD SHORT" "ARGS CCLI_2_PAD S_PAD DESCR CCLI_END_LINE

    fprintf(file, "usage %s [opts]" CCLI_END_LINE
        CCLI_ARGS_LIST
            , cursor);

#undef CCLI_X
}

CCLI_PREFIX bool ccli_action_out_check(
        const char* const restrict cli_name,
        const char* const restrict f_long,
        const char* const restrict f_short,
        const char* const restrict f_args,
        const CCliActionReturn act_ret)
{
    switch (act_ret)
    {
        case CCliActionOK: return true;
        case CCliActionMissingInput:
            fprintf(stderr, "cli: missing input for %s %s OR %s %s" CCLI_END_LINE,
                    f_long, f_args, f_short, f_args);
            break;
        case CCliActionInvalidInput:
            fprintf(stderr, "cli: invalid input for %s %s OR %s %s" CCLI_END_LINE,
                    f_long, f_args, f_short, f_args);
            break;
    }

    c_cli_print_help(cli_name);
    return false;
}

CCLI_PREFIX int c_cli_parse(CCliUserArgs* const restrict args, const int argc, char** argv)
{
    const char* input = NULL;

#define CCLI_X(LONG, L_PAD, SHORT, ARGS, S_PAD, ACTION, DESCR)          \
    else if(!strcmp(input, LONG) || !strcmp(input, SHORT))              \
    {                                                                   \
        const CCliActionReturn ret = ACTION( args, &i, argc, argv );    \
        if(!ccli_action_out_check(argv[0], LONG, SHORT, ""ARGS, ret))   \
        {                                                               \
            return -2;                                                  \
        }                                                               \
    }                                                                   \

    for(int i=1; i< argc; i++)
    {
        input = argv[i];

        if(0)
        {
            //just to introduce else if
        }
        CCLI_ARGS_LIST
    }
#undef CCLI_X

    for(size_t i=0; i<sizeof(CCliUserArgs); i++)
    {
        if( ((const char*)args)[i] )
        {
            return 0;
        }
    }

    c_cli_print_help(argv[0]);
    return -1;
}

#undef CCLI_END_LINE
#undef CCLI_SLASH
