#include "smallsh.h"

#define CD_PRINT_FORMAT "move to %s!!\n"

int oper_cd(char *dir) {
    if(chdir(dir) == 0) {
        printf(CD_PRINT_FORMAT, dir);
        return 0;
    } else {
        perror("can't moved!!\n");
        return -1;
    }
}
int project_cd(int argc, char **argv) {
    char *home_dir = getenv("HOME");
    int exit_num;
    if(argc < 2) // case "cd"
        exit_num = oper_cd(home_dir);
    else
        exit_num = oper_cd(argv[1]);

    return exit_num;
}
