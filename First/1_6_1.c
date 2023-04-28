#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
void main(int argc, char *argv[]){
	int fd;
	char string[20];
	int filename;
	if ((filename = open(argv[1], O_RDONLY)) == -1)
	{
		printf("Ошибка открытия файла!\n");
		exit(1);
	}
	char buffer[1024];
	int rd = read(filename, buffer, strlen(buffer));
	while(rd) {
		buffer[rd] = 0;
		printf("%s", buffer);
		rd = read(filename, buffer, 1024);
	}
	close (filename);
	printf("\n");
}

