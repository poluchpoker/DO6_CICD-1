#ifndef S21_CAT_H
#define S21_CAT_H

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int b, e, n, s, t, v;
} cat_flags;

void CatWithoutArgs(int fd, char *argv[], int i);
void process_work_at_flags(cat_flags *flags, int argc, char *argv[]);
void open_file(cat_flags *flags, char *argv[], int a);
void working_cat(cat_flags *flags, FILE *fd);
int flagS(char cur, char prev, int empty_str_count);
int flagB(char cur, char prev, int str_count);
int flagN(char prev, int str_count);
void flagE(char cur);
char flagT(int cur);
char flagV(int cur);

#endif