#include "smallsh.h"

#include <dirent.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/procfs.h>

#define FLAG_NOTHING 0x0
#define FLAG_OTHER   0x1
#define FLAG_ALL     0x2
#define FLAG_DETAIL  0x4

#define DEFAULT_PRINT_TITLE "%5s %-12s %6s %s\n"
#define DEFAULT_PRINT_FORMAT "%5d %-12s %3d:%02d %s\n"
#define DETAIL_PRINT_TITLE "%8s %5s %5s %2s %-12s %6s %s\n"
#define DETAIL_PRINT_FORMAT "%8d %5d %5d %2d %-12s %3d:%02d %s\n"

#define CAN_NOT_OPEN_PROC_FOLDER "can not open process folder.\n"
#define CAN_NOT_OPEN_FILE_IN_PROC "can not open file in process folder.\n"
#define CAN_NOT_OPEN_PS_INFO "can not access process info.\n"
#define CAN_NOT_OPEN_PS_STATUS "can not access process status.\n"

static int ps_flag = FLAG_NOTHING;

void help_print() {
    printf("use - ps [ -aAdefh ]\n");
    exit(0);
}

void print_title() {
    if(ps_flag == FLAG_NOTHING) {
        printf(DEFAULT_PRINT_TITLE,
            "PID", "TTY", "TIME", "CMD");
    } else if(ps_flag & FLAG_DETAIL) {
        printf(DETAIL_PRINT_TITLE,
            "UID", "PID", "PPID", "C", "TTY", "TIME", "CMD");
    }
}

void print_process(int fd, prpsinfo_t *ps_info) {
    time_t now;
    unsigned long nowtime;
    int tty;

    time(&now);
    nowtime = now - ps_info->pr_start.tv_sec;
    tty = isatty(fd);

    if(ps_flag == FLAG_NOTHING) {
        printf(DEFAULT_PRINT_FORMAT,
            ps_info->pr_pid, ttyname(tty),
            (int)nowtime/60, nowtime%60,
            ps_info->pr_psargs);
    } else if(ps_flag & FLAG_DETAIL) {
        printf(DETAIL_PRINT_FORMAT,
            ps_info->pr_uid, ps_info->pr_pid,
            ps_info->pr_ppid, ps_info->pr_cpu,
            ttyname(tty), (int)nowtime/60, nowtime%60,
            ps_info->pr_psargs);
    }
}

int read_process_file(char* pid) {
    prpsinfo_t ps_info;
    int fd;
    char path[14] = {0,};
    
    sprintf(path, "/proc/%s", pid);

    if (!(ps_flag & FLAG_OTHER) && !(ps_flag & FLAG_ALL)) {
        if (-1 == access(path, R_OK)) {
            return -1;
        }
    }

    if(-1 == (fd = open(path, O_RDONLY))) {
        perror(CAN_NOT_OPEN_FILE_IN_PROC);
        exit(1);
    }

    if(-1 == ioctl(fd, PIOCPSINFO, &ps_info)) {
        perror(CAN_NOT_OPEN_PS_INFO);
        exit(1);
    }
    
    print_process(fd, &ps_info);
    close(fd);
    return 0;
}

int ps_operation() {
    DIR *dir;
    struct dirent *dir_entry;

    if(NULL == (dir = opendir("/proc"))) {
        perror(CAN_NOT_OPEN_PROC_FOLDER);
        exit(1);
    }

    print_title();
    while(NULL != (dir_entry = readdir(dir))) {
        if(dir_entry->d_name[0] == '.') continue;
        read_process_file(dir_entry->d_name);
    }
    closedir(dir);
    return 0;
}

int project_ps(int argc, char **argv) {
    int parameter;
    printf("ps_info - 20131722 Han Sang Woo \n");

    if(argc < 2) {
        ps_operation();
        return 0;
    }

    while( -1 != (parameter = getopt(argc, argv, "aAefh"))) {
        switch(parameter) {
            case 'a': // show current other pid
                ps_flag |= FLAG_OTHER;
                break;
            case 'A':
            case 'e': // all process
                ps_flag |= FLAG_ALL;
                break;
            case 'f': // detail
                ps_flag |= FLAG_DETAIL;
                break;
            case 'h':
                help_print();
                break;
            case '?':
            default:
                fprintf(stderr, "illegal - option -- %c\n", parameter);
                help_print();
                break;
        }
    }
    ps_operation();
    return 0;
}

// for test
// int main(int argc, char **argv) {
//     return project_ps(argc, argv);
// }
