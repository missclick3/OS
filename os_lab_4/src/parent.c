#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include "unistd.h"

#include "myString.h"
#include "myVector.h"
/*
Родительский процесс создает дочерний процесс. Первой строчкой пользователь в консоль
родительского процесса пишет имя файла, которое будет передано при создании дочернего
процесса. Родительский и дочерний процесс должны быть представлены разными программами.
Родительский процесс передает команды пользователя через pipe1, который связан с
стандартным входным потоком дочернего процесса. Дочерний процесс принеобходимости
передает данные в родительский процесс через pipe2. Результаты своей работы дочерний
процесс пишет в созданный им файл. Допускается просто открыть файл и писать туда, не
перенаправляя стандартный поток вывода.
2 вариант) Пользователь вводит команды вида: «число число число<endline>». Далее эти числа
передаются от родительского процесса в дочерний. Дочерний процесс считает их сумму и
выводит её в файл. Числа имеют тип float. Количество чисел может быть произвольным.
*/

typedef enum{
	R_SUCCESS,
	R_EOL,
	R_EOF,
	R_ERROR,
} r_status;

r_status reading_int(int *cur){//чтение чисел типа int с STDIN
	char c;
	*cur = 0;
	int tmp = read(STDIN_FILENO, &c, sizeof(char));
	while(tmp > 0){
		if(c == '\n') return R_EOL;
		if(c == ' ') break;
		if((c < '0') || (c > '9')){
			return R_ERROR;
		}
		*cur = *cur * 10 + c - '0';
		tmp = read(STDIN_FILENO, &c, sizeof(char));
	}
	if(tmp == 0) return R_EOF;
	return R_SUCCESS;
}

r_status read_float(int fd, float* cur){
	bool dot_fnd = false;
	char c;
	*cur = 0;
	double i = 0.1;
	int res = read(fd, &c, sizeof(char));
	while(res > 0){
		if (c == '-') {
			i *= -1;
			res = read(fd, &c, sizeof(char));
			continue;
		}
		if(c == '\n') return R_EOL;
		if(c == ' ')
			break;
		if(((c < '0') || (c > '9')) && c != '.'){
			return R_ERROR;
		}
		if (!dot_fnd) {
			if(c == '.')
				dot_fnd = true;
			else {
				*cur = *cur * 10 + c - '0';
			}
		} else {
			if(c == '.')
				return R_ERROR;

			*cur = *cur + i * (c - '0');
			i /= 10;
		}
		res = read(fd, &c, sizeof(char));
	}
	if(res == 0)
		return R_EOF;

	return R_SUCCESS;
}

void reading_filename(string* str){
    	char cur;
    	while(read(STDIN_FILENO, &cur, sizeof(char)) > 0){
    		if(cur == '\n'){
    			break;
    		}
    		s_push(str, cur);
    	}
}

int main(){
    int fd1[2];
    int fd2[2];
    bool firstEnter = true;
    vector_dbl vec;
    string fileName;

    if (firstEnter){
        float tmp = 0;
        s_create(&fileName);
        reading_filename(&fileName);
        v_create(&vec);
        r_status st = read_float(STDIN_FILENO, &tmp);
        while(st != R_ERROR){
            v_push(&vec, tmp);
            if(st == R_EOF){
				perror("\nUSAGE: «число число число<endline>»\n");
				return -8;
			}
            else if(st == R_EOL){
                break;
            }
            tmp = 0;
			st = read_float(STDIN_FILENO, &tmp);
        }
        if (st == R_ERROR){
			perror("Wrong value\n");
			return -9;
		}
        firstEnter = false;
    }
    else{//на втором заходе в родительский процесс удаляем созданную строку и вектор чисел
		s_destroy(&fileName);
		v_destroy(&vec);
	}
	int N = v_size(&vec);
	char template[] = "/tmp/tmpXXXXXX";
    int desc = mkstemp(template);
    if(desc < 0){
    	perror("Tmp file create error!\n");
		return -3;
    }
	if(ftruncate(desc, N*sizeof(int)) < 0){
		perror("Tmp file filling error!\n");
		return -4;
	}

    if (pipe(fd1) < 0) {
        printf("Something is wrong with 1 pipe\n");
        return -1;
    }
    if (pipe(fd2) < 0) {
        printf("Something is wrong with 2 pipe\n");
        return -2;
    }
    int id = fork();
    if (id < 0) {
        printf("Something is wrong with fork\n");
        return -3;
    }
    if (id == 0) {//Child process
		printf("[%d] It's child\n\n", getpid());
		fflush(stdout);

		close(fd1[1]);
		close(fd2[0]);
		if (dup2(fd1[0], STDIN_FILENO) == -1){
			perror("Cannot dup reading channel of pipe1 to stdin\n");
			return -5;
		}
		if (dup2(fd2[1], STDOUT_FILENO)== -1){
			perror("Cannot dup recording channel of pipe2 to stdout\n");
			return -6;
		}
		//заменяет текущий процесс, процессом, описанном в исп. файле
		if(execl("child", s_get(&fileName), template,  NULL) == -1){
			perror("Execl error!");
			return -7;
		}
    }
    else {

		close(fd1[0]);
		write(fd1[1], &N, sizeof(int));
		close(fd1[1]);
		close(fd2[1]);

		int desc = open(template, O_RDWR);
		if(desc < 0){
	    	perror("Tmp file create error!\n");
			return -8;
		}

		float* fd = mmap(NULL, N*sizeof(float),
    				PROT_READ | PROT_WRITE,
    				MAP_SHARED, desc, 0);
		if (fd == MAP_FAILED){
	    	perror("Mmap error!\n");
			return -9;
	    }
		for (int i = 0; i < N; i++){
			float x = v_getInd(&vec, i);
			fd[i] = x;
		}
		if(msync(fd, N*sizeof(float), MS_SYNC) < 0){
			perror("Msync error!");
			return -10;
		}

		int status;
		wait(&status);
		int exit_status = WEXITSTATUS(status);
		fprintf(stdout,"Exit status of the child was %d\n\n", exit_status);
	   	printf("\n[%d] It's parent. Child id: %d\n\n", getpid(), id);
	   	fflush(stdout);

		unlink(template);

	    if(munmap(fd, N*sizeof(float)) < 0){
			perror("Munmap error!");
			return -12;
		}
		close(desc);
	    close(fd1[1]);
	    close(fd2[0]);
     }
	 return 0;
}
