#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> //////// add
#include <arpa/inet.h> ////// add
// функция обслуживания
// подключившихся пользователей
void dostuff(int);
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
// функция обработки данных
int myfunc(int action, int a, int b) {
	if (action == 1) return a + b;
	if (action == 2) return a - b; //////// add
	if (action == 3) return a * b; //////// add
	if (action == 4) return a / b; //////// add
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
			close(sockfd);
			dostuff(newsockfd);
			nclients--; //// add
			exit(0);
		}
		else {close(newsockfd); nclients--;} /////// add
	}
	close(sockfd);
	return 0;
}

void dostuff (int sock) {
	int bytes_recv; // размер принятого сообщения
	int a,b, act; // переменные для myfunc
	char buff[20 * 1024];
	#define str1 "Enter 1 parameter\r\n"
	#define str2 "Enter 2 parameter\r\n"
	#define str3 "Enter 3 parameter: 1.Sum; 2. Dif; 3. Increase; 4. Division;\r\n" ////// add
	// отправляем клиенту сообщение
	write(sock, str1, sizeof(str1));
	// обработка первого параметра
	bytes_recv = read(sock,&buff[0],sizeof(buff));
	if (bytes_recv < 0) error("ERROR reading from socket");
	a = atoi(buff); // преобразование первого параметра в int
	// отправляем клиенту сообщение
	write(sock,str2,sizeof(str2));
	bytes_recv = read(sock,&buff[0],sizeof(buff));
	if (bytes_recv < 0) error("ERROR reading from socket");
	b = atoi(buff); // преобразование второго параметра в int
	// отправляем клиенту третье сообщение
	write(sock, str3, sizeof(str3)); ///// begin
	bytes_recv = read(sock, &buff[0], sizeof(buff));
	if (bytes_recv < 0) error ("ERROR reading from socket");
	act = atoi(buff);

	a = myfunc(act, a,b); // вызов пользовательской функции //// end
	snprintf(buff, strlen(buff), "%d", a); // преобразование результата в строку
	buff[strlen(buff)] = '\n'; // добавление к сообщению символа конца строки 
	buff[strlen(buff)] = 'q';
	buff[strlen(buff)] = 'u';
	buff[strlen(buff)] = 'i';
	buff[strlen(buff)] = 't';
	buff[strlen(buff)] = '\n';
	//////////////// add quit
	// отправляем клиенту результат
	write(sock,&buff[0], sizeof(buff));
	nclients--; // уменьшаем счетчик активных клиентов
	printf("-disconnect\n");
	printusers();
	return;
}
