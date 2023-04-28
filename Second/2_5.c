#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

void listener(int sig){
	signal (SIGINT, SIG_IGN);
	if (sig == SIGINT){
                printf("Был вызван сигнал SINGINT -2\n");
        }
	signal (SIGQUIT, SIG_IGN);
	if (sig == SIGQUIT){
		printf("Был вызван сигнал SIGQUIT -3\n");
	}
}

int main(){
	FILE *fd;
	char filename[] = "file1.txt";
	for (int i = 1; i <=8; i++){
		filename[4] = i+'0';
		if ((fd = fopen(filename, "a+")) == NULL){
			printf("Ошибка открытия файла для записи!\n");
			return 0;
		}
		int j = 1;
		signal (SIGINT, listener);
		signal (SIGQUIT, listener);
		printf("Pid процесса: %d \n", getpid());
		while (i != 100){
			if ((fd = fopen(filename, "a+")) == NULL){
                      	 	 printf("Ошибка открытия файла для записи!\n");
                	        return 0;
        	        }
			signal (SIGQUIT, listener);
			signal (SIGINT, listener);
	               	fprintf(fd, "Процесс %d, счетчик: %d \n", i, j);
			fclose(fd);
			printf("Процесс %d, счетчик: %d \n", i, j);
			j++;
			sleep(1);
		}
	}
	return 1;
}
