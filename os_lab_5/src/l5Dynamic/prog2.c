#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dlfcn.h>

Usage(){
    printf("0.Change lib.\n");
    printf("1. For searching pi.\n");
    printf("2. For searching e.\n");
}

int main() {
    double (*piSearch)(int);
    double (*eSearch)(int);
    void* lib1 = dlopen("lib1.so", RTLD_LAZY);
    if (!lib1) {
        fprintf(stderr, "Cannot open library lib.so\n");
        return -2;
    }
    void* lib2 = dlopen("lib2.so", RTLD_LAZY);
    if (!lib2) {
        fprintf(stderr, "Cannot open library lib.so\n");
        return -2;
    }
    piSearch = dlsym(lib1, "piSearch");
    eSearch = dlsym(lib1, "eSearch");
    int cmd;
    bool first_realise = false;
    Usage();
    printf("Enter number the command and args: ");
    while (scanf("%d", &cmd) == 1) {
        if (cmd == 0) {
            if (first_realise) {
                piSearch = dlsym(lib1, "piSearch");
                eSearch = dlsym(lib1, "eSearch");
            }
            else {
                piSearch = dlsym(lib2, "piSearch");
                eSearch = dlsym(lib2, "eSearch");
            }
            first_realise = !first_realise;
            if (piSearch == NULL || eSearch == NULL) {
                fprintf(stderr, "Cannot find functions\n");
                exit(2);
            }
        }
        else if (cmd == 1) {
            int n;
            if (scanf("%d", &n) != 1) {
                fprintf(stderr, "Function piSearch error input\n");
                exit(1);
            }
            printf("pi = %lf\n", piSearch(n));
        }
        else if (cmd == 2) {
            int n;
            if (scanf("%d", &n) != 1) {
                fprintf(stderr, "Function eSearch error input\n");
                exit(1);
            }
            printf("e = %lf\n", eSearch(n));
        }
        else {
            fprintf(stderr, "Undefined command\n");
            Usage();
        }
        printf("Enter number the command and args: ");
    }
    dlclose(lib1);
    dlclose(lib2);
    return 0;
}
