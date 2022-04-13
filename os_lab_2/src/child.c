#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "unistd.h"
#include "myVector.h"
#include "myString.h"
void write_num(int a, int fd) {
	char* num;
	if (a == 0) num = "0";
	if (a == 1) num = "1";
	if (a == 2) num = "2";
	if (a == 3) num = "3";
	if (a == 4) num = "4";
	if (a == 5) num = "5";
	if (a == 6) num = "6";
	if (a == 7) num = "7";
	if (a == 8) num = "8";
	if (a == 9) num = "9";
	if (a == -1) num = " ";
	if (a == -2) num = ".";
	write(fd, num, sizeof(char));
}

int pow_ten(int l){
	int res = 1;
	while (l > 0){
		res *= 10;
		l--;
	}
	return res;
}

int length_int(int cur){
    int tmp = cur;

    int c = 0;

    while (tmp > 0){
        tmp /= 10;
        c++;
    }
    return c;
}

void writing_float(float cur, int fd){
	int left = cur;
	float right = cur - left;
	int new_right = right * pow_ten(7);
    int l = length_int(left) - 1;
    if (left == 0){
        write_num(0, fd);
    }
    else{
    	while (left > 0){
    	    int tmp = left;
        	int c = tmp / pow_ten(l);

        	write_num(c, fd);

        	left = left % pow_ten(l);
        	l--;
    	}
    }
	write_num(-2, fd);
	l = length_int(new_right) - 1;
    if (new_right == 0){
        for (int i = 0; i < 7; i++){
            write_num(0, fd);
        }
    }
    else{
        while (new_right > 0){
            int tmp = new_right;
            int c = tmp / pow_ten(l);
            write_num(c, fd);
            new_right = new_right % pow_ten(l);
            l--;
	   }
    }
	write_num(-1, fd);
}

int main(){
    int len_str;
	read(STDIN_FILENO, &len_str, sizeof(int));

	string file_name;
	s_create(&file_name);
	for (int i = 0; i < len_str; i++){
		char x;
		read(STDIN_FILENO, &x, sizeof(char));
		s_push(&file_name, x);
	}
	s_push(&file_name, '\0');


	int file = open(s_get(&file_name), O_WRONLY);
	if(file == -1){
		s_destroy(&file_name);
		return -1;
	}
  	s_destroy(&file_name);

    float x;
    float sum = 0;
	int len;
	read(STDIN_FILENO, &len, sizeof(int));
	write(STDOUT_FILENO, &len, sizeof(int));

	//bool first = true;
	while (len > 0){
		read(STDIN_FILENO, &x, sizeof(float));
		sum += x;
        printf("%f", sum);
		write(STDOUT_FILENO, &sum, sizeof(float));
		len--;
	}
    writing_float(sum, file);
    close(file);
    return 0;
}
