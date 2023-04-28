#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
int main(){
	int fd;
	char  string[20];
	if ((fd = open("1_6text.txt", O_WRONLY | O_APPEND | O_CREAT)) == -1)
	{
		printf("Ошибка открытия файла!\n");
		return 0;
	}
	printf("Введите строку для добавления: ");
	fgets(string, 20, stdin);
	string[strcspn(string, "\n")] = 0;
	write(fd, &string, strlen(string));
	printf("Запись была произведена!\n");
	return 0;
}
