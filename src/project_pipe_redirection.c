#include "smallsh.h"

int runcommand_pipe(int argc, char **cline, char where)
{
    int pid[2]; // , exitstat, ret;
    int status;
    int pr_code;

    int fd[i][2];
    char buf[BUF_SIZE];
    int i;

    for(i = 0; i < 2; i++) {
        if (pipe(fd[i]) < 0) {
            perror("smallsh pipe error");
            return -1;
        }

        if ((pid[i] = fork()) < 0) {
            perror("smallsh error");
            return -1;
        }

        if(i == 0) {
            dup2(fd[i][1], 1);
        } else if(i > 0) {
            dup2(fd[i][0], 0);
        }

        printf("asdfasf %d\n", pid[i]);
        pr_code = command_parser(pid, argc, cline);
        if (pid[i] == -1) {
            perror("failed fork");
            exit(1);
        } else if (pid[i] == 0) { /* child */
            if(pr_code == -1) {
                execvp(*cline, cline);
                perror(*cline);
                exit(127);
            } else {
                exit(0);
            }
        }
    }
    printf("qwerqwr %d\n", pid[i]);
    for(i = 0; i < 2; i++) {
        close(fd[i][0]);
        close(fd[i][1]);
    }

    /* code for parent */
    /* if background process, print pid and exit */
    if (where == BACKGROUND) {
        if(pid[0] > 0) {
            printf("[Process id %d]\n",pid[0]);
        }
        if(pid[1] > 0) {
            printf("[Process id %d]\n",pid[1]);
        }
        return 0;
    }

    /* 프로세스 pid가 퇴장할 때까지 기다린다. */
    if (waitpid(pid[1], &status, 0) == -1)
        return -1;
    else
        return status;
}

int runcommand_redirection(int argc, char **cline, char where)
{
    int pid; // , exitstat, ret;
    int status;
    int pr_code;

    if ((pid = fork()) < 0) {
        perror("smallsh");
        return -1;
    }
    
    pr_code = command_parser(pid, argc, cline);
    if (pid == 0) { /* child */
        if(pr_code == -1) {
            execvp(*cline, cline);
            perror(*cline);
            exit(127);
        } else {
            exit(0);
        }
    }
    /* code for parent */
    /* if background process, print pid and exit */
    if (where == BACKGROUND) {
        printf("[Process id %d]\n",pid);
        return 0;
    }
    /* 프로세스 pid가 퇴장할 때까지 기다린다. */
    if (waitpid(pid, &status, 0) == -1)
        return -1;
    else
        return status;
}