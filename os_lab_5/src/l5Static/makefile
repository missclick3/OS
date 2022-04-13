CC=gcc

all: a.out

a.out: prog1.c lib1.so
	$(CC) prog1.c -L. -Wl,-R. -l1 -o a.out

lib2.so: lib2.c
	$(CC) -shared lib2.c -o lib2.so  -lm

lib1.so: lib1.c
	$(CC) -shared lib1.c -o lib1.so  -lm
