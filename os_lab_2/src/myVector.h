#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int size;
    int cap;
    double* buf;
} vector_dbl;

void v_create(vector_dbl *v);
void v_destroy(vector_dbl *v);
bool v_isEmpty(vector_dbl *v);
int v_cap(vector_dbl *v);
int v_size(vector_dbl *v);
bool v_growBuf(vector_dbl *v);
bool v_push(vector_dbl *v, double val);
bool v_shrinkBuf(vector_dbl *v);
double v_pop(vector_dbl *v);
double v_getInd(vector_dbl *v   , int id);
double* v_get(vector_dbl *s);

#endif
