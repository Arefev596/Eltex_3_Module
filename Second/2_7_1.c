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
	key_t key = ftok("key2_7", 1111);
	msqid = msgget(key, IPC_CREAT);
	if (msqid == -1){
		printf("Ошибка ключа!\n");
	}
	printf("ID = %d\n", msqid);
	while(1){
		if (msgrcv(msqid, &msg, 10, 0, 0) == -1) {
            		printf("Ошибка получения сообщения!\n");
           		return 0;
       		}
  	      	if (msg.mtype == 255) {
        	    	break;
       		}
        	printf("%s", msg.mtext);
	}
	return 0;
}
