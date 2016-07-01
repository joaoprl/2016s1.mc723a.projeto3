
#include <stdio.h>

#define START_ADRESS (200*1024*1024)
#define CORES 4

typedef struct{
  int i;
}Args;

volatile int *lock = (volatile int *) START_ADRESS;
volatile int procCounter = 0;
void *funcs1[CORES];
Args coreArgs[CORES];

void AcquireLock(){
  while(*lock);
}

void ReleaseLock(){
  *lock = 0;
}

void func(Args a){
  AcquireLock();
  printf("%d\n", a.i);
  ReleaseLock();
}

void exec(int core){
  void (*f)();
  while((int)funcs1[core] != 0){
    if(funcs1[core] != 0){      
      f = funcs1[core];      
      f(coreArgs[core]);      
      funcs1[core] = NULL;
    }
  }
}

int submain(){
  AcquireLock();
  int procNumber = procCounter;
  procCounter++;
  funcs1[0] = funcs1[1] = funcs1[2] = func;
  ReleaseLock();

  if(procNumber != 0){
    exec(procNumber);
    return 0;
  }

  AcquireLock();
  printf("a\n");
  ReleaseLock();

  return 0;
}

int main(){
  return submain();
}
