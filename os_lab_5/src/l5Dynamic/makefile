CC=gcc

all: prog2

prog2: prog2.o lib2.so lib1.so
	$(CC) prog2.o -Wl,-rpath,. -ldl -lm -o prog2

lib1.so: lib1.o
	$(CC) -shared -o lib1.so lib1.o -lm
lib2.so: lib2.o
	$(CC) -shared -o lib2.so lib2.o -lm

prog2.o: prog2.c
	$(CC) -c prog2.c -lm

lib1.o: lib1.c
	$(CC) -fPIC -c lib1.c -lm

lib2.o: lib2.c
	$(CC) -fPIC -c lib2.c -lm
