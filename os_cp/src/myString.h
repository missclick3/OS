#ifndef MYSTRING_H
#define MYSTRING_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int size;
    int cap;
    char* buf;
} string;

void s_create(string *s);
void s_destroy(string *s);
bool s_isEmpty(string *s);
int s_cap(string *s);
int s_size(string *s);
bool s_growBuf(string *s);
bool s_push(string *s, char c);
bool s_shrinkBuf(string *s);
char s_pop(string *s);
char s_getInd(string *s, int id);
char* s_get(string *s);
void s_print(string *s);
#endif
