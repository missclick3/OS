#include <stdio.h>
#include "myVector.h"
void v_create(vector_dbl *v){
    v->buf = NULL;
    v->size = 0;
    v->cap = 0;
}

void v_destroy(vector_dbl *v){
    v->size = 0;
    v->cap = 0;
    free(v->buf);
}

bool v_isEmpty(vector_dbl *v){
    return v->size == 0;
}

int v_cap(vector_dbl *v){
    return v->cap;
}

int v_size(vector_dbl *v){
    return v->size;
}

bool v_growBuf(vector_dbl *v){
    int tmp = v->cap * 3/2;
    if (!tmp){
        tmp = 10;
    }
    double *newBuf = realloc(v->buf, sizeof(double) * tmp);
    if (newBuf != NULL){
        v->buf = newBuf;
        v->cap = tmp;
        return true;
    }
    return false;
}

bool v_push(vector_dbl *v, double val){
    if (v_cap(v) == v_size(v)){
        if (!v_growBuf(v)){
            return false;
        }
    }
    v->buf[v_size(v)] = val;
    v->size++;
    return true;
}

bool v_shrinkBuf(vector_dbl *v){
    int tmp = v->cap * 4/9;
    if (tmp < v_size(v)){
        return true;
    }
    double *newBuf = realloc(v->buf, sizeof(double) * tmp);
    if (newBuf != NULL){
        v->buf = newBuf;
        v->cap = tmp;
        return true;
    }
    return false;
}

double v_pop(vector_dbl *v){
    double tmp = v->buf[v_size(v) - 1];
    v_shrinkBuf(v);
    v->size--;
    return tmp;
}

double v_getInd(vector_dbl *v, int id){
    return v->buf[id];
}

double* v_get(vector_dbl *v){
    return v->buf;
}
