#include "cli.h"

int main(int argc, char *argv[])
{
    CCliUserArgs args = {0};

    if(cli_parse(&args, argc, argv) < 0)
    {
        return 69;
    }

    cli_print_args(&args);

    return 0;
}
