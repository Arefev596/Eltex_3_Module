#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
void main(int argc, char *argv[]){
	FILE *fd;
	char string[20];
	if ((fd = fopen(argv[1], "r")) == NULL)
	{
		printf("Ошибка открытия файла!\n");
		exit(1);
	}
	char buffer[1024];	
	while(fgets(buffer, 1024, fd)) {
		printf("%s", buffer);
	}
	fclose (fd);
	printf("\n");
}

