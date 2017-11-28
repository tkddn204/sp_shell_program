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
    //printf("du - 20131705 Bae Min Su \n");

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


    if (optCount == 0) { // 옵션이 없을시
        if (argv[1] == NULL) {
            getcwd(buf, 1024);  // 인자가 없을시 현재경로설정
            filePath = buf;
        } else {
            filePath = argv[1]; // 인자가 있을시 인자를 경로로 설정
        }
    } else { // 옵션이 존재할 시
        if (argv[2] == NULL) { // 인자로 경로가 주어지지 않을시 현재경로 설정
            getcwd(buf, 1024);
            filePath = buf;
        } else {                // 인자가 있을시 인자를 경로로 설정
            filePath = argv[2];
        }
    }

    //탐색 시작. 결과값으로 총 용량이 total에 저장
    int total = findDirectory(filePath);

    printf("total : %d\n", total);

    return total;
}


//실제 디렉토리를 탐색하는 함수 (재귀 구조)
int findDirectory(char *path) {

    struct dirent *dent;
    DIR *dir;

    char *subPath;
    int size, tmpTotal = 0;
    int defaultCheck = 0;

    // 디렉토리 포인터 할당
    dir = opendir(path);
    if (dir == NULL) {
        perror("Failed to open directory");
        return -1; // alter exit
    }

    while ((dent = readdir(dir))) {



        // . 과 .. 건너뛰기 위한 처리
        defaultCheck++;

        if (defaultCheck > 2) {

            //디렉토리 내의 파일들을 읽어 새로운 경로 생성
            subPath = pathAddString(path, dent->d_name);

            //해당 파일이 디렉토리일시 재귀
            if (isDir(subPath)) {
                tmpTotal = tmpTotal + findDirectory(subPath);

                //일반 파일일시 파일사이즈 측정
            } else if (isRegularFile(subPath)) {

                if ((size = getFileSize(subPath)) == -1) {
                    printf("fail get file size : %s\n", subPath);
                } else {
                    //-a옵션이 있을시 파일의 사이즈  출력
                    if (sflag == 0 && aflag == 1) {
                        printf("file size : %d   path : %s\n", size, subPath);
                    }
                    tmpTotal = tmpTotal + size;
                }
            }


        }


    }
    // 해당 디렉토리의 총 용량을 출력
    if (sflag == 0) {
        printf("%8.d   path : %s\n", tmpTotal, path);
    }
    closedir(dir);

    // 해당 디렉토리에서 구한 총 용량을 리턴
    return tmpTotal;
}

void helpPrint() {
    (void) fprintf(stderr, "can not parse args - du [a],[b],[k],[s] path\n"); //stderr
    exit(1);
}


//파일 탐색에따른 경로추가를 위한 함수
char *pathAddString(char *str1, char *str2) {

    int i = 0;
    int j = 0;
    char *sumStr;
    int str1Len = (int) strlen(str1);
    int str2Len = (int) strlen(str2);


    //새로운 경로생성을 위한 메모리할당
    sumStr = (char *) malloc(sizeof(char *) * (str1Len + str2Len + 1000));

    //기존의 경로를 합칠 변수에 저장
    for (i = 0; i < str1Len; i++) {
        sumStr[i] = str1[i];

    }

    //경로 구분자 추가
    if (sumStr[str1Len - 1] != '/') {
        sumStr[str1Len] = '/';
        i++;
    }

    //기존 경로에 추가될 파일 or 디렉토리명 추가
    for (j = 0; j < str2Len; j++) {
        sumStr[i + j] = str2[j];
    }

    return sumStr;
}
// 디렉토리인지 검사하는 함수
int isDir(char *path) {
    struct stat buf;
    int len = (int) strlen(path);
    if (stat(path, &buf) == -1)
        return 0;
    else
        return S_ISDIR(buf.st_mode);
}


// 일반파일인지 검사하는 함수
int isRegularFile(char *path) {
    struct stat buf;

    if (stat(path, &buf) == -1)
        return 0;
    else
        return S_ISREG(buf.st_mode);

}


//읽은 파일의 경로를 통해 파일사이즈를 구함
int getFileSize(char *path) {
    struct stat buf;
    if (stat(path, &buf) == -1)
        return -1;


    // opt -b의 경우 바이트단위 리턴
    if (bflag == 1)
        return (int) buf.st_size;

    // opt -k 킬로바이트단위 리턴
    else if (kflag == 1)
        return (int) buf.st_blocks / 2;

    //옵션 없을시 stat 구조체 blocks 단위 출력
    return (int) buf.st_blocks;

}