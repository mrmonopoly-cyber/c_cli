#include <stdio.h>
#include <stdlib.h>

#include "nob.h"
#include "test_interface.h"

#define STR2(S) #S
#define STR(S) STR2(S)

#ifdef TEST_TO_RUN
#include STR(TEST_TO_RUN/test_infos.h)
#endif /* ifdef TEST_TO_RUN */

#define PROJECT_ROOT "../"

#define O_FILE "main"

#define BUILD_DIR "build/"
#define PROJECT_SRC_DIR PROJECT_ROOT "c_cli"


#ifdef TEST_TO_RUN
static bool tester_compile()
{
    Cmd cmd = {0};

    nob_log(INFO, "compiling srcs for test: %s", STR(TEST_TO_RUN));

    nob_mkdir_if_not_exists(BUILD_DIR);

    nob_cc(&cmd);

    const TeCmpFlags cmd_flags = te_cmp_flags();

    for(size_t i=0; i<cmd_flags.n_data; i++)
    {
        nob_cmd_append(&cmd, cmd_flags.data[i]);
    }

    nob_cmd_append(&cmd, "-I"PROJECT_SRC_DIR);

    nob_cc_output(&cmd, BUILD_DIR O_FILE);

    const TeSrcs cmd_src = te_srcs();

    for(size_t i=0; i<cmd_src.n_data; i++)
    {
        nob_cmd_append(&cmd, temp_sprintf("%s/%s", STR(TEST_TO_RUN), cmd_src.data[i]));
    }

    return cmd_run(&cmd);
}

static bool tester_run()
{
    Cmd cmd = {0};
    TeRunArgs run_args = te_run_flags();

    nob_log(INFO, "running test: %s", STR(TEST_TO_RUN));

    cmd_append(&cmd, BUILD_DIR O_FILE);

    for(size_t i=0; i<run_args.n_data; i++)
    {
        cmd_append(&cmd, run_args.data[i]);
    }

    return cmd_run(&cmd);
}

static int test_runner()
{
    if(!tester_compile())
    {
        fprintf(stderr, "failed to compile test: %s\n", STR(TEST_TO_RUN));
        return false;
    }

    if(!tester_run())
    {
        fprintf(stderr, "failed to run test: %s\n", STR(TEST_TO_RUN));
        return false;
    }

    return EXIT_SUCCESS;
}
#endif /* ifdef TEST_TO_RUN */

static int test_selection(int argc, char** argv)
{
    GO_REBUILD_URSELF(argc, argv);
    const char* tester_name = temp_file_name(argv[0]);

    Cmd cmd = {0};

    if(argc < 2)
    {
        fprintf(stderr, "usage %s: <test_name> <test_name> ...\n"
                        "\t\t --all\trun all tests\n",
                temp_file_name(tester_name));
        return EXIT_FAILURE;
    }

    for(int i=1; i<argc; i++)
    {
        const char* test_name = argv[i];
        nob_log(INFO, "preparing test: %s", test_name);

        nob_cc(&cmd);
        nob_cmd_append(&cmd, temp_sprintf("-DTEST_TO_RUN=%s", test_name));
        nob_cc_inputs(&cmd, temp_sprintf("%s.c", tester_name));
        nob_cc_output(&cmd, tester_name);

        if(!cmd_run(&cmd))
        {
            fprintf(stderr, "failed compiling tests runner for %s\n", test_name);
            continue;
        }

        cmd_append(&cmd, temp_sprintf("./%s", tester_name));
        if(!cmd_run(&cmd))
        {
            fprintf(stderr, "failed running tests runner for %s\n", test_name);
            continue;
        }
    }

    nob_cc(&cmd);
    nob_cc_inputs(&cmd, temp_sprintf("%s.c", tester_name));
    nob_cc_output(&cmd, tester_name);

    return cmd_run(&cmd);
}

static bool clean_dir(Walk_Entry entry)
{
    delete_file(entry.path);
    return true;
}

int main(int argc, char *argv[])
{
    walk_dir(BUILD_DIR, clean_dir, .post_order = true);

#ifdef TEST_TO_RUN
    return test_runner();
#else
    return test_selection(argc, argv);
#endif /* ifdef TEST_TO_RUN */
}

#define NOB_IMPLEMENTATION
#include "nob.h"
