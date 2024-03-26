#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct arguments {
  int b, n, s, E, T, v;
} arguments;
static const char SHORTOPTS[] = "bneETts";

arguments parser(int argc, char** argv) {
  arguments ex = {0};
  int opt = 0;
  struct option for_log_opt[] = {{"number", no_argument, NULL, 'n'},
                                 {"number-nonblank", no_argument, NULL, 'b'},
                                 {"squeeze-blank", no_argument, NULL, 's'},
                                 {"show-ends", no_argument, NULL, 'E'},
                                 {"show-tabs", no_argument, NULL, 'T'}};
  opt = getopt_long(argc, argv, SHORTOPTS, for_log_opt, 0);
  switch (opt) {
    case 'b':
      ex.b = 1;
      break;
    case 'n':
      ex.n = 1;
      break;
    case 'e':
      ex.E = 1;
      ex.v = 1;
      break;
    case 'E':
      ex.E = 1;
      break;
    case 't':
      ex.T = 1;
      ex.v = 1;
      break;
    case 'T':
      ex.T = 1;
      break;
    case 's':
      ex.s = 1;
      break;
    default:
      perror("ERROR");
      exit(1);
      break;
  }
  return ex;
}

char v(char ch) {
  if (ch == '\t' || ch == '\n') return ch;
  if (ch < 0) {
    printf("M-");
    ch = ch & 0x7F;
  }
  if (ch > 0 && ch <= 31) {
    putchar('^');
    ch += 64;
  } else if (ch == 127) {
    putchar('^');
    ch = '?';
  }
  return ch;
}

int isEmpty(char* line, int n) {
  int kol = 0;
  int flg = 1;
  for (int i = 0; i < n; i++) {
    if (line[i] != '\n') {
      kol++;
      flg = 0;
    }
  }
  return flg;
}
void output(arguments* arg, char* line, int n) {
  for (int i = 0; i < n; i++) {
    if (arg->E && line[i] == '\n') putchar('$');
    if (arg->v) {
      line[i] = v(line[i]);
      // printf("!!!!!!");
    }
    if (arg->T) {
      if (line[i] == '\t') printf("^I");
    }
    putchar(line[i]);
  }
}
void print(arguments* arg, char* argv[]) {
  FILE* file = fopen(argv[optind], "r");
  int isEOF = 0;
  char* line = NULL;
  size_t size = 0;
  isEOF = getline(&line, &size, file);
  int kolvoOb = 1;
  int fl = 0;
  while (isEOF != -1) {
    int isemp = isEmpty(line, isEOF);
    if (arg->n) {
      printf("%6d\t", kolvoOb);
      output(arg, line, isEOF);
      kolvoOb++;
    }
    if (arg->s) {
      if (isemp == 1) {
        if (fl == 0) output(arg, line, isEOF);
        fl = 1;
      } else {
        fl = 0;
        output(arg, line, isEOF);
      }
    }
    if (arg->b) {
      if (isemp == 0) {
        printf("%6d\t", kolvoOb);
        kolvoOb++;
      }
      output(arg, line, isEOF);
    }
    if ((arg->E == 1) || (arg->T == 1)) {
      output(arg, line, isEOF);
    }
    isEOF = getline(&line, &size, file);
  }
  free(line);
  fclose(file);
}

void readFile() {}

int main(int argc, char* argv[]) {
  arguments opi = parser(argc, argv);
  print(&opi, argv);
  return 0;
}