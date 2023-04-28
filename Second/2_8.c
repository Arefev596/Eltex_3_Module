#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
struct msgbuf {
	long mtype;
	char mtext[10];
} msg;

int main(int argc, char *argv[])
{
	int i, msqid;
	srand(time(NULL));
	key_t key = ftok("key2_7", 1111);
	msqid = msgget(key, IPC_CREAT);
	
	while(1) {
	msg.mtype = 1;
	printf(">> ");
	fgets(msg.mtext, 10, stdin);	
	if (msgsnd(msqid, &msg, sizeof(msg.mtext), 0 ) == -1){
		printf("Ошибка отправки сообщения в очередь! \n");
	}
	if (strstr(msg.mtext, "cancel") != NULL){
		printf("Выход... \n");
		return 0;
	}

	if (msgrcv(msqid, &msg, 10, 1, 0) == -1) {
            perror("Ошибка принятия сообщения!");
            return 0;
        }
	printf("<< %s", msg.mtext);
	if (strstr(msg.mtext, "cancel") != NULL){
                printf("Выход... \n");
                return 0;
        }
	sleep(1);
	
	}
return 0;
}
