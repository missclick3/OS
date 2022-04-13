#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    int pc2b = atoi(argv[1]);
    int pa2b = atoi(argv[2]);

    int size;

    while (read(pa2b, &size, sizeof(int))) {
        printf("(B) Recieved from A: %d\n", size);
        read(pc2b, &size, sizeof(int));
        printf("(B) Recieved from A: %d\n", size);
    }

    close(pc2b);
    close(pa2b);

    return 0;
}
