#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
  int i = 0;
  int n = 0;
  FILE* finp = fopen("inp.txt", "r");
  FILE* fout = fopen("out.txt", "w");
  float *a = NULL;
  float *b = NULL;
  float c = 0.;

  fscanf(finp, "%d", &n);
  fprintf(fout, "%d\n", n);

  a = (float*)malloc(n * sizeof(float));
  b = (float*)malloc(n * sizeof(float));

  for (i = 0; i < n; ++i) {
    fscanf(finp, "%f", &(a[i]));
  }
  for (i = 0; i < n; ++i) {
    fscanf(finp, "%f", &(b[i]));
  }
  fclose(finp);
  /* for (i = 0; i < n; ++i) { */
  /*   c = pow(a[i], b[i]); */
  /*   fprintf(fout, "%f ", c); */
  /* } */
  /* fprintf(fout, "\n"); */
  fclose(fout);

  free(a);
  a = NULL;
  free(b);
  b = NULL;

  return 0;
}
