#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> //////// add
#include <arpa/inet.h> ////// add

void write_file(int sock);
// функция обработки ошибок
void error(const char *msg) {
	perror(msg);
	exit(1);
}
// количество активных пользователей
int nclients = 0;

// печать количества активных пользователей
void printusers() {
	if(nclients) {
		printf("%d user on-line\n", nclients);
	}
	else {
		printf("No User on line\n");
	}
}

int main(int argc, char *argv[])
{
	char buff[1024]; // Буфер для различных нужд
	int sockfd, newsockfd; // дескрипторы сокетов
	int portno; // номер порта
	int pid; // id номер потока
	socklen_t clilen; // размер адреса клиента типа socklen_t
	struct sockaddr_in serv_addr, cli_addr; // структура сокета сервера и клиента
	printf("TCP SERVER DEMO\n");
	// ошибка в случае если мы не указали порт
	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	// Шаг 1 - создание сокета
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) error("ERROR opening socket");
	// Шаг 2 - связывание сокета с локальным адресом
	bzero((char*) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY; // сервер принимает подключения на все IP-адреса
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding");
	// Шаг 3 - ожидание подключений, размер очереди - 5
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	// Шаг 4 - извлекаем сообщение из очереди (цикл извлечения запросов на подключение)
	while (1) {
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) error("ERROR on accept");
		nclients++;
		// вывод сведений о клиенте
		struct hostent *hst;
		hst = gethostbyaddr((char *)&cli_addr.sin_addr, 4, AF_INET);
		printf("+%s [%s] new connect!\n", (hst) ? hst->h_name : "Unknown host", (char*)inet_ntoa(cli_addr.sin_addr));
		printusers();
		pid = fork();
		if (pid < 0) error("ERROR on fork");
		if (pid == 0) {
			//close(sockfd);
			write_file(newsockfd);
			nclients--; //// add
			close(sockfd);
			exit(0);
		}
		else {close(newsockfd); nclients--;} /////// add
	}
	close(sockfd);
	return 0;
}

void write_file(int sock){
	FILE *fp;
	int bytes_recv;
	char buff[20 * 1024];
	fp = fopen("file.txt", "w");
	if (fp == NULL){
		error("Ошибка чтения файла");
	}
	while(1){
		bytes_recv = recv(sock, buff, 20*1024, 0);
		if (bytes_recv == -1 ){
			error("Ошибка принятия данных");
			break;
			exit(0);
		}
		printf("Data written\n");
		fprintf(fp, "%s", buff);
		bzero(buff, 20*1024);
		break;
	}
	nclients--;
	printf("-disconnect\n");
        printusers();
	return;
}
