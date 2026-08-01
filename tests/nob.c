#include <stdio.h>
#include <string.h>
#define NOB_IMPLEMENTATION
#include "nob.h"

#define ArraySize(ARR) (sizeof(ARR)/sizeof(ARR[0]))

#define PROJECT_ROOT ".."

#define BUILD_DIR "build"
#define SRC_DIR "src"
#define PROJ_SRC_DIR PROJECT_ROOT"/c_cli"

#define O_FILE "main"

const static char* comp_args[] =
{
  "-Wall",
  "-Wextra",
  "-pedantic",
  "-xc",
  "-std=c99",
  "-ggdb",
  "-pedantic",
  "-I"PROJ_SRC_DIR,
  "-fsanitize=address",
};

const static char* linker_args[] =
{
  "-Wall",
  "-Wextra",
  "-fsanitize=address",
};


bool compile_obj(Nob_Walk_Entry entry)
{
    static char file_name[PATH_MAX] = {0};

    Cmd cmd = {0};
    size_t len = strlen(entry.path);
    char* cursor, *start, *end;

    if(
            entry.type == NOB_FILE_REGULAR &&
            !strncmp(entry.path + len - 2, ".c", 2)
            
      )
    {
        strncpy(file_name, path_name(entry.path), sizeof(file_name));
        file_name[strlen(file_name)-1] = 'o'; //c
        const char* o_file = temp_sprintf("%s/%s", BUILD_DIR, file_name);

        cmd_append(&cmd, "cc");

        for(size_t i=0; i<ArraySize(comp_args); i++)
        {
            cmd_append(&cmd, comp_args[i]);
        }
        cmd_append(&cmd, "-o", o_file);
        cmd_append(&cmd, "-c", entry.path);

        return cmd_run(&cmd);
    }

    return true;
}

static bool link_files(const char* build_dir)
{
    Cmd cmd = {0};
    Dir_Entry dir = {0};
    bool res =false;

    cmd_append(&cmd, "cc");
    for(size_t i=0; i<ArraySize(linker_args); i++)
    {
        cmd_append(&cmd, linker_args[i]);
    }
    cmd_append(&cmd, "-o", O_FILE);

    if(!dir_entry_open(build_dir, &dir))
    {
        printf("failed open build dir\n");
        return 2;
    }

    while(dir_entry_next(&dir))
    {
        int len = strlen(dir.name);
        if(
                strncmp(dir.name, "..", 2) &&
                strncmp(dir.name, ".", 1) &&
                !strncmp(dir.name + len - 2, ".o", 2))
        {
            cmd_append(&cmd, temp_sprintf("%s/%s", BUILD_DIR, dir.name));
        }
    }
    res = cmd_run(&cmd);

    dir_entry_close(dir);

    return res;
}

int main(int argc, char **argv)
{
  GO_REBUILD_URSELF(argc, argv);

  Cmd cmd = {0};
  char src_dir[PATH_MAX] = {0};
  char proj_src_dir[PATH_MAX] = {0};
  char build_dir[PATH_MAX] = {0};

  const char* pwd = get_current_dir_temp();

  snprintf(src_dir, sizeof(src_dir), "%s/"SRC_DIR, pwd);
  snprintf(proj_src_dir, sizeof(proj_src_dir), "%s/"PROJ_SRC_DIR, pwd);
  snprintf(build_dir, sizeof(build_dir), "%s/"BUILD_DIR, pwd);

  nob_mkdir_if_not_exists(BUILD_DIR);

  nob_log(INFO, "compile args:");
  for(size_t i=0; i<ArraySize(comp_args); i++)
  {
    nob_log(INFO, "\t\t%s", comp_args[i]);
  }

  nob_log(INFO, "linker args");
  for(size_t i=0; i<ArraySize(linker_args); i++)
  {
    nob_log(INFO, "\t\t%s", linker_args[i]);
  }

  nob_log(INFO, "output_file: %s", O_FILE);
  nob_log(INFO, "src dir: %s", src_dir);
  nob_log(INFO, "proj src dir: %s", proj_src_dir);
  nob_log(INFO, "build dir: %s", build_dir);

  if(!walk_dir(proj_src_dir, compile_obj))
  {
      printf("error compiling\n");
      return 1;
  }

  if(!walk_dir(src_dir, compile_obj))
  {
    printf("error compiling\n");
    return 1;
  }

  if(!link_files(build_dir))
  {
    printf("link failed\n");
    return 1;
  }

  return 0;
}
