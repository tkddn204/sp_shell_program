#include "smallsh.h"

#define CD_PRINT_FORMAT "[move to %s]\n"

int oper_cd(char *dir) {
    if(chdir(dir) == 0) { // chdir 함수로 path 이동
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

    if(argc < 2) // "cd" 만을 타이핑할 경우
        exit_num = oper_cd(home_dir);
    else // argv[1]을 받아서 path로 등록
        exit_num = oper_cd(argv[1]);

    return exit_num;
}
