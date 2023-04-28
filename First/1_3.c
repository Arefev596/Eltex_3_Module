#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void main(int argc, char *argv[]){
pid_t pid;
pid = fork();
if(pid == -1){
        perror("fork failed");
        exit(-1);
}
if(pid == 0){
        printf("Child process:\n");
        for(int i=1; i<argc; i+=3){
		int a = atoi(argv[i]);
                printf("Вычисление площади квадратов со стороной %d: %d\n", a, a*a);
        }
	exit(0);
}
else {
        printf("Parent process:\n");
        for(int i=2; i<argc; i+=3){
                int a = atoi(argv[i]);
                printf("Вычисление площади квадратов со стороной %d: %d\n", a, a*a);

        }
        exit(1);
}
}

