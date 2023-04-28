#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <time.h>
struct msgbuf {
	long mtype;
	char mtext[10];
} msg;

int main(int argc, char *argv[])
{
	int i, msqid;
	srand(time(NULL));
	key_t key = ftok("key2_7", 1111);
	msg.mtype = 1;
	msqid = msgget(key, IPC_CREAT);
        printf("ID = %d\n", msqid);
	char temp[10];
	for (int i = 0; i< 10; i++){
		temp[i] = rand()%100;
	}
	strncpy(msg.mtext, temp, 10);	
	if (msgsnd(msqid, &msg, sizeof(msg.mtext), 0 ) == -1){
		printf("Ошибка отправки сообщения в очередь! \n");
	}
	printf("Произошла отправка сообщения!\n");
	msg.mtype = 255;
	return 0;
}
