#include <stdio.h>
#include "myString.h"

void s_create(string *s){
    s->buf = NULL;
    s->cap = 0;
    s->size = 0;
}

void s_destroy(string *s){
    s->size = 0;
    s->cap = 0;
    free(s->buf);
}

bool s_isEmpty(string *s){
    return s->size == 0;
}

int s_cap(string *s){
    return s->cap;
}

int s_size(string *s){
    return s->size;
}

bool s_growBuf(string *s){
    int tmp = s->cap * 3/2;
    if (!tmp){
        tmp = 10;
    }
    char *newBuf = realloc(s->buf, sizeof(char) * tmp);
    if (newBuf != NULL){
        s->buf = newBuf;
        s->cap = tmp;
        return true;
    }
    return false;
}

bool s_push(string *s, char c){
    if (s_size(s) == s_cap(s)){
        if (!s_growBuf(s)){
            return false;
        }
    }
    s->buf[s_size(s)] = c;
    s->size++;
    return true;
}

bool s_shrinkBuf(string *s){
    int tmp = s->cap * 4/9;
    if (tmp < s_size(s)){
        return true;
    }
    char *newBuf = realloc(s->buf, sizeof(char) * tmp);
    if (newBuf != NULL){
        s->buf = newBuf;
        s->cap = tmp;
        return true;
    }
    return false;
}

char s_pop(string *s){
    char tmp = s->buf[s_size(s) - 1];
    s_shrinkBuf(s);
    s->size--;
    return tmp;
}

char s_getInd(string *s, int id){
    return s->buf[id];
}

char* s_get(string *s){
    return s->buf;
}

void s_print(string *s){
    for (int i = 0; i < s_size(s); i++){
        printf("%c", s_getInd(s, i));
    }
}
