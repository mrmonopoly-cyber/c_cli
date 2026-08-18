#include "cli.h"
#include "c_cli.h"

#define CLI_PREFIX static inline

CLI_PREFIX CCLI_PARSER_DECLARE(path);
CLI_PREFIX CCLI_PARSER_DECLARE(test);

static const CCliArgDef cli_flags[] =
{
    {//--file [path], -f [path]
        .f_long = CCLI_LONG_FLAG(path),
        .f_short = CCLI_SHORT_FLAG(p),
        .f_args =
        {
            CCLI_NEW_ARG(path, CCliArgStr),
        },
        .f_description = "use file from path",
        .f_parser = CCLI_PARSER_NAME(path),
    },

    {//--test [name, arg], -t [name, arg]
        .f_long = CCLI_LONG_FLAG(test),
        .f_short = CCLI_SHORT_FLAG(t),
        .f_args =
        {
            CCLI_NEW_ARG(name, CCliArgStr),
            CCLI_NEW_ARG(arg, CCliArgU8)
        },
        .f_description = "run the test [name] with arg [arg]",
        .f_parser = CCLI_PARSER_NAME(test),
    },
};

#define CCLI_DEPLOY
#include "c_cli.h"

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
    printf("help: %s\n", c_cli_bool_to_str(args->help));
    printf("path: %s\n", c_cli_str_arg_to_str(args->path));
    printf("test: [name:%s, arg:%u]\n", c_cli_str_arg_to_str(args->test.name), args->test.arg);
}

CLI_PREFIX CCLI_PARSER_DECLARE_FULL(path, args, ctx)
{
    return c_cli_parse_next_arg_str(ctx, &args->path);
}

CLI_PREFIX CCLI_PARSER_DECLARE_FULL(test, args, ctx)
{
    CCliActionReturn res;

    if(
            (res = c_cli_parse_next_arg_str(ctx, &args->test.name)) != CCliActionOK ||
            (res = c_cli_parse_next_arg_uint8_t(ctx, &args->test.arg)) != CCliActionOK
      )
    {
        return res;
    }

    return res;
}
