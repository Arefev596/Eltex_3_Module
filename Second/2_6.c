#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
int lock = 0;
void usr1(int sig){
	lock = 1;
}

void usr2(int sig){
	lock = 0;
}


int main(int argc, char *argv[]){
	int pipefd[2];
	FILE *fd;
	int count = atoi(argv[1]);
	srand(time(NULL));
	if (pipe(pipefd) == -1){
		printf("Ошибка создания канала!");
		return -1;
	}
	signal(SIGUSR1, usr1);
        signal(SIGUSR2, usr2);
	pid_t pid;
        pid = fork();
	if (pid == 0){ //child
		//close(pipefd[0]);
		char number[count];
		for(int i = 0; i < count; i++){
		        number[i] = rand() % 100;
			//printf("%d ", number[i]);	
		}
		if(write(pipefd[1], &number, sizeof(char[count])) == -1){
			printf("Ошибка записи в канал в child процессе 1\n");
			return 0;
		}
		//close(pipefd[1]);

		if(lock == 1){
			pause();
		}
		char numtemp[count];
		read(pipefd[0], &numtemp, sizeof(char[count]));
		int numb = 66;
		if(write(pipefd[1], &numb, sizeof(int)) == -1){
                        printf("Ошибка записи в канал в child процессе 2\n");
                        return 0;
                }
		printf("Была произведена отправка нового числа: %d\n", numb);
		
                close(pipefd[1]);
		close(pipefd[0]);
		
		if (read(pipefd[0], &numb, sizeof(int)) > 0){
                        close(pipefd[0]);
                }
		if ((fd = fopen("2_6.txt", "a+")) == NULL){
                        printf("Ошибка открытия файла!\n");
                        return 2;
                }
		printf("Вывод child процесса!\n");
		for (int i = 0; i < count; i++){
                        printf("%d ", numtemp[i]);
                        fprintf(fd, "%d ", numtemp[i]);
                }
		fprintf(fd, "%d ", numb);
                fprintf(fd, "\n");
                fclose(fd);

	}
	else {  //parent
		close (pipefd[1]);
		char num[count];
		signal(SIGUSR1, usr1);
                signal(SIGUSR2, usr2);
		if (read(pipefd[0], &num, sizeof(char[count])) > 0){
			close(pipefd[0]);
		}
		if ((fd = fopen("2_6.txt", "a+")) == NULL){
			printf("Ошибка открытия файла!\n");
			return 2;
		}
		kill(getpid(), SIGUSR1);
		printf("Вывод parent процесса.Файл был изменен\n");
		fprintf(fd, "Изменение файла... ");
		kill(getpid(), SIGUSR2);
		fclose(fd);
		
	}
	printf("\n");
	return 100;
}
