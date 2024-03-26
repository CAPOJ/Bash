#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct arguments {
  int e, i, v, c, l, n, h, s, f, o;
  char *pattern;
  int len_pattern;
  int mem_pattern;

} arguments;

void output(char *line, int n);
void print(arguments arg, int argc, char **argv);
void processFile(arguments arg, char *path, regex_t *reg);
void printReg(arguments arg, regex_t *reg, char *line, char *path,
              int line_count);
void processLine(arguments arg, char *path, int line_count);
void add_reg_from_file(arguments *arg, char *filepath);
arguments parser(int argc, char *argv[]);
void pattern_add(arguments *arg, char *pattern);

int main(int argc, char **argv) {
  arguments arg = parser(argc, argv);
  print(arg, argc, argv);
  free(arg.pattern);
  return 0;
}

void print(arguments arg, int argc, char **argv) {
  regex_t re;
  int error = regcomp(&re, arg.pattern, REG_EXTENDED | arg.i);
  if (error) {
    perror("Error");
  }
  for (int i = optind; i < argc; i++) {
    processFile(arg, argv[i], &re);
  }
  regfree(&re);
}

void processFile(arguments arg, char *way, regex_t *reg) {
  FILE *f = fopen(way, "r");
  if (f == NULL) {
    if (!arg.s) {
      perror(way);
    }
    exit(1);
  }
  char *line = NULL;
  size_t memlen = 0;
  int isEOF = 0;
  int line_count = 1;
  int c = 0;
  isEOF = getline(&line, &memlen, f);

  while (isEOF != -1) {
    int result = regexec(reg, line, 0, NULL, 0);
    if ((result == 0 && !arg.v) || (arg.v && result != 0)) {
      if (!arg.c && !arg.l) {
        if (!arg.o) {
          if (!arg.h) {
            printf("%s:", way);
          }
          if (arg.n) {
            printf("%d:", line_count);
          }
        }
        if (arg.o) {
          printReg(arg, reg, line, way, line_count);
        } else {
          output(line, isEOF);
        }
      }
      c++;
    }
    isEOF = getline(&line, &memlen, f);
    line_count++;
  }
  if (arg.c && !arg.l) {
    if (!arg.h) {
      printf("%s:", way);
    }
    printf("%d\n", c);
  }
  if (arg.l && c > 0) {
    printf("%s\n", way);
  }
  free(line);
  fclose(f);
}

void printReg(arguments arg, regex_t *reg, char *line, char *path,
              int line_count) {
  regmatch_t match;
  int offset = 0;
  while (1) {
    int result = regexec(reg, line + offset, 1, &match, 0);

    if (result != 0) {
      break;
    }

    processLine(arg, path, line_count);

    for (int i = match.rm_so; i < match.rm_eo; i++) {
      putchar(line[offset + i]);
    }
    putchar('\n');
    offset += match.rm_eo;
  }
}

void processLine(arguments arg, char *path, int line_count) {
  if (!arg.c && !arg.l) {
    if (!arg.h) {
      printf("%s:", path);
    }
    if (arg.n) {
      printf("%d:", line_count);
    }
  }
}

void output(char *line, int n) {
  for (int i = 0; i < n; i++) {
    putchar(line[i]);
  }
  if (line[n - 1] != '\n') {
    putchar('\n');
  }
}

arguments parser(int argc, char *argv[]) {
  arguments arg = {0};
  int opt;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (opt) {
      case 'e':
        arg.e = 1;
        pattern_add(&arg, optarg);
        break;
      case 'i':
        arg.i = REG_ICASE;
        break;
      case 'v':
        arg.v = 1;
        break;
      case 'c':
        arg.c = 1;
        break;
      case 'l':
        arg.c = 1;
        arg.l = 1;
        break;
      case 'n':
        arg.n = 1;
        break;
      case 'h':
        arg.h = 1;
        break;
      case 's':
        arg.s = 1;
        break;
      case 'f':
        arg.f = 1;
        add_reg_from_file(&arg, optarg);
        break;
      case 'o':
        arg.o = 1;
        break;
    }
  }
  if (arg.len_pattern == 0) {
    pattern_add(&arg, argv[optind]);
    optind++;
  }
  if (argc - optind == 1) {
    arg.h = 1;
  }

  return arg;
}

void add_reg_from_file(arguments *arg, char *filepath) {
  FILE *f = fopen(filepath, "r");
  if (f == NULL) {
    if (!arg->s) {
      perror(filepath);
    }
    exit(1);
  }
  char *line = NULL;
  size_t memlen = 0;
  int read = getline(&line, &memlen, f);
  while (read != -1) {
    if (line[read - 1] == '\n') {
      line[read - 1] = '\0';
    }

    pattern_add(arg, line);
    read = getline(&line, &memlen, f);
  }
  free(line);
  fclose(f);
}

void pattern_add(arguments *arg, char *pattern) {
  int n = strlen(pattern);
  if (arg->len_pattern == 0) {
    arg->pattern = malloc(1024 * sizeof(char));
    arg->pattern[0] = '\0';
    arg->mem_pattern = 1024;
  }
  if (arg->mem_pattern < arg->len_pattern + n) {
    arg->pattern = realloc(arg->pattern, arg->mem_pattern * 2 * sizeof(char));
    arg->mem_pattern *= 2;
  }
  if (arg->len_pattern != 0) {
    strcat(arg->pattern + arg->len_pattern, "|");
    arg->len_pattern++;
  }
  arg->len_pattern += sprintf(arg->pattern + arg->len_pattern, "(%s)", pattern);
}
