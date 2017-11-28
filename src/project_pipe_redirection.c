#include "smallsh.h"

// 파이프 처리 루틴이나 미구현
int runcommand_pipe(int argc, char where)
{
    int pid[2]; // pipe's each command pid
    int status;
    int pr_code;

    int fd[2];
    char buf[BUF_SIZE];
    int i, j;

    // 파이프를 생성함
    if (pipe(fd) < 0) {
        perror("smallsh pipe error");
        return -1;
    }
    
    // 각각의 커맨드를 fork함
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
        }

        // 해당 명령어 실행
        execvp(arg_pipe[i][0], arg_pipe[i]);
        perror(arg_pipe[i][0]);
        exit(1);
        // command_parser(pid[i], argc, arg_pipe[i]);
    }

    // 부모일 경우 디스크립터 해제
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

// 리다이렉션 처리 루틴이나 미구현
int runcommand_redirection(int argc, char where, int special_type)
{
    int pid; // , exitstat, ret;
    int status;
    int pr_code;
    int file_id;
    int filename = arg_redirection[1][0];

    if ((pid = fork()) < 0) {
        perror("smallsh");
        return -1;
    }

    // create
    if(pid == 0) {
        if(file_id = creat(filename, 0640)) {
            perror("file create error");
            exit(1);
        }

        // dup2 함수로 파일디스크립터 복사
        dup2(file_id, STDOUT_FILENO);
        close(file_id);
        
        execvp(*arg_redirection, arg_redirection);
        perror(*arg_redirection);
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