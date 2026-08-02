#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct CCliUserArgs{
    bool verbose;
    bool help;
    const char* path;
    struct{
        const char* name;
        uint8_t arg;
    }test;
}CCliUserArgs;

int cli_parse(CCliUserArgs* const restrict args, int argc, char** argv);
void cli_print_args(const CCliUserArgs* const restrict args);
