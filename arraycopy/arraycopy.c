#include <stdlib.h>
#include <stdio.h>
#include "fcyc.h"
#include "clock.h"

#define MAXELEMS  2048
#define SLEEPTIME 4  // time to sleep when estimating the clock frequenc

int src[MAXELEMS][MAXELEMS],dst[MAXELEMS][MAXELEMS];
void init_data(int data[][MAXELEMS], int n);
void copyij(int src[][MAXELEMS], int dst[][MAXELEMS]);
void copyji(int src[][MAXELEMS], int dst[][MAXELEMS]);
double run(test_funct f, double Mhz);

int main()
{
  double Mhz;

  init_data(src, MAXELEMS);
  init_data(dst, MAXELEMS);
  Mhz = mhz(SLEEPTIME);

  printf("Clock frequency is approx %.1f MHz.\n", Mhz);
  printf("Copy (sec)\n");
  printf("%.1f\t", run(copyij, Mhz));
  printf("\n");
  printf("%.1f\t", run(copyji, Mhz));
  printf("\n");

  exit(0);
}

void init_data(int data[][MAXELEMS], int n)
{
  int i, j;
  for (i = 0; i < n; i++)
    for(j=0; j < n; j++)
      data[i][j] = 1;
}

void copyij(int src[][MAXELEMS], int dst[][MAXELEMS])
{
  int i, j;
  for (i = 0; i < MAXELEMS; i++) for(j=0; j<MAXELEMS; j++)
    dst[i][j] = src[i][j];
}

void copyji(int src[][MAXELEMS], int dst[][MAXELEMS])
{
  int i, j;
  for (j = 0; j < MAXELEMS; j++) for(i = 0; i<MAXELEMS; i++)
    dst[i][j] = src[i][j];
}

double run(test_funct f , double Mhz)
{
  double cycles;
  f(src, dst); // warms up the cache
  cycles = fcyc(f, src, dst);
  return (cycles / Mhz); // sec
}
