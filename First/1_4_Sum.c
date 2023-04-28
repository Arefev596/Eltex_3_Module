#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[]){
	char *a = argv[0];
	char *b = argv[1];
	char *result = malloc(strlen(a)+strlen(b)+1);
	memcpy(result, a, strlen(a));
	memcpy(result+strlen(a), b, strlen(b)+1);
	printf(">> Сумма слов: %s \n", result);
	return 1;
}
