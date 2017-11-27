#ifndef SMALLSH_H
#define SMALLSH_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

// common constant values
#define MAXARG 512 /* max. no. command args */
#define MAXBUF 512 /* max. length input line */
#define FOREGROUND 0
#define BACKGROUND 1

#define ALIAS_FILE_NAME "/.alias"
#define TMP_ALIAS_FILE_NAME "/.alias_tmp"

#define HISTORY_FILE_NAME "/.history"

#define PROMPT_SIZE 256
#define CWD_SIZE 256
#define PATH_SIZE 256
#define BUF_SIZE 512

// default functions
int userin(char *p);
int gettok(char **outptr);
int inarg(char c);
void procline();
int runcommand(int argc, char **cline, char where);

// project functions
void prompt(char *prom);

void command_parser(int pid, int argc, char **cline);
int alias_check(char *);

static char *arg_pipe[20][MAXARG+1];
static char *arg_redirection[20][MAXARG+1];
int runcommand_pipe(int argc, char where);
int runcommand_redirection(int argc, char where, int special_type);

int project_cd(int argc, char **argv);

int project_history(int argc, char **argv);
int project_alias(int argc, char **argv);

int project_ps(int argc, char **argv);
int project_df(int argc, char **argv);
int project_du(int argc, char **argv);

#endif