#include <stdio.h>
#include <mqueue.h>

#define QUEUE_NAME "/my_queue"
#define PRIORITY 1
#define SIZE 256

int main(){
	mqd_t ds;
	char new_text[SIZE];
	struct mq_attr attr, old_attr;
	int prio;
	if ((ds = mq_open(QUEUE_NAME, O_RDWR | O_NONBLOCK, 0600, NULL)) == (mqd_t)-1){
		perror("Ошибка создания очереди!\n");
		return -1;
	}
	if (mq_receive(ds, new_text, SIZE, &prio) == -1){
		perror("Ошибка получения сообщения!");
		return -1;
	}
	printf("Сообщение: %s\n", new_text);

	if (mq_close(ds) == -1){
		perror("Ошибка закрытия очереди!");
	}
	if (mq_unlink(QUEUE_NAME) == -1){
		perror("Ошибка удаления очереди сообщений!");
	}
	return 0;
}
