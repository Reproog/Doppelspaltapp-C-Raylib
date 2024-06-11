#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char **argv) 
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    // COMPILE WITH RAYLIB
    Nob_Cmd cmd = {0};          // SRC FILE
    
    nob_cmd_append(&cmd, "gcc", "src/main.c", "-ggdb");
    nob_cmd_append(&cmd, "-I", "./raylib-src/raylib-5.0_linux_amd64/include/");
    nob_cmd_append(&cmd, "-L", "./raylib-src/raylib-5.0_linux_amd64/lib/", "-L", "./raylib-src/old-raygui-src/", "-l:libraylib.a", "-l:raygui.so", "-lm");

    // Main Exec File
    nob_cmd_append(&cmd, "-o", "./main");

    if (!nob_cmd_run_sync(cmd)) return -1;
     
    cmd.count = 0;
    nob_cmd_append(&cmd, "./main");
    if (!nob_cmd_run_sync(cmd)) return -1;
 
    return 0;
}

