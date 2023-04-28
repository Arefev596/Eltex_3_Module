#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
int main(){
	FILE *fd;
	char string[20];
	if ((fd = fopen("1_7text.txt", "a+")) == NULL)
	{
		printf("Ошибка открытия файла!\n");
		return 0;
	}
	printf("Введите строку для добавления: ");
	fgets(string, 20, stdin);
	string[strcspn(string, "\n")] = 0;
	fputs(string, fd);
	fclose(fd);
	printf("Запись была произведена!\n");
	return 0;
}
