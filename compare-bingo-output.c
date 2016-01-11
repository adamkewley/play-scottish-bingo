#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 20
#define NUMBER_OF_LINES 52

struct input {
  char lines[NUMBER_OF_LINES][MAX_LINE_LEN];
  int len;
};

int main(int argc, char **argv) {
  char *apath = argv[1];
  char *bpath = argv[2];

  FILE *fa;
  FILE *fb;
  if ((fa = fopen(apath, "r")) == NULL) {
    fprintf(stderr, "Error opening %s\n", apath); 
    return 1;
  }
  
  if ((fb = fopen(bpath, "r")) == NULL) {
    fprintf(stderr, "Error opening %s\n", bpath);
    return 1;
  }

  struct input a = {};
  struct input b = {};

  // Read entire file into memory rather than
  // jumping between *fa and *fb to mazimize
  // access efficiency (files won't be huge)
  while(fgets(a.lines[a.len], MAX_LINE_LEN, fa) != NULL && a.len <= NUMBER_OF_LINES)
    ++a.len;

  while(fgets(b.lines[b.len], MAX_LINE_LEN, fb) != NULL && b.len <= NUMBER_OF_LINES)
    ++b.len;

  for (int i = 0; i < a.len; i++)
    if (strcmp(a.lines[i], b.lines[i]) == 0)
      printf("%s", a.lines[i]);
}
