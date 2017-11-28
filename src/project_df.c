#include "smallsh.h"

#include <sys/vfs.h>

void explainCommand();
unsigned long GetFileSystemSpace();
unsigned long GetFileSystemAvailableSpace();
void FileCheck();
struct statvfs stat;

int project_df(int argc, char **argv) {
    int n;
    
    if(argc > 2 || argc < 1) {
        perror("It is not in the correct format ");
        exit(1);
    }
    FileCheck(); // get file in current directory
    while((n = getopt(argc,argv,"kga"))!=-1) { // option
        switch(n) {
            case 'k': //kilobyte 
                printf("\nFileSystem");
                printf("\n1024-blocks  : %lu\n",GetFileSystemSpace());
                printf("Available    : %lu\n",GetFileSystemAvailableSpace());
                printf("Used         : %lu\n",GetFileSystemSpace() - GetFileSystemAvailableSpace());
                printf("Capacity     : %lu%%\n\n",(((stat.f_blocks/2)-(stat.f_bavail/2))*100)/(stat.f_blocks/2)+1);
                break;
            case 'g': //gigabyte
                printf("\nFileSystem\n");
                printf("1G-blocks    : %luG\n",GetFileSystemSpace()/1024/1024+1);
                printf("Available    : %luG\n",GetFileSystemAvailableSpace()/1024/1024+1);
                printf("Used         : %luG\n",(GetFileSystemSpace() - GetFileSystemAvailableSpace())/1024/1024);
                printf("Capacity     : %lu%%\n\n",(((stat.f_blocks/2)-(stat.f_bavail/2))*100)/(stat.f_blocks/2)+1);
                break;
            case 'a': //filesystem all infomation
                printf("\nFileSystem\n");
                printf("Block size      : %lu\n",stat.f_bsize);
                printf("Flag size       : %lu\n",stat.f_frsize);
                printf("Total blocks    : %lu\n",stat.f_blocks);
                printf("Free blocks     : %lu\n",stat.f_bfree);
                printf("Available       : %lu\n",stat.f_bavail);
                printf("Total files     : %lu\n",stat.f_files);
                printf("Free files      : %lu\n",stat.f_ffree);
                printf("Filesystem id   : %lu\n",stat.f_fsid);
                printf("Filename length : %lu\n\n",stat.f_namemax);
                break;
            case '?':
            default:
                explainCommand(); // option explain
                break;
        }        
        unlink("fileSystemTest"); // temporary file remove
        exit(1);
    }
    printf("\nFileSystem\n");
    printf("Total blocks    : %lu\n",stat.f_blocks);
    printf("Total files     : %lu\n\n",stat.f_files);
    unlink("fileSystemTest");
    return 0;
}

void explainCommand() {
    printf("Usage : df\n\n");
    printf("[-k] : Expressed in kilobytes.\n");
    printf("[-g] : Expressed in gigabytes.\n");
    printf("[-a] : Dispalys all information in the filesystem.\n\n");
    exit(1);
}

unsigned long GetFileSystemSpace() { //return 1k-block
    return (unsigned long)(stat.f_blocks/2);
}

unsigned long GetFileSystemAvailableSpace() {//return available block
    return (unsigned long)((stat.f_bavail)/2);
}

void FileCheck() { //create temporaryfile
    creat("fileSystemTest",0777);
    if(statvfs("fileSystemTest",&stat) == -1) {
        perror("Error ");
        exit(1);
    }
}