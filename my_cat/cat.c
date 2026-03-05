#include <stdio.h>
#include <string.h>

#define LINE_NUMBER 'n'
#define NO_BLANK 's'
#define SPECIAL_CHARACTER 'A'

void cat(int argc, char *argv[], int flag_n, int flag_s, int flag_A);
void process(FILE *ptr, int flag_n, int flag_s, int flag_A);

int main(int argc, char *argv[]) {
  int flag_n = 0, flag_s = 0, flag_A = 0;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      for (int j = 1; argv[i][j] != '\0'; j++) {
        if (argv[i][j] == LINE_NUMBER)
          flag_n = 1;
        else if (argv[i][j] == NO_BLANK)
          flag_s = 1;
        else if (argv[i][j] == SPECIAL_CHARACTER)
          flag_A = 1;
        else {
          printf("Flag '-%c' not valid\n", argv[i][j]);
          return 1;
        }
      }
    }
  }
  cat(argc, argv, flag_n, flag_s, flag_A);
  return 0;
}

void cat(int argc, char *argv[], int flag_n, int flag_s, int flag_A) {
  if (argc == 1) {
    process(stdin, flag_n, flag_s, flag_A);
    return;
  }
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-')
      continue;
    FILE *ptr = fopen(argv[i], "r");
    if (ptr == NULL) {
      printf("Could not open %s\n", argv[i]);
      continue;
    }
    process(ptr, flag_n, flag_s, flag_A);
    fclose(ptr);
  }
}

void process(FILE *ptr, int flag_n, int flag_s, int flag_A) {
  int c, prev = '\n';
  int line = 1, new_line = 1;

  while ((c = fgetc(ptr)) != EOF) {
    // -s: skip consecutive blank lines
    if (flag_s && c == '\n' && prev == '\n')
      continue;

    // -n: print line number at start of each line
    if (flag_n && new_line) {
      printf("%6d\t", line++);
      new_line = 0;
    }

    if (c == '\n')
      new_line = 1;

    // -A: show special characters
    if (flag_A && c == '\t')
      printf("^I");
    else if (flag_A && c == '\n')
      printf("$\n");
    else
      printf("%c", c);

    prev = c;
  }
}
