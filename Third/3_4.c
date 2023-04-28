#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>

int main (int argc, char *argv[]){
	sem_t *sem = sem_open("/sem3_4", O_CREAT, 0777, 0);
	if (sem == SEM_FAILED){
		perror("Ошибка создания семафора");
		exit(0);
	}
	////
	srand(time(NULL));
	if (mkfifo("myfifo1", 777) == -1) {
		printf("Ошибка создания именованного канала!\n");
		return -1;
	}
	///
	sem_post(sem);
	///
	int fd = open("myfifo1", O_WRONLY);
	int number;
	for (int i = 0; i < 2; i++){
		number = rand() % 100;
		write(fd, &number, sizeof(number));
	}
	sem_wait(sem);
	close(fd);
	sem_close(sem);
	sem_unlink("/sem3_4");
	return 0;
}
