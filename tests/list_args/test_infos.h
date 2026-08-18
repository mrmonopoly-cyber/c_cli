#include "../test_interface.h"

static TeSrcs te_srcs()
{
    static const char* srcs [] = 
    {
        "./src/main.c"
    };

    return (TeSrcs){
        .n_data = ArraySize(srcs),
        .data = srcs,
    };
}

static TeSrcs te_cmp_flags()
{
    static const char* flags [] =
    {
        "-Wall",
        "-Wextra",
        "-pedantic",
        "-xc",
        "-std=c99",
        "-ggdb",
        "-pedantic",
        "-fsanitize=address",
    };

    return (TeSrcs){
        .n_data = ArraySize(flags),
        .data = flags,
    };
}

static TeRunArgs te_run_flags()
{
    static const char* flags [] =
    {
        "-f",
        "12",
        "52,",
        "88",
        "77,",
        "22",
        "33",
    };

    return (TeSrcs){
        .n_data = ArraySize(flags),
        .data = flags,
    };
}
