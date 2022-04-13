#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "myString.h"

int main(int argc, char *argv[]) {
    int pa2c = atoi(argv[1]);
    int pc2a = atoi(argv[2]);
    int pc2b = atoi(argv[3]);

    int size;

    while (read(pa2c, &size, sizeof(int))) {
        char* str = (char*)malloc(size);
        if (str == NULL){
            printf("NO MEMORY\n");
        }
        read(pa2c, str, size);
        printf("(C) Delivered from A: %s\n", str);
        write(pc2b, &size, sizeof(int));
        int ok = 1;
        write(pc2a, &ok, sizeof(int));
        free(str);
    }

    close(pa2c);
    close(pc2a);
    close(pc2b);

    return 0;
}
