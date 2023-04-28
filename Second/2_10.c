#include <stdio.h>
#include <string.h>
#include <mqueue.h>

#define QUEUE_NAME "/my_queue"
#define PRIORITY 1
#define SIZE 256

int main(){
	mqd_t ds;
	char text[32];
	fgets(text, 32, stdin);
	struct mq_attr queue_attr;
	queue_attr.mq_maxmsg = 32;
	queue_attr.mq_msgsize = SIZE;
	if ((ds = mq_open(QUEUE_NAME, O_CREAT | O_RDWR , 0600, &queue_attr)) == (mqd_t)-1){
		perror("Ошибка создания очереди!\n");
		return -1;
	}
	if (mq_send(ds, text, strlen(text), PRIORITY) == -1){
		perror("Ошибка отправки сообщения!\n");
		return -1;
	}
	if (mq_close(ds) == -1)
		perror("Соединение закрыто!\n");
	return 0;
}
