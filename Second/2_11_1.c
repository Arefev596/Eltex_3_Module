#include <stdio.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#define QUEUE_NAME "/my_queue"
#define QUEUE_NAME_2 "/my_queue_2"
#define PRIORITY 1
#define SIZE 256

int main(){
	mqd_t ds, ds2;
	struct mq_attr attr;
	int prio;
	if ((ds = mq_open(QUEUE_NAME, O_RDWR | O_NONBLOCK, 0600, NULL)) == (mqd_t)-1){
		perror("Ошибка создания очереди!\n");
		return -1;
	}
	if ((ds2 = mq_open(QUEUE_NAME_2, O_RDWR | O_NONBLOCK, 0600, NULL)) == (mqd_t)-1){
                perror("Ошибка создания очереди!\n");
                return -1;
        }
	
	while(1){
		char text[SIZE];
		char new_text[SIZE];
		while(1){
		if (mq_receive(ds, new_text, SIZE, &prio) == -1){
			sleep(1);
		}
		else{
			break;
		}
		}
		printf("<< %s", new_text);
		
		if(strstr (new_text, "cancel") != NULL){
                        printf("Выход...\n");
                        break;
                }

		printf(">> ");
                fgets(text, SIZE, stdin);
                if (mq_send(ds2, text, strlen(text), PRIORITY) == -1){
                        perror("Ошибка отправки сообщения!\n");
                        return -1;
                }
                if(strstr (text, "cancel") != NULL){
                        printf("Выход...\n");
                        break;
                }

	}
	if (mq_close(ds) == -1){
		perror("Ошибка закрытия очереди!");
	}
	if (mq_unlink(QUEUE_NAME) == -1){
		perror("Ошибка удаления очереди сообщений!");
	}
	if (mq_unlink(QUEUE_NAME_2) == -1){
                perror("Ошибка удаления очереди сообщений!");
        }
	return 0;
}
