#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
int main(int argc, char *argv[]){
	int pipefd[2];
	FILE *fd;
	int count = atoi(argv[1]);
	srand(time(NULL));
	if (pipe(pipefd) == -1){
		printf("Ошибка создания канала!");
		return -1;
	}
	pid_t pid;
        pid = fork();
	if (pid == 0){ //child
		close(pipefd[0]);
		char number[count];
		for(int i = 0; i < count; i++){
		        number[i] = rand() % 100;
			//printf("%d ", number[i]);	
		}
		if(write(pipefd[1], &number, sizeof(char[count])) == -1){
			printf("Ошибка записи в канал\n");
			return 0;
		}
		close(pipefd[1]);
	}
	else {  //parent
		close (pipefd[1]);
		char num[count];
		read(pipefd[0], &num, sizeof(char[count]));
		close(pipefd[0]);
		if ((fd = fopen("1_8.txt", "a+")) == NULL){
			printf("Оибка открытия файла!\n");
			return 2;
		}
		for (int i = 0; i < count; i++){
			printf("%d ", num[i]);
			fprintf(fd, "%d ", num[i]);
			//fputs(num[int], fd);
		}
		fprintf(fd, "\n");
		fclose(fd);
	}
	printf("\n");
	return 100;
}
