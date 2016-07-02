
#include <stdio.h>
#define START_ADRESS (200*1024*1024)

volatile int *lock = (volatile int *) START_ADRESS;

void AcquireLock(){
  while(*lock);
}

void ReleaseLock(){
  *lock = 0;
}

void submain() {
  AcquireLock();
  int a;
  scanf("%d", &a);
  printf("%d\n", a);
  ReleaseLock();

}

int main(int ac, char *av[]){
  submain();

  return 0;
}
