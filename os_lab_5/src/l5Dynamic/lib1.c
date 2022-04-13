#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

double piSearch(int n) {
    printf("This is pi_leibniz realise of pi\n");
    double mpi = 0;
    double zn = 1.0;
    for (int i = 0; i < n; zn += 2.0, i++) {
        if (i % 2) {
            mpi -= 1.0 / zn;
        }
        else {
            mpi += 1.0 / zn;
        }
    }
    return mpi * 4.0;
}

double eSearch (int n) {
    if (n < 0) {
        return -1;
    }
    double e = 1.0;
    for (int i = 0; i < n; ++i) {
        e *= 1 + 1/(double)n;
    }
    return e;
}
