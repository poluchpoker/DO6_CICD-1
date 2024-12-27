#include "s21_grep.h"

int main(int argc, char *argv[]) {
  grep_flags flags = {.e = 0,
                      .i = 0,
                      .v = 0,
                      .c = 0,
                      .l = 0,
                      .n = 0,
                      .h = 0,
                      .s = 0,
                      .f = 0,
                      .o = 0};
  info information = {.a = 0, .b = 0, .d = 0, .z = 0};
  char pattern[2048] = {0};
  int *lines = &information.a, *count_files = &information.b,
      *empty_lines = &information.d, *eCount = &information.z;
  if (argc > 1) {
    parser(argc, argv, &flags, eCount, empty_lines, pattern);
    reader_file(argc, argv, count_files, lines, &flags, pattern);
  }

  return 0;
}

void parser(int argc, char *argv[], grep_flags *flags, int *eCount,
            int *empty_lines, char *pattern) {
  int flag;
  char *short_options = "e:ivclnhsf:o";
  while ((flag = getopt_long(argc, argv, short_options, 0, 0)) != -1) {
    switch (flag) {
      case 'e':
        flags->e = 1;
        *eCount = *eCount + 1;
        flagE(eCount, pattern);
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        flagF(eCount, empty_lines, pattern);
        break;
      case 'o':
        flags->o = 1;
        break;
      default:
        exit(EXIT_FAILURE);
    }
    if (flags->v && flags->o) flags->o = 0;
    if (flags->v && flags->c) flags->o = 0;
  }
  if (!flags->e && !flags->f) strcat(pattern, argv[optind++]);
}

void flagE(int *eCount, char *pattern) {
  if (*eCount > 1) {
    strcat(pattern, "|");
  }
  strcat(pattern, optarg);
}

void flagF(int *eCount, int *empty_lines, char *pattern) {
  FILE *fd = NULL;
  char buffer[2048] = {0};
  if ((fd = fopen(optarg, "r"))) {
    while (fgets(buffer, 2048, fd) != NULL) {
      if (buffer[strlen(buffer) - 1] == '\n') {
        buffer[strlen(buffer) - 1] = 0;
      }
      if (*eCount > 0) {
        strcat(pattern, "|");
      }
      if (*buffer == '\0') {
        *empty_lines = 1;
        strcat(pattern, ".");
      } else {
        strcat(pattern, buffer);
      }
      *eCount = *eCount + 1;
    }
    fclose(fd);
  } else {
    printf("s21_grep: No such file or directory\n");
    exit(EXIT_FAILURE);
  }
}

void reader_file(int argc, char *argv[], int *count_files, int *lines,
                 grep_flags *flags, char *pattern) {
  *count_files = argc - optind;
  while (optind < argc) {
    FILE *fd = fopen(argv[optind], "r");
    if (fd != NULL) {
      result(argv, fd, lines, count_files, flags, pattern);
      fclose(fd);
    } else if (!flags->s) {
      fprintf(stderr, "s21_grep: %s: No such file or directory\n",
              argv[optind]);
    }
    ++optind;
  }
}

void result(char *argv[], FILE *fd, int *lines, int *count_files,
            grep_flags *flags, char *pattern) {
  regex_t regex;
  int cflags = REG_EXTENDED;
  char buffer[2048];
  regmatch_t regmatch[1] = {0};
  size_t file_lines_count = 1;
  *lines = 0;
  if (flags->i) cflags = REG_ICASE;
  regcomp(&regex, pattern, cflags);
  while (!feof(fd)) {
    if (fgets(buffer, 2048, fd)) {
      int is_new_line = 1;
      int status = regexec(&regex, buffer, 1, regmatch, 0);
      if (flags->v) status = status ? 0 : 1;
      if (status != REG_NOMATCH) {
        if (!flags->c && !flags->l) {
          if (!flags->h && *count_files > 1) printf("%s:", argv[optind]);
          if (flags->n) printf("%lu:", file_lines_count);
          if (flags->o && !flags->v) {
            is_new_line = 0;
            char *prt = buffer;
            while (!status) {
              printf("%.*s\n", (int)(regmatch[0].rm_eo - regmatch[0].rm_so),
                     prt + regmatch[0].rm_so);
              prt += regmatch[0].rm_eo;
              status = regexec(&regex, prt, 1, regmatch, REG_NOTBOL);
            }
          }
          if (!flags->o) printf("%s", buffer);
          if (buffer[strlen(buffer) - 1] != '\n' && is_new_line) printf("\n");
        }
        *lines = *lines + 1;
      }
      file_lines_count++;
    }
  }
  if (flags->c) {
    if (!flags->h && *count_files > 1) printf("%s:", argv[optind]);
    if (flags->l && *lines) {
      printf("1\n");
    } else {
      printf("%d\n", *lines);
    }
  }
  if (flags->l && *lines) printf("%s\n", argv[optind]);
  regfree(&regex);
}