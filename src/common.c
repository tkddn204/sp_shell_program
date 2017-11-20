#include "smallsh.h"

/* 이미 있는 헤더 목록
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
*/

// return 1 -> parent program failed
// return 0 -> child program success;
// return -1 -> not found, call execvp;
int project_program(int pid, int argc, char **cline) {
    // parent program
    if(pid > 0) {
        if(strcmp("cd", *cline) == 0)
            project_cd(argc, cline);
        else
            return 1;
    } else if(pid == 0) {
        if(strcmp("exit", *cline) == 0) {
            kill(getppid(), SIGINT);
            exit(0);
        }
        else if(strcmp("ps", *cline) == 0)
            project_ps(argc, cline);
        else if(strcmp("df", *cline) == 0)
            project_df(argc, cline);
        else if(strcmp("du", *cline) == 0)
            project_du(argc, cline);
        else if(strcmp("printrecode", *cline) == 0
            || strcmp("pr", *cline) == 0)
            project_print_recode(argc, cline);
        else
            return -1;
    }
    return 0;
}
int project_print_recode(int argc, char **argv) {
    printf("printrecode - common command \n");

    return 0;
}

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

// 테스트용 메인함수
// int main(int argc, char **argv) {
//     return project_print_recode(argc, argv);
// }