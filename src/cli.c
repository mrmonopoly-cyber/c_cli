#include "cli.h"
#include "c_cli.h"

#define CLI_PREFIX static inline

CLI_PREFIX CCLI_PARSER_DECLARE(path);
CLI_PREFIX CCLI_PARSER_DECLARE(verbose);
CLI_PREFIX CCLI_PARSER_DECLARE(test);

static const CCliArgDef cli_flags[] =
{
    { //--verbose, -v
        .f_long = "--verbose",
        .l_pad = CCLI_1_TAB,
        .f_short = "-v",
        .s_pad = CCLI_1_TAB,
        .f_parser = CCLI_PARSER_NAME(verbose),
        .f_args = "",
        .f_description = "print verbose output",
    },

    {//--help, -h
        .f_long = "--help",
        .l_pad = CCLI_2_TAB,
        .f_short = "-h",
        .s_pad = CCLI_1_TAB,
        .f_parser = CCLI_PARSER_NAME(__ignore_flag),
        .f_args = "",
        .f_description = "print this help",
    },

    {//--file [path], -f [path]
        .f_long = "--path",
        .l_pad = CCLI_1_TAB,
        .f_short = "-p",
        .s_pad = "",
        .f_parser = CCLI_PARSER_NAME(path),
        .f_args = "path",
        .f_description = "use file from path",
    },

    {//--test [name, arg], -t [name, arg]
        .f_long = "--test",
        .l_pad = "",
        .f_short = "-t",
        .s_pad = "",
        .f_parser = CCLI_PARSER_NAME(test),
        .f_args = "name" CCLI_ARG_SEPARATOR "arg",
        .f_description = "run the test [name] with arg [arg]",
    },
};

int cli_parse(CCliUserArgs* const restrict args, int argc, char** argv)
{
    return c_cli_parse(cli_flags, sizeof(cli_flags)/sizeof(cli_flags[0]), args, argc, argv);
}

void cli_print_args(const CCliUserArgs* const restrict args)
{
    printf("verbose: %s\n", c_cli_bool_to_str(args->verbose));
    printf("help : %s\n", c_cli_bool_to_str(args->help));
    printf("path: %s\n", c_cli_str_arg_to_str(args->path));
    printf("test: [name:%s, arg:%s]\n",
            c_cli_str_arg_to_str(args->test.name),
            c_cli_str_arg_to_str(args->test.arg));
}

CLI_PREFIX CCLI_PARSER_DECLARE_FULL(verbose, args, ctx)
{
    (void) ctx;
    args->verbose = true;
    return CCliActionOK;
}

CLI_PREFIX CCLI_PARSER_DECLARE_FULL(path, args, ctx)
{
    args->path = c_cli_next_arg(ctx);

    if(args->path)
    {
        return CCliActionOK;
    }
    return CCliActionMissingInput;
}

CLI_PREFIX CCLI_PARSER_DECLARE_FULL(test, args, ctx)
{
    if(
            (args->test.name = c_cli_next_arg(ctx)) &&
            (args->test.arg  = c_cli_next_arg(ctx))
      )
    {
        return CCliActionOK;
    }
    return CCliActionMissingInput;
}
