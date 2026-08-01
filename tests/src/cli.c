#include "cli.h"
#include "c_cli.h"
#include "c_cli_utils.h"

#define CLI_PREFIX static inline

CLI_PREFIX CCLI_PARSER_DECLARE(path);
CLI_PREFIX CCLI_PARSER_DECLARE(verbose);
CLI_PREFIX CCLI_PARSER_DECLARE(test);

static const CCliArgDef cli_flags[] =
{
    { //--verbose, -v
        .f_long = "--verbose",
        .f_short = "-v",
        .f_args = CCLI_NO_ARG,
        .f_description = "print verbose output",
        .f_parser = CCLI_PARSER_NAME(verbose),
    },

    {//--help, -h
        .f_long = "--help",
        .f_short = "-h",
        .f_args = CCLI_NO_ARG,
        .f_description = "print this help",
        .f_parser = CCLI_PARSER_NAME(__ignore_flag),
    },

    {//--file [path], -f [path]
        .f_long = "--path",
        .f_short = "-p",
        .f_args =
        {
            CCLI_NEW_ARG("path", CCliArgStr),
        },
        .f_description = "use file from path",
        .f_parser = CCLI_PARSER_NAME(path),
    },

    {//--test [name, arg], -t [name, arg]
        .f_long = "--test",
        .f_short = "-t",
        .f_args =
        {
            CCLI_NEW_ARG("name", CCliArgStr),
            CCLI_NEW_ARG("arg", CCliArgU8)
        },
        .f_description = "run the test [name] with arg [arg]",
        .f_parser = CCLI_PARSER_NAME(test),
    },
};

static void default_args(CCliUserArgs* const restrict args)
{
    args->path = "default path";
    args->test.name = "default test";
    args->test.arg = 69;
}

int cli_parse(CCliUserArgs* const restrict args, int argc, char** argv)
{
    return c_cli_parse(
            cli_flags,
            sizeof(cli_flags)/sizeof(cli_flags[0]),
            args,
            argc,
            argv,
            default_args);
}

void cli_print_args(const CCliUserArgs* const restrict args)
{
    printf("verbose: %s\n", c_cli_bool_to_str(args->verbose));
    printf("help : %s\n", c_cli_bool_to_str(args->help));
    printf("path: %s\n", c_cli_str_arg_to_str(args->path));
    printf("test: [name:%s, arg:%u]\n", c_cli_str_arg_to_str(args->test.name), args->test.arg);
}

CLI_PREFIX CCLI_PARSER_DECLARE_FULL(verbose, args, ctx)
{
    (void) ctx;
    args->verbose = true;
    return CCliActionOK;
}

CLI_PREFIX CCLI_PARSER_DECLARE_FULL(path, args, ctx)
{
    return c_cli_parse_nex_arg_str(ctx, &args->path);
}

CLI_PREFIX CCLI_PARSER_DECLARE_FULL(test, args, ctx)
{
    CCliActionReturn res;

    if(
            (res = c_cli_parse_nex_arg_str(ctx, &args->test.name)) != CCliActionOK ||
            (res = c_cli_parse_next_arg_uint8_t(ctx, &args->test.arg)) != CCliActionOK
      )
    {
        return res;
    }

    return res;
}
