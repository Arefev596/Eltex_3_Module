#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
int main (int argc, char *argv[]){
	srand(time(NULL));
	if (mkfifo("myfifo1", 777) == -1) {
		printf("Ошибка создания именованного канала!\n");
		return -1;
	}
	int fd = open("myfifo1", O_WRONLY);
	int number;
	for (int i = 0; i < 2; i++){
		number = rand() % 100;
		write(fd, &number, sizeof(number));
	}
	close(fd);
	return 0;
}
