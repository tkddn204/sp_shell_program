#include "smallsh.h"

#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/procfs.h>

#define FLAG_NOTHING 0

#define DEFAULT_PRINT_TITLE "%5s %-12s %6s %s\n"
#define DEFAULT_PRINT_FORMAT "%5d %-12s %3d:%02d %s\n"

#define CAN_NOT_OPEN_PROC_FOLDER "can not open process folder.\n"
#define CAN_NOT_OPEN_FILE_IN_PROC "can not open file in process folder.\n"
#define CAN_NOT_OPEN_PS_INFO "can not access process info.\n"
#define CAN_NOT_OPEN_PS_STATUS "can not access process status.\n"

void print_title(int flag) {
    switch(flag) {
        case FLAG_NOTHING:
            printf(DEFAULT_PRINT_TITLE,
                "PID", "TTY", "TIME", "CMD");
            break;
    }
}

void print_process(int fd, prpsinfo_t *ps_info, int flag) {
    time_t now;
    unsigned long nowtime;
    int tty;

    time(&now);

    nowtime = now - ps_info->pr_start.tv_sec;
    tty = isatty(fd);
    switch(flag) {
        case FLAG_NOTHING:
            printf(DEFAULT_PRINT_FORMAT,
                ps_info->pr_pid, ttyname(tty),
                (int)nowtime/60, nowtime%60,
                ps_info->pr_psargs);
            break;
    }
}

int read_process_file(char* pid, int flag) {
    prpsinfo_t ps_info;
    int fd;
    char path[14] = {0,};
    
    sprintf(path, "/proc/%s", pid);

    if (-1 == access(path, R_OK)) {
        return -1;
    }

    if(-1 == (fd = open(path, O_RDONLY))) {
        perror(CAN_NOT_OPEN_FILE_IN_PROC);
        exit(1);
    }

    if(-1 == ioctl(fd, PIOCPSINFO, &ps_info)) {
        perror(CAN_NOT_OPEN_PS_INFO);
        exit(1);
    }
    
    print_process(fd, &ps_info, flag);
    close(fd);
    return 0;
}

int operation(int flag) {
    DIR *dir;
    struct dirent *dir_entry;

    if(NULL == (dir = opendir("/proc"))) {
        perror(CAN_NOT_OPEN_PROC_FOLDER);
        exit(1);
    }

    print_title(flag);
    while(NULL != (dir_entry = readdir(dir))) {
        if(dir_entry->d_name[0] == '.') continue;
        read_process_file(dir_entry->d_name, flag);
    }

    closedir(dir);
    return 0;
}

int project_ps(int argc, char **argv) {
    extern char *optarg;
    int parameter;
    printf("ps_info - 20131722 Han Sang Woo \n");

    if(argc < 2) {
        operation(FLAG_NOTHING);
    }
    while( -1 != (parameter = getopt(argc, argv, "a")))
        switch(optarg) {
            case 'a':
            case '?':
            default:
                break;
        }

    return 0;
}

// for test
// int main(int argc, char **argv) {
//     return project_ps(argc, argv);
// }
