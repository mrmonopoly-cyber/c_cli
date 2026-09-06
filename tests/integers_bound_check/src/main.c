#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct CCliUserArgs{
    bool help;
    bool verbose;
}Args;

#define CCLI_DEPLOY
#include "c_cli.h"

#define TEMPLATE_DIG_TEST(T) struct {T min; T max; T out_min; T out_max;}

typedef TEMPLATE_DIG_TEST(uint8_t) DigTestU8;
typedef TEMPLATE_DIG_TEST(uint16_t) DigTestU16;
typedef TEMPLATE_DIG_TEST(uint32_t) DigTestU32;
typedef TEMPLATE_DIG_TEST(uint64_t) DigTestU64;

typedef TEMPLATE_DIG_TEST(int8_t) DigTestS8;
typedef TEMPLATE_DIG_TEST(int16_t) DigTestS16;
typedef TEMPLATE_DIG_TEST(int32_t) DigTestS32;
typedef TEMPLATE_DIG_TEST(int64_t) DigTestS64;

DigTestU8 u8;
DigTestU16 u16;
DigTestU32 u32;
DigTestU64 u64;

DigTestS8 s8;
DigTestS16 s16;
DigTestS32 s32;
DigTestS64 s64;

static inline void _print_udig(
        const uint64_t min, const uint64_t max, const uint64_t out_min, const uint64_t out_max)
{
    printf("min: %lu, max: %lu, out_min: %lu, out_max: %lu\n", min, max, out_min, out_max);
}

static inline void _print_dig(
        const int64_t min, const int64_t max, const int64_t out_min, const int64_t out_max)
{
    printf("min: %ld, max: %ld, out_min: %ld, out_max: %ld\n", min, max, out_min, out_max);
}

#define print_udig(DIG) \
    do{_print_udig((DIG).min, (DIG).max, (DIG).out_min, (DIG).out_max);}while(0);

#define print_dig(DIG) \
    do{_print_dig((DIG).min, (DIG).max, (DIG).out_min, (DIG).out_max);}while(0);

static inline void print_vars(void)
{
    print_udig(u8);
    print_udig(u16);
    print_udig(u32);
    print_udig(u64);

    print_dig(s8);
    print_dig(s16);
    print_dig(s32);
    print_dig(s64);
}


int main(int argc, char *argv[])
{
    Args args = {0};
    int i =0;
    CCliParseCtx ctx =
    {
        .attributes = 0,
        .list_continue = 0,
        .i = &i,
        .argc = argc,
        .argv = argv,
    };

    (void) args;


    printf("parsing: %s\n", argv[i+1]);
    if(c_cli_parse_next_arg_uint8_t(&ctx, &u8.min)) goto fail;                  //u8 min
    printf("parsing: %s\n", argv[i+1]);
    if(c_cli_parse_next_arg_uint8_t(&ctx, &u8.max)) goto fail;                  //u8 max

    printf("parsing: %s\n", argv[i+1]);
    if(c_cli_parse_next_arg_uint16_t(&ctx, &u16.min)) goto fail;                //u16 min
    printf("parsing: %s\n", argv[i+1]);
    if(c_cli_parse_next_arg_uint16_t(&ctx, &u16.max)) goto fail;                //u16 max
 
    printf("parsing: %s\n", argv[i+1]);
    if(c_cli_parse_next_arg_uint32_t(&ctx, &u32.min)) goto fail;                //u32 min
    printf("parsing: %s\n", argv[i+1]);
    if(c_cli_parse_next_arg_uint32_t(&ctx, &u32.max)) goto fail;                //u32 max
 
    printf("parsing: %s\n", argv[i+1]);
    if(c_cli_parse_next_arg_uint64_t(&ctx, &u64.min)) goto fail;                //u64 min
    printf("parsing: %s\n", argv[i+1]);
    if(c_cli_parse_next_arg_uint64_t(&ctx, &u64.max)) goto fail;                //u64 max

    printf("parsing: %s\n", argv[i+1]);
    if(c_cli_parse_next_arg_int8_t(&ctx, &s8.min)) goto fail;                   //s8 min
    printf("parsing: %s\n", argv[i+1]);
    if(c_cli_parse_next_arg_int8_t(&ctx, &s8.max)) goto fail;                   //s8 max

    printf("parsing: %s\n", argv[i+1]);
    if(c_cli_parse_next_arg_int16_t(&ctx, &s16.min)) goto fail;                 //s16 min
    printf("parsing: %s\n", argv[i+1]);
    if(c_cli_parse_next_arg_int16_t(&ctx, &s16.max)) goto fail;                 //s16 max

    printf("parsing: %s\n", argv[i+1]);
    if(c_cli_parse_next_arg_int32_t(&ctx, &s32.min)) goto fail;                 //s32 min
    printf("parsing: %s\n", argv[i+1]);
    if(c_cli_parse_next_arg_int32_t(&ctx, &s32.max)) goto fail;                 //s32 max

    printf("parsing: %s\n", argv[i+1]);
    if(c_cli_parse_next_arg_int64_t(&ctx, &s64.min)) goto fail;                 //s64 min
    printf("parsing: %s\n", argv[i+1]);
    if(c_cli_parse_next_arg_int64_t(&ctx, &s64.max)) goto fail;                 //s64 max

    //out of bounds

    printf("parsing: %s\n", argv[i+1]);
    if(!c_cli_parse_next_arg_uint8_t(&ctx, &u8.out_min)) goto fail;             //s8 out min
    printf("parsing: %s\n", argv[i+1]);
    if(!c_cli_parse_next_arg_uint8_t(&ctx, &u8.out_max)) goto fail;             //s8 out max

    printf("parsing: %s\n", argv[i+1]);
    if(!c_cli_parse_next_arg_uint16_t(&ctx, &u16.out_min)) goto fail;           //s16 out min
    printf("parsing: %s\n", argv[i+1]);
    if(!c_cli_parse_next_arg_uint16_t(&ctx, &u16.out_max)) goto fail;           //s16 out max

    printf("parsing: %s\n", argv[i+1]);
    if(!c_cli_parse_next_arg_uint32_t(&ctx, &u32.out_min)) goto fail;           //s32 out min
    printf("parsing: %s\n", argv[i+1]);
    if(!c_cli_parse_next_arg_uint32_t(&ctx, &u32.out_max)) goto fail;           //s32 out max


    printf("parsing: %s\n", argv[i+1]);
    if(!c_cli_parse_next_arg_uint64_t(&ctx, &u64.out_min)) goto fail;           //s64 out min
    printf("parsing: %s\n", argv[i+1]);
    if(!c_cli_parse_next_arg_uint64_t(&ctx, &u64.out_max)) goto fail;           //s64 out max

    printf("parsing: %s\n", argv[i+1]);
    if(!c_cli_parse_next_arg_int8_t(&ctx, &s8.out_min)) goto fail;              //s8 out min
    printf("parsing: %s\n", argv[i+1]);
    if(!c_cli_parse_next_arg_int8_t(&ctx, &s8.out_max)) goto fail;              //s8 out max

    printf("parsing: %s\n", argv[i+1]);
    if(!c_cli_parse_next_arg_int16_t(&ctx, &s16.out_min)) goto fail;            //s16 out min
    printf("parsing: %s\n", argv[i+1]);
    if(!c_cli_parse_next_arg_int16_t(&ctx, &s16.out_max)) goto fail;            //s16 out max

    printf("parsing: %s\n", argv[i+1]);
    if(!c_cli_parse_next_arg_int32_t(&ctx, &s32.out_min)) goto fail;            //s32 out min
    printf("parsing: %s\n", argv[i+1]);
    if(!c_cli_parse_next_arg_int32_t(&ctx, &s32.out_max)) goto fail;            //s32 out max

    printf("parsing: %s\n", argv[i+1]);
    if(!c_cli_parse_next_arg_int64_t(&ctx, &s64.out_min)) goto fail;            //s64 out min
    printf("parsing: %s\n", argv[i+1]);
    if(!c_cli_parse_next_arg_int64_t(&ctx, &s64.out_max)) goto fail;            //s64 out max

    print_vars();

    return 0;

fail:
    print_vars();
    fprintf(stderr, "test failed\n");
    return 1;
}

