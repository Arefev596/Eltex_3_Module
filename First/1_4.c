#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
int main(int argc, char *argv[]){
while(1){
	printf(">>  ");
	char cmd[20];
	//char *Path = "/home/aref596/Documents/Modul_3/1_4_Sum";
	fgets(cmd, 20, stdin);
	cmd[strcspn(cmd, "\n")] = 0;
	char str[100];
	char str2[100];
	printf(">> ");
	fgets(str, 100, stdin);
	str[strcspn(str, "\n")] = 0;
	printf(">> ");
	fgets(str2, 100, stdin);
	str2[strcspn(str2, "\n")] = 0;
	pid_t pid = fork();
	if (pid == 0){
		//printf(">> Вызов дочернего процесса\n");
		execl(cmd, str, str2,  NULL);
		printf("Error\n");
		sleep(3);
	}
	else {
		wait(0);
	}
}
return 0;
}
