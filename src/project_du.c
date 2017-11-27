#include "smallsh.h"

#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

void helpPrint();
char *pathAddString(char *, char *);
int project_du(int, char **);
int isDir(char *);
int isRegularFile(char *);
int getFileSize(char *);
int findDirectory(char *);

int aflag, sflag, bflag, kflag;

int project_du(int argc, char *argv[]) {
    int n;
    char *filePath = NULL;
    char buf[1024];
    int optCount = 0;
    extern char *optarg;

    if (argc > 3 || argc < 1) {
        helpPrint();
    }

    while ((n = getopt(argc, argv, "absk")) != -1) {

        switch (n) {
            case 'a':  // fundational infor
                aflag = 1;
                optCount++;
                break;
            case 'b':
                bflag = 1;
                optCount++;
                break;
            case 's':  // total
                sflag = 1;
                optCount++;
                break;
            case 'k':
                kflag = 1;
                optCount++;
                break;
            case '?': // can
            default:
                helpPrint();
        }
    }

    if (aflag == 1 && sflag == 1) {
        printf("do not merge -a and -s\n");
        return 0;
    }
    if (kflag == 1 && bflag == 1) {
        printf("do not merge -b and -k\n");
        return 0;
    }


    if (optCount == 0) {
        if (argv[1] == NULL) {
            getcwd(buf, 1024);
            filePath = buf;
        } else {
            filePath = argv[1];
        }
    } else {
        if (argv[2] == NULL) {
            getcwd(buf, 1024);
            filePath = buf;
        } else {
            filePath = argv[2];
        }
    }

    int total = findDirectory(filePath);

    printf("Total : %d\n", total);

    return total;
}

int findDirectory(char *path) {

    struct dirent *dent;
    DIR *dir;

    char *subPath;
    int size, tmpTotal = 0;
    int defaultCheck = 0;

    dir = opendir(path);
    if (dir == NULL) {
        perror("Failed to open directory");
        return -1; // alter exit
    }

    while ((dent = readdir(dir))) {

        
        defaultCheck++;

        if (defaultCheck > 2) {
            subPath = pathAddString(path, dent->d_name);
            if (isDir(subPath)) { // recursion if dir
                tmpTotal = tmpTotal + findDirectory(subPath);

            } else if (isRegularFile(subPath)) { // operate if file

                if ((size = getFileSize(subPath)) == -1) {
                    printf("fail get file size : %s\n", subPath);
                } else {
                    if (sflag == 0 && aflag == 1) {
                        printf("file size : %d   path : %s\n", size, subPath);
                    }
                    tmpTotal = tmpTotal + size;
                }
            }


        }


    }
    if (sflag == 0) {
        printf("%8.d   path : %s\n", tmpTotal, path);
    }
    closedir(dir);

    return tmpTotal;
}

void helpPrint() {
    (void) fprintf(stderr, "can not parse args - du [a],[b],[k],[s] path\n"); //stderr
    exit(1);
}

char *pathAddString(char *str1, char *str2) {

    int i = 0;
    int j = 0;
    char *sumStr;
    int str1Len = (int) strlen(str1);
    int str2Len = (int) strlen(str2);


    sumStr = (char *) malloc(sizeof(char *) * (str1Len + str2Len + 1000));
    for (i = 0; i < str1Len; i++) {
        sumStr[i] = str1[i];

    }

    if (sumStr[str1Len - 1] != '/') {
        sumStr[str1Len] = '/';
        i++;
    }

    for (j = 0; j < str2Len; j++) {
        sumStr[i + j] = str2[j];
    }

    return sumStr;
}

int isDir(char *path) {
    struct stat buf;
    int len = (int) strlen(path);
    if (stat(path, &buf) == -1)
        return 0;
    else
        return S_ISDIR(buf.st_mode);
}

int isRegularFile(char *path) {
    struct stat buf;

    if (stat(path, &buf) == -1)
        return 0;
    else
        return S_ISREG(buf.st_mode);

}

int getFileSize(char *path) {
    struct stat buf;
    if (stat(path, &buf) == -1)
        return -1;

    if (bflag == 1)
        return (int) buf.st_size;
    else if (kflag == 1)
        return (int) buf.st_blocks / 2;

    return (int) buf.st_blocks;

}

// int main(int argc, char **argv) {
//     return project_du(argc, argv);
// }