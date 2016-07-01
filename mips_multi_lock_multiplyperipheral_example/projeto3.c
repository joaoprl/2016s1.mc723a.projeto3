#include <stdio.h>
#include <stdint.h>
#define START_ADRESS (200*1024*1024)

volatile int *lock = (volatile int *) START_ADRESS;
volatile static int procCounter = 0;

volatile uint32_t *p_status01 = (volatile uint32_t*) (200 * 1024 * 1024 +  4);
volatile uint32_t *p_base01 = (volatile uint32_t*) (200 * 1024 * 1024 +  8);
volatile uint32_t *p_exponent01 = (volatile uint32_t*) (200 * 1024 * 1024 + 12);

void AcquireLock(){
  while(*lock);
}

void ReleaseLock(){
  *lock = 0;
}

void submain() {
  AcquireLock();
  volatile int procNumber = procCounter;
  ++procCounter;
  ReleaseLock();

  /* AcquireLock(); */
  /* printf("procNumber address: %#10x\n", &procNumber);  */
  /* printf("procNumber %d\n ", procNumber); */
  /* ReleaseLock(); */

  int status = 0;
  float *p_base = (float*)p_base01, *p_exponent = (float*)p_exponent01;
  float result = 0.;

  /* if (procNumber == 0) { */
  AcquireLock();
  printf("procNumber: %d\n", procNumber);
  *p_base = 1.7;
  *p_exponent = 4.1;
  printf("base: %10.2f, \t exponent: %10.2f\n", *p_base, *p_exponent);
  *p_status01 = 1;
  status = *p_status01;
  printf("status: %d\n", status);
  result = *p_base;
  printf("result: %10.2f\n", result);
  ReleaseLock();
}

int main(int ac, char *av[]){
  submain();

  return 0;
}
