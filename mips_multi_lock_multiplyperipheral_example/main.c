#include <stdio.h>
#define START_ADRESS (200*1024*1024)

volatile int *lock = (volatile int *) START_ADRESS;
volatile static int procCounter = 0;

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
}

int main(int ac, char *av[]){
  submain();

  return 0;
}
