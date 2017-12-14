#include <stdlib.h>
#include <stdio.h>
#include "fcyc.h"
#include "clock.h"

#define MINBYTES  (1 << 10)  // Working set size ranges from 1 KB
#define MAXBYTES  (1 << 27)  // ... up to 128 MB
#define MAXSTRIDE 32         // Strides range from 1 to 32
#define MAXELEMS  MAXBYTES/sizeof(int)
#define SLEEPTIME 4         // time to sleep when estimating the clock frequenc

int data[MAXELEMS];
void init_data(int *data, int n);
void test(int elems, int stride);
double run(int size, int stride, double Mhz);

int main()
{
    int size;
    int stride;
    double Mhz;

    init_data(data, MAXELEMS);
    Mhz = mhz(SLEEPTIME);

    printf("Clock frequency is approx %.1f MHz.\n", Mhz);
    printf("Memory mountain (MB/sec)\n");
    printf("\t");
    for (stride = 1; stride <= MAXSTRIDE; stride++) printf("s%d\t", stride);
    printf("\n");

    for (size = MAXBYTES; size >= MINBYTES; size >>= 1) {

        if (size > (1 << 20)) printf("%dm\t", size / (1 << 20));
        else printf("%dk\t", size / 1024);

        for (stride = 1; stride <= MAXSTRIDE; stride++) {
            printf("%.1f\t", run(size, stride, Mhz));
        }
        printf("\n");
    }
    exit(0);
}

void init_data(int *data, int n)
{
    int i;
    for (i = 0; i < n; i++) data[i] = 1;
}

void test(int elems, int stride)
{
    int i, result = 0;
    volatile int sink;
    for (i = 0; i < elems; i += stride)
      result += data[i];
    sink = result;
}

double run(int size, int stride, double Mhz)
{
    double cycles;
    int elems = size / sizeof(int);
    test(elems, stride); // warms up the cache
    cycles = fcyc(test, elems, stride);
    return (size / stride) / (cycles / Mhz); // MB/s
}
