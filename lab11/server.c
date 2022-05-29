#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <locale.h>

#define MAX 100
typedef struct Client{
	int cfd;
	char name[40];
}Client;

Client client[MAX] = {};
size_t cnt = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast(char *msg,Client c){
	size_t i;
	pthread_mutex_lock(&mutex);
	for(i=0;i<cnt;i++){
		if(client[i].cfd != c.cfd){
			if(send(client[i].cfd,msg,strlen(msg),0)<=0){  //отправление сообщения в сокет
				break;
			}
		}
	}
	pthread_mutex_unlock(&mutex);
}

void *pthread_run(void *arg){
	Client cl = *(Client*)(arg);
	while(1){
		char buf[1024]={};
		strcpy(buf,cl.name);
		strcat(buf," :");
		int ret = recv(cl.cfd,buf+strlen(buf),1024-strlen(buf),0); //используются для получения сообщений из сокета
		if(ret <= 0){
			size_t i;
			for(i=0;i<cnt;i++){
				if(client[i].cfd == cl.cfd){
					client[i] = client[cnt-1];
					--cnt;
					strcpy(buf,"Клиент ");
					strcat(buf,cl.name);
					strcat(buf," вышел");
					break;
				}	
			}
			broadcast(buf,cl);
			close(cl.cfd);
			return NULL;
		}else{
			broadcast(buf,cl);
		}
	}
}

int main(int argc,char *argv[]){
 setlocale(LC_ALL, "Rus");
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

	struct sockaddr_in addr;  //Структура socketaddr_in используется для того чтобы задать адрес по протоколу IPv4
	addr.sin_family = AF_INET; //семейство адресов
	addr.sin_port = htons(port);  //значение порта на хосте и в сети может иметь разный порядок байт. Функция меняет порядок байт (в нашем случае от хоста к сети)
//port - номер порта на котором будем слушать
	addr.sin_addr.s_addr = inet_addr(ip); //реобразовывает обычный вид IP-адреса cp (из номеров и точек) в двоичный код и сохраняет его в структуре
	socklen_t addrlen = sizeof(addr);  //размер

	int ret = bind(sfd,(struct sockaddr*)(&addr),addrlen);
	
	 //sfd - сокет который привязываем
        //addr - куда привязывае
        //addrlen - размер
	if(ret == -1){
		perror("bind");
		return -1;
	}
	if(listen(sfd,10)==-1){   // слушаем на сокете sfd, 10 клиентов может ожидать в очереди
		perror("listen");
		return -1;
	}

	while(1){
		struct sockaddr_in caddr;
		socklen_t len = sizeof(caddr);
		
		printf("Ожидание подключения клиентов .... \n");
		int cfd = accept(sfd,(struct sockaddr*)(&caddr),&len);
		//sfd сокет-дескриптор для принятия связей от клиентов.
		//ссылка на адрес клиента (структура sockaddr ) для соответствующего домена
		//указатель на целое число - длину структуры адреса
		if(cfd == -1){
			perror("accept");
			return -1;
		}
		char buf[100]={};
		recv(cfd,&client[cnt].name,40,0);   //используются для получения сообщений из сокета
		client[cnt].cfd = cfd;
		pthread_t id;
		strcpy(buf,"Клиент ");
		strcat(buf,client[cnt].name);
		strcat(buf," онлайн");
		broadcast(buf,client[cnt]);
		ret = pthread_create(&id,NULL,pthread_run,(void*)(&client[cnt]));
		 //1-адрес дял хранения индефик созд потока 2-безтиповой указатель атрибутов потока 3- ук на потоковую функцию  4- ук содерж аргументы потока
		cnt++;
		if(ret != 0){
			printf("pthread_create:%s\n",strerror(ret));
			continue;
		}
		printf("Клиент успешно подключился\n");
	}
	return 0;
}
