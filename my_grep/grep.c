#include "dirent.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>
#include <sys/types.h>
char directory_name[1024];
char string[1024];
void grep(char *directory_name, char *string);
int main() {
  printf("Give me a directory_name: ");
  fflush(stdout); // Force the prompt to show up immediately
  scanf("%1023s", directory_name);
  while (getchar() != '\n')
    ;
  printf("Give me a string to find: ");
  fflush(stdout);
  scanf("%255s", string);
  grep(directory_name, string);
}

void grep(char *directory_name, char *string) {
  if (strlen(directory_name) > 1024) {
    printf("Path too long: Cannot go that deep\n");
    return; // Return instead of exit to allow other branches to continue
  }
  DIR *dir = opendir(directory_name);
  if (dir == NULL) {
    printf("Failed to open the directory\n");
    return;
  }
  struct dirent *stream;
  char *file_name;
  char *folder_name;
  while ((stream = readdir(dir)) != NULL) {
    if (stream->d_type == 4) {
      folder_name = stream->d_name;
      char new_dir[2048];
      if (strcmp(stream->d_name, ".") != 0 &&
          strcmp(stream->d_name, "..") != 0 &&
          strcmp(stream->d_name, ".git") != 0) {
        snprintf(new_dir, sizeof(new_dir), "%s%s/", directory_name,
                 folder_name);
        // printf("Opening new_dir: %s\n", new_dir);
        grep(new_dir, string);
      }
    } else if (stream->d_type == 8) {
      file_name = stream->d_name;
      char full_path[2048];
      snprintf(full_path, sizeof(full_path), "%s%s", directory_name, file_name);
      FILE *file_ptr = fopen(full_path, "r");
      char c = fgetc(file_ptr);
      int i = 1;
      int found_first = 0;
      int found = 0;
      int enter = 1;
      while (c != EOF) {
        if ((strlen(string) == 1) && (string[0] == c)) {
          found++;
        }
        if ((found_first) && (c == string[i])) {
          i++;
          if (i == (strlen(string))) {
            found++;
            i = 1;
            found_first = 0;
          }
        } else {
          i = 1;
          found_first = 0;
          enter = 1;
        };
        if ((c == string[0]) && enter) {
          found_first = 1;
          i = 1;
          enter = 0;
        };
        c = fgetc(file_ptr);
      }
      if (found > 0) {
        printf("File: %s\n", full_path);
        printf("Found %i occurences\n", found);
      }
      fclose(file_ptr);
    }
  }
  closedir(dir);
}
