#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "funcs.h"

void Usage(){
    printf("1. For searching pi. Input is int number of iterations\n2. For searching e. Input is int number of iterations\n");
}

int main() {
    int cmd;
    printf("Enter number the command and args: ");
    while (scanf("%d", &cmd) == 1) {
        if (cmd == 1) {
            int n;
            if (scanf("%d", &n) != 1) {
                fprintf(stderr, "Function piSearch error input\n");
                exit(2);
            }
            printf("pi: %lf\n", piSearch(n));
        }
        else if (cmd == 2) {
            int n;
            if (scanf("%d", &n) != 1) {
                fprintf(stderr, "Function eSearch error input\n");
                exit(1);
            }
            printf("e = %lf", eSearch(n));
        }
        else {
            fprintf(stderr, "Undefined command\n");
            Usage();
        }
        printf("Enter number the command and args: \n");
    }
    return 0;
}
