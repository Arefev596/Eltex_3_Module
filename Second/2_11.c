#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <unistd.h>

#define QUEUE_NAME "/my_queue"
#define QUEUE_NAME_2 "/my_queue_2"
#define PRIORITY 1
#define SIZE 256

int main(){
	mqd_t ds, ds2;
	struct mq_attr queue_attr;
	queue_attr.mq_maxmsg = SIZE;
	queue_attr.mq_msgsize = SIZE;
	int prio;
	if ((ds = mq_open(QUEUE_NAME, O_CREAT | O_RDWR , 0600, &queue_attr)) == (mqd_t)-1){
		perror("Ошибка создания очереди!\n");
		return -1;
	}
	if ((ds2 = mq_open(QUEUE_NAME_2, O_CREAT | O_RDWR , 0600, &queue_attr)) == (mqd_t)-1){
                perror("Ошибка создания очереди!\n");
                return -1;
        }
	while(1){
		char text[SIZE];
		char new_text[SIZE];
		printf(">> ");
	        fgets(text, SIZE, stdin);
		if (mq_send(ds, text, strlen(text), PRIORITY) == -1){
			perror("Ошибка отправки сообщения!\n");
			return -1;
		}
		if(strstr (text, "cancel") != NULL){
			printf("Выход...\n");
			break;
		}
		
		if (mq_receive(ds2, new_text, SIZE, &prio) == -1){
	                perror("Ошибка получения сообщения!");
        	        return -1;
      		}
	       	printf("<< %s", new_text);
		if(strstr (new_text, "cancel") != NULL){
                        printf("Выход...\n");
                        break;
                }

	}
	if (mq_unlink(QUEUE_NAME) == -1){
       	        perror("Ошибка удаления очереди сообщений!");
	}

	if (mq_unlink(QUEUE_NAME_2) == -1){
		perror("Ошибка удаления очереди сообщений!");
	}

	if (mq_close(ds) == -1){
		perror("Соединение закрыто!\n");
	}
	return 0;
}
