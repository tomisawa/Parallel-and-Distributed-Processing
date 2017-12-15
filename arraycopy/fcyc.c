#include <stdlib.h>
#include <sys/times.h>
#include <stdio.h>
#include "clock.h"
#include "fcyc.h"

#define KBEST 3
#define MAXSAMPLES 20
#define EPSILON 0.01

static double *values = NULL;
static int samplecount = 0;

static void init_sampler()
{
	if (values) free(values);
	values = calloc(KBEST, sizeof(double));
	samplecount = 0;
}

static void add_sample(double val)
{
	int pos = 0;
	if (samplecount < KBEST) {
		pos = samplecount;
		values[pos] = val;
	} else if (val < values[KBEST-1]) {
		pos = KBEST-1;
		values[pos] = val;
	}

	samplecount++;
	while (pos > 0 && values[pos-1] > values[pos]) {
		double temp = values[pos-1];
		values[pos-1] = values[pos];
		values[pos] = temp;
		pos--;
	}
}

static int has_converged()
{
	return
		(samplecount >= KBEST) &&
		((1 + EPSILON)*values[0] >= values[KBEST-1]);
}

double fcyc(test_funct f, int **arg1, int **arg2 )
{
	init_sampler();

	do {
		double cyc;
		start_counter();
		f(arg1, arg2);
		cyc = get_counter();
        printf("cyc=%f\t",cyc);
		add_sample(cyc);
	} while (!has_converged() && samplecount < MAXSAMPLES);
    printf("\n");
    return values[0];
}
