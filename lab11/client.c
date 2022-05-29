#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(int argc,char *argv[]){
	if(argc != 3){
		fprintf(stderr,"Недостаточно аргументов! use: %s <ip> [port]\n",argv[0]);	
		return -1;
	}
	const char *ip = argv[1];
	unsigned short int port = atoi(argv[2]);

	int sfd = socket(AF_INET,SOCK_STREAM,0);
//AF_INET - сетевое соединение на основе TCP\IPv4
//SOCK_STREAM - протокол данных (в нашем случае ТСР)
//*SOCK_DGRAM для UDP
//0 - протокол нижележащего уровня (если нужно по умолчанию, то 0)
	if(sfd == -1){
		perror("socket");
		return -1;
	}
	struct sockaddr_in addr; //Структура socketaddr_in используется для того чтобы задать адрес по протоколу IPv4
	addr.sin_family = AF_INET; //семейство адресов
	addr.sin_port = htons(port); //значение порта на хосте и в сети может иметь разный порядок байт. Функция меняет порядок байт (в нашем случае от хоста к сети)
//port - номер порта на котором будем слушать
	addr.sin_addr.s_addr = inet_addr(ip); //реобразовывает обычный вид IP-адреса cp (из номеров и точек) в двоичный код и сохраняет его в структуре
	socklen_t addrlen = sizeof(addr); //размер

	int ret = connect(sfd,(const struct sockaddr*)(&addr),addrlen);
	//sfd - сокет клиента (который создается в клиенте)

//size
	if(ret == -1){
		perror("connect");
		return -1;
	}
	printf("Успешно подключен к серверу! \n");
	printf("Пожалуйста, введите ваше имя: ");
	char name[100];
	gets(name);
	send(sfd,name,strlen(name)+1,0); //отправление сообщения в сокет
	pid_t pid = fork();
	if(pid == -1){
		perror("fork");	
		return -1;
	}
	if(pid == 0){
		while(1){
			char buf[1024]={};
			gets(buf);
			if(send(sfd,buf,strlen(buf)+1,0)<=0){ //отправление сообщения в сокет
				break;	
			}
		}
	}else{
		while(1){
			char buf[1024]={};
			if(recv(sfd,buf,1024,0)<=0){  //используются для получения сообщений из сокета
				break;
			}
			time_t timep;
			time(&timep);
			printf("%s\n",ctime(&timep));
			printf("%s\n",buf);
		}
	}
	close(sfd);
	return 0;	
}
