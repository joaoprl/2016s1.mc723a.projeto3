#include <stdio.h>
#define START_ADRESS (200*1024*1024)

volatile int *lock = (volatile int *) START_ADRESS;
volatile static int procCounter = 0;

volatile int *port1 = (volatile int*) (200 * 1024 * 1024 +  4);
volatile int *port2 = (volatile int*) (200 * 1024 * 1024 +  8);
volatile int *port3 = (volatile int*) (200 * 1024 * 1024 + 12);
volatile int *port4 = (volatile int*) (200 * 1024 * 1024 + 16);

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
  volatile int a = 0, b = 0;

  AcquireLock();
  printf("procNumber address: %#10x\n", &procNumber); 
  printf("procNumber %d, a %d\n ", procNumber, a);
  ReleaseLock();
  for (b = 0; b < 10; ++b) {
    a += 1;
  }
  AcquireLock();
  printf("procNumber %d, a %d\n ", procNumber, a);
  ReleaseLock();

  AcquireLock();
  *port1 = 17;
  *port2 = 18;
  *port3 = 19;
  *port4 = 20;
  printf("*port1: %d\n", *port1);
  printf("*port2: %d\n", *port2);
  printf("*port3: %d\n", *port3);
  printf("*port4: %d\n", *port4);
  ReleaseLock();
}

int main(int ac, char *av[]){
  submain();

  return 0;
}
