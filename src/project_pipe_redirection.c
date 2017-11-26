#include "smallsh.h"

int runcommand_pipe(int argc, char **cline, char where)
{
    int pid[2]; // , exitstat, ret;
    int status;
    int pr_code;

    int fd[2];
    char buf[BUF_SIZE];
    int i;

    for(i = 0; i < 2; i++) {

        if (pipe(fd) < 0) {
            perror("smallsh pipe error");
            return -1;
        }

        if ((pid[i] = fork()) < 0) {
            perror("smallsh error");
            return -1;
        }

        if (pid[i] == -1) {
            perror("failed fork");
            exit(1);
        } else if (pid[i] == 0) { /* child */
            if(i == 0) {
                dup2(fd[1], 1);
            } else if(i > 0) {
                dup2(fd[0], 0);
            }
            pr_code = command_parser(pid, argc, cline);
            if(pr_code == -1) {
                execvp(*cline, cline);
                perror(*cline);
                exit(127);
            } else {
                exit(0);
            }
        } else if (pid[i] > 0) {
            dup2(fd[1], 1);
            pr_code = command_parser(pid, argc, cline);
        }

        /* code for parent */
        /* if background process, print pid and exit */
        if (where == BACKGROUND) {
            printf("[Process id %d]\n",pid);
            return 0;
        }

        /* 프로세스 pid가 퇴장할 때까지 기다린다. */
        if (waitpid(pid[i], &status, 0) == -1)
            return -1;
        else
            return status;
    }
    close(fd[0]);
    close(fd[1]);
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