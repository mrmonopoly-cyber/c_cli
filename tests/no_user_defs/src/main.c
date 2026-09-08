#include <stdbool.h>

typedef struct CCliUserArgs{
    bool help;
    bool verbose;
}Args;

static inline bool parse_args(Args* args, const int argc, char** argv);


int main(int argc, char *argv[])
{
    Args args = {0};

    if(!parse_args(&args, argc, argv))
    {
        return -1;
    }

    return 0;
}


#define CCLI_IMPLEMENTATION
#include "c_cli.h"

static inline bool parse_args(Args* args, const int argc, char** argv)
{
    return c_cli_parse(
            NULL,
            0,
            args,
            argc,
            argv,
            NULL);
}
