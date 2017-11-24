#include "smallsh.h"

#define HISTORY_FILE_NAME "/.history"

#define CAN_NOT_OPEN_HISTORY_FILE "can not open history file"

// int save_history(char* inp) {
//     int fd, len;

//     if(-1 == (fd = open(HISTORY_FILE_NAME,
//                 O_RDWR | O_CREAT | O_APPEND, 0644))) {
//         perror(CAN_NOT_OPEN_HISTORY_FILE);
//         exit(1);
//     }

//     len = strlen(inp);

//     write(fd, inp, len);
//     close(fd);
//     return 1;
// }

int project_history(int argc, char **argv) {
    printf("history - 20131705 Bae Min Su \n");
    
    return 0;
}

// 테스트용 메인함수
// int main(int argc, char **argv) {
//     return project_history(argc, argv);
// }