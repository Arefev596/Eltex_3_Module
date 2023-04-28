#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
	int val; /* значение для SETVAL */
	struct semid_ds *buf; /* буферы для IPC_STAT, IPC_SET */
	unsigned short *array; /* массивы для GETALL, SETALL */
	/* часть, особенная для Linux: */
	struct seminfo *__buf; /* буфер для IPC_INFO */
};

int main (int argc, char *argv[]){
	if (mkfifo("myfifo1", 777) == -1) {
                perror("Ошибка создания именованного канала");
                exit(0);
        }
	int fd = open("myfifo1", O_WRONLY);
	int number;
	srand(time(NULL));

	key_t key = ftok(".", 1234);
	int semid = semget(key, 1, 0666| IPC_CREAT);
	union semun arg;
	struct sembuf lock_res = {0, -1, SEM_UNDO};
	struct sembuf rel_res = {0, 1, SEM_UNDO};
	if (semid == -1){
		perror("Ошибка создания семафора");
		exit(0);
	}
	arg.val = 1;
	if (semctl(semid, 0, SETVAL, arg) == -1){
		perror("Ошибка установки значения семафора");
		exit(0);
	}
	if (semop(semid, &lock_res, 1) == -1){
                perror("Ошибка блокирования ресурса");
        }
		
	for (int i = 0; i < 2; i++){
		number = rand() % 100;
		write(fd, &number, sizeof(number));
	}
	
	if (semop(semid, &rel_res, 1) == -1){
                        perror("Ошибка разблокирования ресурса");
        }

	close(fd);	
	return 0;
}
