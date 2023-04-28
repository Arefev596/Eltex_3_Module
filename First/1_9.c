#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
int main(int argc, char *argv[]){
	int pipefd[2];
	int pipefdtochild[2];
	// fd[0] - read
	// fd[1] - write
	FILE *fd;
	int count = atoi(argv[1]);
	srand(time(NULL));
	if (pipe(pipefd) == -1){
		printf("Ошибка создания канала pipefd!");
		return -1;
	}
	if (pipe(pipefdtochild) == -1){
                printf("Ошибка создания канала pipefstochild!");
                return -1;
        }
	pid_t pid;
        pid = fork();
	if (pid == 0){ //child
		close(pipefd[0]);
		close(pipefdtochild[1]);
		char number[count];
		for(int i = 0; i < count; i++){
		        number[i] = rand() % 70;
		}
		if(write(pipefd[1], &number, 2*sizeof(char[count])) == -1){
			printf("Ошибка записи в канал\n");
			return 0;
		}
		close(pipefd[1]);
		wait(NULL);
		printf("Вывод дочернего процесса: ");
		read(pipefdtochild[0], &number, 2*sizeof(char[count]));
		for (int i = 0; i < count; i++){
			printf("%d ", number[i]);
		}	
		close(pipefdtochild[1]);
		printf("\n");
	}
	else {  //parent
		close (pipefd[1]);
		close (pipefdtochild[0]); // 0 - чтение    1 - запись
		char num[count];
		read(pipefd[0], &num, 2*sizeof(char[count]));
		close(pipefd[0]);
		if ((fd = fopen("1_9.txt", "a+")) == NULL){
			printf("Ошибка открытия файла!\n");
			return 2;
		}
		printf("Вывод родителя: ");
		for (int i = 0; i < count; i++){
			printf("%d ", num[i]);
			fprintf(fd, "%d ", num[i]);
		}
		fprintf(fd, "\n");
		fclose(fd);
		for (int i = 0; i < count; i++){
                        num[i] *= 2;
		}

                if(write(pipefdtochild[1], &num, 2* sizeof(char[count])) == -1){
                        printf("Ошибка записи в канал\n");
                        return 0;
                }
		close(pipefdtochild[1]);
	}
	printf("\n");
	return 100;
}
