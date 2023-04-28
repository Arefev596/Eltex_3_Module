#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int lock = 0;
void usr1(int sig){
	lock = 1;
}

void usr2(int sig){
	lock = 0;
}

union semun {
	int val; /* значение для SETVAL */
	struct semid_ds *buf; /* буферы для IPC_STAT, IPC_SET */
	unsigned short *array; /* массивы для GETALL, SETALL */
	/* часть, особенная для Linux: */
	struct seminfo *__buf; /* буфер для IPC_INFO */
};

int main(int argc, char *argv[]){
	////
	key_t key = ftok("/", 1234);
	int semid = semget(key, 1, 0666| IPC_CREAT);
	union semun arg;
	struct sembuf lock_res = {0, -1, SEM_UNDO};
	struct sembuf rel_res = {0, 1, SEM_UNDO};
	if (semid == -1){
		perror("Ошибка создания семафора");
		exit(0);
	}
	////
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
		char number[count];
		for(int i = 0; i < count; i++){
		        number[i] = rand() % 100;
			//printf("%d ", number[i]);	
		}
		if(write(pipefd[1], &number, sizeof(char[count])) == -1){
			printf("Ошибка записи в канал в child процессе 1\n");
			return 0;
		}
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
		
		////
		arg.val = 1;
		if (semctl(semid, 0, SETVAL, arg) == -1){
			perror("Ошибка установки значения семафора в child процессе");
			exit(0);
		}
		if (semop(semid, &lock_res, 1) == -1){
        	        perror("Ошибка блокирования ресурса в child процессе");
       		} 
		////
		if ((fd = fopen("3_2.txt", "a+")) == NULL){
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
		/*if (semop(semid, &rel_res, 1) == -1){
                        perror("Ошибка разблокирования ресурса в child процессе");
        	}*/	

	}
	else {  //parent
		close (pipefd[1]);
		char num[count];
		signal(SIGUSR1, usr1);
                signal(SIGUSR2, usr2);
		if (read(pipefd[0], &num, sizeof(char[count])) > 0){
			close(pipefd[0]);
		}
		////
		if (semop(semid, &lock_res, 1) == -1){
			perror("Ошибка блокирования ресурса в parent процессе");
        	}
		////
		if ((fd = fopen("3_2.txt", "a+")) == NULL){
			printf("Ошибка открытия файла!\n");
			return 2;
		}
		kill(getpid(), SIGUSR1);
		printf("Вывод parent процесса.Файл был изменен\n");
		fprintf(fd, "Изменение файла...\n");
		kill(getpid(), SIGUSR2);
		fclose(fd);
		////
		if (semop(semid, &rel_res, 1) == -1){
			perror("Ошибка разблокирования ресурса в parent процессе");
        	}
		if (semctl(semid, 0, IPC_RMID) == -1){
                	perror("Ошибка удаления семафора");
        	}
		////
	}
	printf("\n");
	return 1;
}
