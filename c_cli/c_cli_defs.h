#pragma once

#include <stdbool.h>

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
typedef struct{
    bool verbose;
    bool help;
}CCliBaseUserArgs;

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
