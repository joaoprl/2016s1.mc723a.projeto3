
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define START_ADRESS (200*1024*1024)
#define INITIAL_VALUE 3.1415
#define CORES 4

volatile int *lock = (volatile int *) START_ADRESS;
volatile static int procCounter = 0;

typedef struct{
  double x;
  int begin;
  int end;
  double *coef;
  double *exp;
  double result;
}Args;

volatile void *funcs1[CORES];
volatile Args coreArgs[CORES];

void AcquireLock(){
  while(*lock);
}

void ReleaseLock(){
  *lock = 0;
}

void derivateTerm(double coef, double exp, double *coefDest, double *expDest){
  *coefDest = coef * exp;
  *expDest = exp - 1;
}

void derivatePol(int size, double *coef, double *exp, double *dCoef, double *dExp){
  int i;

  for(i = 0; i < size; i++){
    derivateTerm(coef[i], exp[i], &(dCoef[i]), &(dExp[i]));
  }
}

void parSolvePol(Args a){
  int i;
  a.result = 0;

  for(i = a.begin; i < a.end; i++){
    a.result += a.coef[i] * pow(a.x, a.exp[i]);
  }
}

double solvePol(double x, int size, double *coef, double *exp){
  coreArgs[1].x = x;
  coreArgs[1].begin = 0;
  coreArgs[1].end = size;
  coreArgs[1].coef = coef;
  coreArgs[1].exp = exp;
  funcs1[1] = parSolvePol;

  while(funcs1[1] != NULL);

  return coreArgs[1].result;  
}

double getNextNewton(double x, int size, double *coef, double *exp, double *dCoef, double *dExp){
  double f, dF;
  f = solvePol(x, size, coef, exp);
  dF = solvePol(x, size, dCoef, dExp);

  if(dF != 0)
    return x - f/dF ;
  return x;
}

double getMin(int size, double *coef, double *exp){

  double *dCoef, *dExp;
  dCoef = malloc(sizeof(double) * size);
  dExp = malloc(sizeof(double) * size);

  derivatePol(size, coef, exp, dCoef, dExp);

  double x = INITIAL_VALUE; // intial value
  double oldX = x + 1;
  
  while(x != oldX){
    oldX = x;
    x = getNextNewton(x, size, coef, exp, dCoef, dExp);
  }

  free(dCoef);
  free(dExp);
  
  return x;
}

void exec(int core){
  void (*f)();
  while((int)funcs1[core] != -1){
    if(funcs1[core] != NULL){
      AcquireLock();
      printf("c%d\n", core);
      ReleaseLock();

      f = funcs1[core];      
      f(coreArgs[core]);      
      funcs1[core] = NULL;
    }
  }
}

void submain() {
  AcquireLock();
  volatile int procNumber = procCounter;
  ++procCounter;
  funcs1[0] = funcs1[1] = funcs1[2] = NULL;
  ReleaseLock();
  
  
  if(procNumber > 0){
    AcquireLock();
    printf("a%d\n", procNumber);
    ReleaseLock();

    exec(procNumber);
    return;
  }
  
  AcquireLock(); 
  printf("b%d\n", procNumber); 
  ReleaseLock();

  int size;
  scanf("%d", &size);

  double *coef = malloc(sizeof(double) * size);
  double *exp = malloc(sizeof(double) * size);

  int i;
  for(i = 0; i < size; i++){
    scanf("%lf", &(coef[i]));
  }
  for(i = 0; i < size; i++){
    scanf("%lf", &(exp[i]));
  }
  double result;
  result = getMin(size, coef, exp);
  printf("Result: %.30lf\n", result);

  free(coef);
  free(exp);
}

int main(int ac, char *av[]){
  submain();

  return 0;
}
