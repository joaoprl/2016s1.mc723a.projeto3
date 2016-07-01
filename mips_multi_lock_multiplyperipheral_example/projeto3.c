#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define START_ADDRESS (200<<20)

volatile static uint32_t procCounter = 0;

volatile uint32_t *lock = (volatile uint32_t *) START_ADDRESS;

volatile uint32_t *p_status00 =   (volatile uint32_t*) ((uint32_t)(START_ADDRESS) +  4); // exponentiation 00
volatile uint32_t *p_base00 =     (volatile uint32_t*) ((uint32_t)(START_ADDRESS) +  8);
volatile uint32_t *p_exponent00 = (volatile uint32_t*) ((uint32_t)(START_ADDRESS) + 12);
volatile uint32_t *p_status01 =   (volatile uint32_t*) ((uint32_t)(START_ADDRESS) + 16); // exponentiation 01
volatile uint32_t *p_base01 =     (volatile uint32_t*) ((uint32_t)(START_ADDRESS) + 20);
volatile uint32_t *p_exponent01 = (volatile uint32_t*) ((uint32_t)(START_ADDRESS) + 24);
volatile uint32_t *p_status02 =   (volatile uint32_t*) ((uint32_t)(START_ADDRESS) + 28); // exponentiation 02
volatile uint32_t *p_base02 =     (volatile uint32_t*) ((uint32_t)(START_ADDRESS) + 32);
volatile uint32_t *p_exponent02 = (volatile uint32_t*) ((uint32_t)(START_ADDRESS) + 36);
volatile uint32_t *p_status03 =   (volatile uint32_t*) ((uint32_t)(START_ADDRESS) + 40); // exponentiation 03
volatile uint32_t *p_base03 =     (volatile uint32_t*) ((uint32_t)(START_ADDRESS) + 44);
volatile uint32_t *p_exponent03 = (volatile uint32_t*) ((uint32_t)(START_ADDRESS) + 48);

float *p_interest = NULL;
float *p_time = NULL;
float *p_capital = NULL;
float *p_result = NULL;
uint32_t N = 0;

volatile uint32_t wait_load = 1;
volatile uint32_t p_finished[4] = {0};

void AcquireLock(){
  while(*lock);
}

void ReleaseLock(){
  *lock = 0;
}

void submain() {
  AcquireLock();
  volatile uint32_t procNumber = procCounter;
  ++procCounter;
  ReleaseLock();

  if (0 < procNumber)
    return;

  /* AcquireLock(); */
  /* printf("procNumber address: %#10x\n", &procNumber);  */
  /* printf("procNumber %d\n ", procNumber); */
  /* ReleaseLock(); */

  FILE *p_file = NULL;
  uint32_t i = 0, n = 0;
  float total_capital = 0., total_result = 0.;
  /* float interest = 0.; */
  /* float time = 0.; */
  /* float capital = 0.; */

  volatile uint32_t status = 0;
  volatile uint32_t *p_status = NULL;
  volatile float *p_base = NULL, *p_exponent = NULL;

  if (procNumber == 0) {
    AcquireLock();
    printf("Reading input files.\n");
    ReleaseLock();

    /* interest */
    p_file = fopen("./y4k_monthly_interest.txt", "r");
    if (p_file == NULL) {
      printf("Error opening ./y4k_monthly_interest.txt\n");
      exit(-1);
    }
    fscanf(p_file, "%zu", &n);
    p_interest = (float*)(calloc(n, sizeof(float)));
    for (i = 0; i < n; ++i) {
      fscanf(p_file, "%f", &(p_interest[i]));
    }
    fclose(p_file);

    /* time */
    p_file = fopen("./y4k_investment_time.txt", "r");
    if (p_file == NULL) {
      printf("Error opening ./y4k_investment_time.txt\n");
      exit(-1);
    }
    fscanf(p_file, "%zu", &n);
    p_time = (float*)(calloc(n, sizeof(float)));
    for (i = 0; i < n; ++i) {
      fscanf(p_file, "%f", &(p_time[i]));
    }
    fclose(p_file);

    /* capital */
    p_file = fopen("./y4k_initial_capital.txt", "r");
    if (p_file == NULL) {
      printf("Error opening ./y4k_initial_capital.txt\n");
      exit(-1);
    }
    fscanf(p_file, "%zu", &n);
    p_capital = (float*)(calloc(n, sizeof(float)));
    for (i = 0; i < n; ++i) {
      fscanf(p_file, "%f", &(p_capital[i]));
      total_capital += p_capital[i];
    }
    fclose(p_file);

    N = n;
    p_result = (float*)(calloc(n, sizeof(float)));
    wait_load = 0;
  }

  while(wait_load == 1);

  if (procNumber == 0) {
    p_status = p_status00;
    p_base = (volatile float*)p_base00;
    p_exponent = (volatile float*)p_exponent00;
  } else if (procNumber == 1) {
    p_status = p_status01;
    p_base = (volatile float*)p_base01;
    p_exponent = (volatile float*)p_exponent01;
  } else if (procNumber == 2) {
    p_status = p_status02;
    p_base = (volatile float*)p_base02;
    p_exponent = (volatile float*)p_exponent02;
  } else if (procNumber == 3) {
    p_status = p_status03;
    p_base = (volatile float*)p_base03;
    p_exponent = (volatile float*)p_exponent03;
  }

  /* result */
  AcquireLock();
  printf("Starting exponentiation %zu\n", procNumber);
  ReleaseLock();
  for (i = procNumber; i < N; i += 1) {
    p_result[i] = p_capital[i] * pow(p_interest[i], p_time[i]);

    /* *p_base = p_interest[i]; */
    /* *p_exponent = p_time[i]; */
    /* *p_status = 1; */
    /* while(*p_status != 0); */
    /* p_result[i] = p_capital[i] * (*p_base); */
  }
  p_finished[procNumber] = 1;

  if (procNumber == 0) {
    /* /\* result *\/ */
    /* p_result = (float*)(calloc(n, sizeof(float))); */
    /* for (i = 0; i < n; ++i) { */
    /*   /\* p_result[i] = p_capital[i] * pow(p_interest[i], p_time[i]); *\/ */

    /*   *p_base = p_interest[i]; */
    /*   *p_exponent = p_time[i]; */
    /*   *p_status = 1; */
    /*   while(*p_status != 0); */
    /*   p_result[i] = p_capital[i] * (*p_base); */
    /* } */

    while(p_finished[0] == 0 && p_finished[1] == 0 && p_finished[2] == 0 && p_finished[3] == 0);

    AcquireLock();
    printf("Writing output files and calculating total values.\n");
    ReleaseLock();

    p_file = fopen("./y4k_result0.txt", "w");
    /* p_file = fopen("./y4k_result1.txt", "w"); */
    /* p_file = fopen("./y4k_result2.txt", "w"); */
    fprintf(p_file, "%d\n", n);
    for (i = 0; i < N; ++i) {
      total_result += p_result[i];
      fprintf(p_file, "%.2f ", p_result[i]);
    }
    fclose(p_file);

    AcquireLock();
    printf("Total initial capital: %20.2f\n", total_capital);
    printf("Total final result:    %20.2f\n", total_result);
    ReleaseLock();

    free(p_interest);
    p_interest = NULL;
    free(p_time);
    p_time = NULL;
    free(p_capital);
    p_capital = NULL;
    free(p_result);
    p_result = NULL;

  /*   p_status = p_status00; */
  /*   p_base = (volatile float*)p_base00; */
  /*   p_exponent = (volatile float*)p_exponent00; */

  /*   /\* *p_base = 1.7; *\/ */
  /*   /\* *p_exponent = 4.1; *\/ */
  /*   /\* *p_status = 1; *\/ */
  /*   /\* AcquireLock(); *\/ */
  /*   /\* printf("procNumber: \t %zu\n", procNumber); *\/ */
  /*   /\* printf("base: \t %10.2f, \t exponent: %10.2f\n", *p_base, *p_exponent); *\/ */
  /*   /\* printf("status: \t %d\n", *p_status); *\/ */
  /*   /\* printf("result: \t %10.2f\n", *p_base); *\/ */
  /*   /\* printf("expected: \t %10.2f\n", pow(1.7, 4.1)); *\/ */

  /*   for (i = 0; i < 5000; ++i) { */
  /*     a = 1. + .01 * ((float)i / (float)5000); */
  /*     b = 36. * ((float)i / (float)5000); */
  /*     f = 50000. * ((float)i / (float)5000); */

  /*     c = f * pow(a, b); */
  /*     d += c; */
  /*     /\* AcquireLock(); *\/ */
  /*     /\* printf("%10.2f ", c); *\/ */
  /*     /\* ReleaseLock(); *\/ */

  /*     *p_base = a; */
  /*     *p_exponent = b; */
  /*     *p_status = 1; */
  /*     status = *p_status; */
  /*     c = f * (*p_base); */
  /*     e += c; */
  /*     /\* AcquireLock(); *\/ */
  /*     /\* printf("%10.2f\n", c); *\/ */
  /*     /\* ReleaseLock(); *\/ */
  /*   } */
  /*   AcquireLock(); */
  /*   printf("\n"); */
  /*   printf("d: %10.2f\n", d); */
  /*   printf("e: %10.2f\n", e); */
  /*   ReleaseLock(); */
  }
}

int main(int ac, char *av[]){
  submain();

  return 0;
}
