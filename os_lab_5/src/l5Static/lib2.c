#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "funcs.h"

double piSearch(int n) {
    printf("This is vallis realise of pi\n");
    double mpi = 1.0;
    for (int i = 1; i <= n; i++) {
        mpi *= (4.0 * (double)i * (double)i) / (4.0 * (double)i * (double)i - 1.0);
    }
    return mpi * 2.0;
}

double eSearch(int n) {
    if (n < 0) {
        return -1;
    }
    double e = 1.0;
    double term = 1.0;
    for (int i = 1; i <= n; ++i) {
        term /= i;
        e += term;
    }
    return e;
}
