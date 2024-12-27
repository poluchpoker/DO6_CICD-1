#include "s21_cat.h"

int main(int argc, char *argv[]) {
  if (argc == 1)
    CatWithoutArgs(STDIN_FILENO, argv, 0);
  else {
    cat_flags flags = {.b = 0, .e = 0, .n = 0, .s = 0, .t = 0, .v = 0};
    process_work_at_flags(&flags, argc, argv);
  }

  return 0;
}

void CatWithoutArgs(int fd, char *argv[], int i) {
  if (fd != -1) {
    char buf[100];
    int bytes_read;

    bytes_read = read(fd, buf, 100);

    while (bytes_read > 0) {
      printf("%.*s", bytes_read, buf);
      bytes_read = read(fd, buf, 100);
    }
  } else {
    fprintf(stderr, "cat: %s: No such file or directory", argv[i]);
  }
}

void process_work_at_flags(cat_flags *flags, int argc, char *argv[]) {
  int option = 0, exit_from_prog = 0;
  const char *short_options = "beEnstTv";
  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {0, 0, 0, 0}};
  while ((option = getopt_long(argc, argv, short_options, long_options,
                               NULL)) != -1) {
    switch (option) {
      case 'b':
        flags->b = 1;
        break;
      case 'e':
        flags->e = 1, flags->v = 1;
        break;
      case 'E':
        flags->e = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 't':
        flags->t = 1, flags->v = 1;
        break;
      case 'T':
        flags->t = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      default:
        printf("cat: illegal option -- %c\n", option);
        printf("usage: cat [-benstuv] [file ...]\n");
        exit_from_prog = 1;
        break;
    }
  }
  int rez = (flags->b + flags->e + flags->n + flags->s + flags->t + flags->v);
  int a = 0;
  if (argc > 2 && rez && !exit_from_prog) {
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] != '-') {
        a = 1;
        open_file(flags, argv, a);
        optind++;
      }
    }
  } else if (!exit_from_prog && !rez) {
    for (int i = 1; i < argc; i++) {
      CatWithoutArgs(open(argv[i], O_RDONLY), argv, i);
    }
  } else if (rez && !exit_from_prog) {
    open_file(flags, argv, a);
  }
}

void open_file(cat_flags *flags, char *argv[], int a) {
  FILE *fd;
  if (a != 0) {
    fd = fopen(argv[optind], "r+");
    if (!fd)
      fprintf(stderr, "cat: %s: No such file or directory", argv[1]);
    else
      working_cat(flags, fd);
  } else if (a == 0) {
    fd = stdin;
    while (1) working_cat(flags, fd);
  }
  fclose(fd);
}

void working_cat(cat_flags *flags, FILE *fd) {
  int last = '\n', str_count = 0, empty_str_count = 0;
  if (flags->b & flags->n) flags->n = 0;

  while (!feof(fd)) {
    int cur = fgetc(fd);

    if (cur == EOF) break;

    if (flags->s) {
      if ((empty_str_count = flagS(cur, last, empty_str_count)) > 1) continue;
    }
    if (flags->b) {
      str_count = flagB(cur, last, str_count);
    }
    if (flags->n) {
      str_count = flagN(last, str_count);
    }
    if (flags->t) {
      cur = flagT(cur);
    }
    if (flags->e) {
      flagE(cur);
    }
    if (flags->v) {
      cur = flagV(cur);
    }
    printf("%c", cur);
    last = cur;
  }
}

int flagS(char cur, char prev, int empty_str_count) {
  if (cur == '\n' && prev == '\n') {
    empty_str_count++;
  } else {
    empty_str_count = 0;
  }

  return empty_str_count;
}

int flagB(char cur, char prev, int str_count) {
  if (prev == '\n' && cur != '\n') {
    printf("%6d\t", ++str_count);
  }
  return str_count;
}

int flagN(char prev, int str_count) {
  if (prev == '\n') printf("%6d\t", ++str_count);

  return str_count;
}

void flagE(char cur) {
  if (cur == '\n') printf("$");
}

char flagT(int cur) {
  if (cur == '\t') {
    printf("^");
    cur = 'I';
  }
  return cur;
}

char flagV(int cur) {
  if ((cur >= 0 && cur < 9) || (cur > 10 && cur < 32) ||
      (cur > 126 && cur <= 160)) {
    printf("^");
    if (cur > 126) {
      cur -= 64;
    } else {
      cur += 64;
    }
  }

  return cur;
}