#pragma once

#ifndef CCLI_MAX_NUM_ARGS
#define CCLI_MAX_NUM_ARGS 8
#endif // !CCLI_MAX_NUM_ARGS

#define CCLI_END_LINE "\n\r"
#define CCLI_SLAH '/'

#ifndef CCLI_ARG_SEPARATOR
#define CCLI_ARG_SEPARATOR " "
#endif // !CCLI_ARG_SEPARATOR

#define CCLI_NO_TAB ""
#define CCLI_1_TAB "\t"
#define CCLI_2_TAB CCLI_1_TAB CCLI_1_TAB
#define CCLI_3_TAB CCLI_1_TAB CCLI_2_TAB
#define CCLI_4_TAB CCLI_1_TAB CCLI_3_TAB

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

typedef struct{
    const char* name;
    const CCliArgType type;
}CCliArgSpec;

typedef struct{
    const char* f_long;
    const char* l_pad;
    const char* f_short;
    const char* s_pad;
    const CCliArgSpec f_args[CCLI_MAX_NUM_ARGS]; //series of strings devided by ' '
    char* f_description; //series of strings devided by ' '
    const CCliParser f_parser;
}CCliArgDef;

typedef struct CCliParseCtx{
    int* i;
    int argc;
    char **argv;
}CCliParseCtx;
