#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

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

r_status reading_int(int *cur){
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
    else{
		s_destroy(&fileName);
		v_destroy(&vec);
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
		if (execl("child", "", NULL) == -1){
			perror("Execl child problem\n");
			return -7;
		}
    }
    else {//Parent process
        printf("\n[%d] It's parent. Child id: %d\n\n", getpid(), id);
		fflush(stdout);

		close(fd1[0]);
		close(fd2[1]);
		int len_str = s_size(&fileName);
		write(fd1[1], &len_str, sizeof(int));
        printf("Your file_name: %s\n", s_get(&fileName));
		for(int i = 0; i < len_str; i++){
			char x = s_getInd(&fileName, i);
			write(fd1[1], &x, sizeof(char));
		}
        int len = v_size(&vec);
		write(fd1[1], &len, sizeof(int));
		printf("Your numbers: ");
		for (int i = 0; i < len; i++){
			float x = v_getInd(&vec, i);
			printf("%f ", x);
			write(fd1[1], &x, sizeof(float));
		}
        printf("\n\n");

		int status;
	    waitpid(id, &status, 0);
		if (WIFEXITED(status)){
		   int exit_status = WEXITSTATUS(status);
		   printf("Exit status of the child was %d\n", exit_status);
		   if (exit_status != 0){
			   perror("Some error!\n\n");
			   return -9;
		   }
	   }
	   printf("\n[%d] It's parent. Child id: %d\n\n", getpid(), id);
	   fflush(stdout);

	   float sum;
	   int length;
	   read(fd2[0], &length, sizeof(int));
	   close(fd1[1]);
	   close(fd2[0]);
    }
	return 0;
}
