#include "smallsh.h"

int runcommand_pipe(int argc, char **cline, char where)
{
    int left_fd[2], right_fd[2];
    char buf[BUF_SIZE];
    int pid; // , exitstat, ret;
    int status;
    int pr_code;

    if (pipe(left_fd) < 0 || pipe(right_fd) < 0) {
        perror("smallsh pipe error");
        return -1;
    }

    if ((pid = fork()) < 0) {
        perror("smallsh error");
        return -1;
    }
    
    pr_code = command_parser(pid, argc, cline);
    if (pid == -1) {
        perror("failed fork");
        exit(1);
    } else if (pid == 0) { /* child */
        if(pr_code == -1) {
            execvp(*cline, cline);
            perror(*cline);
            exit(127);
        } else {
            exit(0);
        }
    } else if (pid > 0) {
        while(1) {
            write(stdout, buf, strlen( buf));
            memset( buf, 0, BUF_SIZE);
            read( left_fd[0], buf, BUF_SIZE);
            printf( "부모 프로세스: %s\n", buf);
            if(buf == NULL) {
                break;
            }
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