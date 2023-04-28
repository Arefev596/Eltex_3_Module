#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
int main (int argc, char *argv[]){
	srand(time(NULL));
	
	int fd = open("myfifo1", O_RDONLY);
	int number;
	for (int i = 0; i < 2; i++) {
		read(fd, &number, sizeof(number));
		printf("%d ", number);
	}
	printf("\n");
	close(fd);
	return 0;
}
