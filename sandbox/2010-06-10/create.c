#include <stdio.h>

int main() {
  int i = 0;
  int n = 10000;
  FILE* fout = fopen("inp.txt", "w");

  fprintf(fout, "%d", n);
  fprintf(fout, "\n");
  for (i = 0; i < n; ++i) {
    fprintf(fout, "%f ", 1. + .01 * (float)i / (float)n);
  }
  fprintf(fout, "\n");
  for (i = 0; i < n; ++i) {
    fprintf(fout, "%f ", 100. + 10. * (float)i / (float)n);
  }
  fprintf(fout, "\n");
  fclose(fout);

  return 0;
}
