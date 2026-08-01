#pragma once


#define CCLI_END_LINE "\n\r"
#define CCLI_SLAH '/'

#ifndef CCLI_ARG_SEPARATOR
#define CCLI_ARG_SEPARATOR " "
#endif // !CCLI_ARG_SEPARATOR

#define CCLI_1_TAB "\t"
#define CCLI_2_TAB CCLI_1_TAB CCLI_1_TAB
#define CCLI_3_TAB CCLI_1_TAB CCLI_2_TAB
#define CCLI_4_TAB CCLI_1_TAB CCLI_3_TAB

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

typedef struct{
    const char* f_long;
    const char* l_pad;
    const char* f_short;
    const char* s_pad;
    const CCliParser f_parser;
    char* f_args; //series of strings devided by ' '
    char* f_description; //series of strings devided by ' '
}CCliArgDef;

typedef struct CCliParseCtx{
    int* i;
    int argc;
    char **argv;
}CCliParseCtx;
