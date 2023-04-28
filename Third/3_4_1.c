#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>
int main (int argc, char *argv[]){
	sem_t *sem = sem_open ("/sem3_4", O_CREAT, 0777, 0);
	if (sem == SEM_FAILED){
		perror("Ошибка создания семафора");
		exit(0);
	}
	srand(time(NULL));	
	sem_post(sem);
	int fd = open("myfifo1", O_RDONLY);
	int number;
	for (int i = 0; i < 2; i++) {
		read(fd, &number, sizeof(number));
		printf("%d ", number);
	}
	sem_wait(sem);
	printf("\n");
	close(fd);
	sem_close(sem);
	sem_unlink("/sem3_4");
	return 0;
}
