#pragma once

#include <stdbool.h>

typedef struct CCliUserArgs{
    bool verbose;
    bool help;
    const char* path;
    struct{
        const char* name;
        const char* arg;
    }test;
}CCliUserArgs;

int cli_parse(CCliUserArgs* const restrict args, int argc, char** argv);
void cli_print_args(const CCliUserArgs* const restrict args);
