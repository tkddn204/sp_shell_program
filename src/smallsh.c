int userin(char *p) {
    FILE *fp;
    char *home = getenv("HOME");
    char path[PATH_SIZE];
    int c, count;

    strcpy(path, home);
    strcat(path, "/.history");

    if ((fp = fopen(path, "a")) == NULL) {
        perror("history file open");
    }

    /* initialization for later routines */
    ptr = inpbuf;
    tok = tokbuf;
    /* display prompt */
    printf("%s ", p);
    count = 0;
    while (1) {
        if ((c = getchar()) == EOF) {
            fclose(fp);
            return EOF;
        }
        if (count < MAXBUF) inpbuf[count++] = c;
        if (c == '\n' && count < MAXBUF) {
            inpbuf[count] = '\0';
            break;
        }
        /* if line too long, restart */
        if (c == '\n') {
            printf(" smallsh : input line too long\n");
            count = 0;
            printf("%s ", p);
        }
    }

    if (count == 1 && inpbuf[count - 1] == '\n') {
        fclose(fp);
        return count;
    }

    fputs(inpbuf, fp);
    fclose(fp);
    return count;
}