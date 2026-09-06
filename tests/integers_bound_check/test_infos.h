#include "../test_interface.h"
#include <stdint.h>

static TeSrcs te_srcs()
{
    static const char* srcs [] = 
    {
        "./src/main.c",
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
        "0",                        //u8    MIN
        "255",                      //u8    MAX

        "0",                        //u16   MIN
        "65535",                    //u16   MAX

        "0",                        //u32   MIN
        "4294967295",               //u32   MAX

        "0",                        //u64   MIN
        "18446744073709551615",     //u64   MAX

        "-128",                     //s8    MIN
        "127",                      //s8    MAX

        "-32768",                   //s16    MIN
        "32767",                    //s16    MAX

        "-2147483648",              //s32    MIN
        "2147483647",               //s32    MAX

        "-9223372036854775808",     //s64   MIN
        "9223372036854775807",      //s64   MAX

        "-1",                       //u8    OUT MIN
        "256",                      //u8    OUT MAX

        "-2",                       //u16    OUT MIN
        "65536",                    //u16    OUT MAX

        "-3",                       //u32    OUT MIN
        "4294967296",               //u32    OUT MAX

        "-4",                       //u64    OUT MIN
        "18446744073709551616",     //u64    OUT MAX

        "-129",                     //s8    OUT MIN
        "128",                      //s8    OUT MAX

        "-32769",                   //s16    OUT MIN
        "32768",                    //s16    OUT MAX

        "-2147483649",              //s32    OUT MIN
        "2147483648",               //s32    OUT MAX

        "-9223372036854775809",     //s64    OUT MIN
        "9223372036854775808",      //s64    OUT MAX
    };

    return (TeSrcs){
        .n_data = ArraySize(flags),
        .data = flags,
    };
}
