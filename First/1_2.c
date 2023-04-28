#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void exit_call(){
    printf("All right. This is exit_call\n");
}

int main(int argc, char *argv[]){

atexit(exit_call);
pid_t pid;
pid = fork();
if(pid == -1){
	perror("fork failed");
	exit(-1);
}
if(pid == 0){
    	printf("Child process. Arguments:\n");
    	for(int i=0; i<argc; i++){
        	printf("%s\n", argv[i]);
    	}
	exit(0);
}
else {
	printf("Parent process. Arguments:\n");
	for(int i=0; i<argc; i++){
		printf("%s\n", argv[i]);
	}
	exit(1);
}
}
