#include "smallsh.h"

// 이미 있는 헤더 목록
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <signal.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/wait.h>

#define ALIAS_FILE_NAME "/.alias"

#define DO_NOT_WRITE_ARGS "please write args"
#define CAN_NOT_OPEN_ALIAS_FILE "can not open alias file"

int project_alias(int argc, char **argv) {
    printf("alias - 20131705 Bae Min Su \n");
    // FILE* fp;
    // char* path = getenv("HOME");
    // int i;

    // if(argc < 3) {
    //     perror(DO_NOT_WRITE_ARGS);
    //     exit(3);
    // }

    // if(-1 == (fp = fopen(path,
    //            O_RDWR | O_CREAT | O_APPEND, 0644))) {
    //     perror(CAN_NOT_OPEN_ALIAS_FILE);
    //     exit(1);
    // }

    // len = strlen(argv[1]);

    // write(fd, argv[1], len);
    // close(fd);
    return 1;
}

// int main(int argc, char **argv) {
//     return project_alias(argc, argv);
// }