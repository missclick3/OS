#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#include "myString.h"

#define MIN_CAP 10

// int reading_string(string* str){
//         s_create(str);
//     	char cur;
//     	while(read(STDIN_FILENO, &cur, sizeof(char)) > 0){
//     		if(cur == '\n'){
//     			break;
//     		}
//     		s_push(str, cur);
//     	}
//         return s_size(str);
// }
int read_string(char **str_, int fd) {
    free(*str_);
    int str_size = 0;
    int cap = MIN_CAP;
    char *str = (char*) malloc(sizeof(char) * cap);
    if (str == NULL) {
        perror("Malloc error");
        exit(-1);
    }
    char c;
    while (read(fd, &c, sizeof(char)) == 1) {
        if (c == '\n') {
            break;
        }
        str[(str_size)++] = c;
        if (str_size == cap) {
            cap = cap * 3 / 2;
            str = (char*) realloc(str, sizeof(char) * cap);
            if (str == NULL) {
                perror("Realloc error");
                exit(-2);
            }
        }
    }
    str[str_size] = '\0';

    *str_ = str;
    return str_size;
}
int str_length(char *str) {
    int length = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        ++length;
    }
    return length;
}

int main(){
    int a2b[2];
    int a2c[2];
    int c2a[2];
    int c2b[2];
    if (pipe(a2b) == -1) {
        perror("Cannot create pipe\n");
        return -1;
    }if (pipe(a2c) == -1) {
        perror("Cannot create pipe\n");
        return -2;
    }if (pipe(c2a) == -1) {
        perror("Cannot create pipe\n");
        return -3;
    }if (pipe(c2b) == -1) {
        perror("Cannot create pipe\n");
        return -4;
    }
    int pid1 = fork();
    if (pid1 < 0){
        perror("Problem with fork\n");
        return -5;
    }
    else if(pid1 == 0){
        close(a2c[1]);
        close(c2a[0]);
        close(c2b[0]);
        close(a2b[0]);
        close(a2b[1]);
        char pa2c[3];
        char pc2a[3];
        char pc2b[3];
        sprintf(pa2c, "%d", a2c[0]);
        sprintf(pc2a, "%d", c2a[1]);
        sprintf(pc2b, "%d", c2b[1]);
        execl("C_prog", "C_prog", pa2c, pc2a, pc2b, NULL);
    }
    else {
        int pid2 = fork();
        if (pid2 < 0){
            perror("Problem with fork\n");
            return -6;
        }
        else if (pid2 == 0){
            close(a2c[0]);
            close(a2c[1]);
            close(c2a[0]);
            close(c2a[1]);
            close(c2b[1]);
            close(a2b[1]);
            char pc2b[2];
            char pa2b[2];
            sprintf(pc2b, "%d", c2a[0]);
            sprintf(pa2b, "%d", c2b[0]);
            execl("B_prog", "B_prog",  pc2b, pa2b, NULL);
        }
        else {
            close(a2c[0]);
            close(c2a[1]);
            close(a2b[0]);
            close(c2b[1]);
            close(c2b[0]);
            // string str;
            // while(reading_string(&str)){
            //     int size = s_size(&str);
            //     write(a2c[1], &size, sizeof(int));
            //     write(a2c[1], &str, size);
            //     write(a2b[1], &size, sizeof(int));
            //     int ok;
            //     read(c2a[0], &ok, sizeof(ok));
            // }
            char *str = NULL;
            while ((read_string(&str, 0))) {
                int size = str_length(str);
                write(a2c[1], &size, sizeof(int));
                write(a2c[1], str, size);
                write(a2b[1], &size, sizeof(int));
                int ok;
                read(c2a[0], &ok, sizeof(ok));
            }
            close(c2a[0]);
            close(a2c[1]);
            close(a2b[1]);
        }
    }
    return 0;
}
