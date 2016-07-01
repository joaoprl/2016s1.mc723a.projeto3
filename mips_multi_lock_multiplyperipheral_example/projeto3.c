#include <stdio.h>
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

  /* AcquireLock(); */
  /* printf("procNumber address: %#10x\n", &procNumber);  */
  /* printf("procNumber %d\n ", procNumber); */
  /* ReleaseLock(); */

  uint32_t i = 0;
  float a = 0., b = 0., c = 0., d = 0., e = 0., f = 0.;

  volatile uint32_t status = 0;
  volatile uint32_t *p_status = NULL;
  volatile float *p_base = NULL, *p_exponent = NULL;

  if (procNumber == 0) {
    p_status = p_status00;
    p_base = (volatile float*)p_base00;
    p_exponent = (volatile float*)p_exponent00;

    /* *p_base = 1.7; */
    /* *p_exponent = 4.1; */
    /* *p_status = 1; */
    /* AcquireLock(); */
    /* printf("procNumber: \t %zu\n", procNumber); */
    /* printf("base: \t %10.2f, \t exponent: %10.2f\n", *p_base, *p_exponent); */
    /* printf("status: \t %d\n", *p_status); */
    /* printf("result: \t %10.2f\n", *p_base); */
    /* printf("expected: \t %10.2f\n", pow(1.7, 4.1)); */

    for (i = 0; i < 5000; ++i) {
      a = 1. + .01 * ((float)i / (float)5000);
      b = 36. * ((float)i / (float)5000);
      f = 50000. * ((float)i / (float)5000);

      c = f * pow(a, b);
      d += c;
      /* AcquireLock(); */
      /* printf("%10.2f ", c); */
      /* ReleaseLock(); */

      *p_base = a;
      *p_exponent = b;
      *p_status = 1;
      status = *p_status;
      c = f * (*p_base);
      e += c;
      /* AcquireLock(); */
      /* printf("%10.2f\n", c); */
      /* ReleaseLock(); */
    }
    AcquireLock();
    printf("\n");
    printf("d: %10.2f\n", d);
    printf("e: %10.2f\n", e);
    ReleaseLock();
  }
}

int main(int ac, char *av[]){
  submain();

  return 0;
}
