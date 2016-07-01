#include <stdio.h>
#include <stdint.h>
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

  volatile uint32_t *p_status = NULL;
  volatile float *p_base = NULL, *p_exponent = NULL;

  if (procNumber == 0) {
    p_status = p_status00;
    p_base = (volatile float*)p_base00;
    p_exponent = (volatile float*)p_exponent00;

    *p_base = 1.7;
    *p_exponent = 4.1;
    *p_status = 1;
    AcquireLock();
    printf("procNumber: %zu\n", procNumber);
    printf("base: %10.2f, \t exponent: %10.2f\n", *p_base, *p_exponent);
    printf("status: %d\n", *p_status);
    printf("result: %10.2f\n", *p_base);
    ReleaseLock();
  }
}

int main(int ac, char *av[]){
  submain();

  return 0;
}
