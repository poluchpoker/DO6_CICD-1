#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 2048

typedef struct {
  int e, i, v, c, l, n, h, s, f, o;
} grep_flags;

typedef struct {
  int a, b, d, z;
} info;

void parser(int argc, char *argv[], grep_flags *flags, int *eCount,
            int *empty_lines, char *pattern);
void flagE(int *eCount, char *pattern);
void flagF(int *eCount, int *empty_lines, char *pattern);
void reader_file(int argc, char *argv[], int *count_files, int *lines,
                 grep_flags *flags, char *pattern);
void result(char *argv[], FILE *fd, int *lines, int *count_files,
            grep_flags *flags, char *pattern);

#endif