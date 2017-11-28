#include "smallsh.h"

int runcommand_pipe(int argc, char where)
{
    int pid[2]; // pipe's each command pid
    int status;
    int pr_code;

    int fd[2];
    char buf[BUF_SIZE];
    int i, j;

    // make pipe
    if (pipe(fd) < 0) {
        perror("smallsh pipe error");
        return -1;
    }
    
    // fork commands
    for(i = 0; i < 2; i++) {
        if ((pid[i] = fork()) < 0) {
            perror("smallsh error");
            return -1;
        }

        if(pid[i] == 0) {
            if(i == 0) {
                dup2(fd[0], 0);
                close(fd[0]);
                close(fd[1]);
            } else if(i > 0) {
                dup2(fd[1], 1);
                close(fd[0]);
                close(fd[1]);
            }
            // for(j = 0; j < 2; j++) {
            //     close(fd[j]);
            // }
        }
        command_parser(pid[i], argc, arg_pipe[i]);
    }

    for(j = 0; j < 2; j++) {
        close(fd[j]);
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

int runcommand_redirection(int argc, char where, int special_type)
{
    char *temp1[MAXARG], *temp2[MAXARG];
    int pid; // , exitstat, ret;
    int status;
    int pr_code;
    int file_id;

	for(j=0; j<=i-2; j++)
        temp1[j] = arg_redirection[j];

    temp1[j+1] = NULL;
    temp2[0] = arg_redirection[i];
    temp2[1] = NULL;

    if ((pid = fork()) < 0) {
        perror("smallsh");
        return -1;
    }
    if ((pid = fork()) < 0) {
        perror("smallsh error");
        return -1;
    }

    if(pid == 0) {
        
        if(file_id = creat(arg_redirection[2], 0640)) {
            perror("file create error");
            exit(1);
        }

        dup2(file_id, STDOUT_FILENO);
        close(file_id);
        execvp(*temp1, temp1);
        perror(*temp1);
        exit(1);
    }

    // command_parser(pid, argc, cline);
    
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