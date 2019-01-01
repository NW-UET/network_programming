#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <cstdint>
#include "package.h"
#include "convert.h"
using namespace package;
static void *doit(void *arg);
int main(int argc, char const *argv[])
{
    //created argument for thread
	int listenfd, connfd;
	pthread_t tid;
	socklen_t addrlen, len;
	struct sockaddr cliaddr;
	//socket
	/*
	family:
	AF_INET IPv4
	AF_INET6 IPv6
	AF_LOCAL Unix Domain
	
	type:
	SOCK_STREAM (TCP)
	SOCK_DGRAM (UDP)
	SOCK_RAW
	
	protocol:
	0 mac dinh
	neu SOCK_RAW moi can thiet lap
	*/

	int sockfd;
	
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
		perror("socket");
		exit(0);
	}
	int opval=1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opval,sizeof(opval));

	// struct timeval	tv;
	// tv.tv_sec = 5;
	// tv.tv_usec = 0;
	// setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	
	struct sockaddr_in sockaddr;
	bzero(&sockaddr, sizeof(sockaddr));
	sockaddr.sin_family=AF_INET;
	sockaddr.sin_port=htons(9876);
	sockaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	// printf("%d\n%d\n",inet_addr("127.0.0.1"),INADDR_ANY);
	//bind
	/*
	
	*/
	if(bind(sockfd,(struct sockaddr*) &sockaddr, sizeof(sockaddr))<0){
		perror("bind");
		exit(0);
	}
	while(1){
		//listen
		printf("Listening connection!!!\n");
		if(listen(sockfd,10)<0){
			perror("listen");
			continue;
		}
		
		//accept
		int *newfd=(int*)malloc(sizeof(int));
		struct sockaddr_in cliaddr;
		int cliaddr_len=sizeof(cliaddr);
		(*newfd) = accept(sockfd,(struct sockaddr*) &cliaddr, (socklen_t*) &cliaddr_len);
		if((*newfd)<0){
			if(errno==EINTR) continue;
			perror("accept");
			continue;
		}

		//CREATE THREAD
		pthread_create(&tid,NULL,&doit,(void*)newfd);

		//printf("Client IP: %s\nClient Port: %d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);
		
		
	}	
    return 0;
}
static void *doit(void *arg){
	int newfd= *((int*)arg);
	pthread_detach(pthread_self());
	free(arg);
    //read data received
	int nbytes;
	char buf_char[1000];
    if((nbytes=read(newfd,buf_char,sizeof(buf_char)))<0){
		perror("read");
		exit(0);
	}
	printf("nbytes = %d\n",nbytes);
	// nho delete buf
	// uint8_t* buf=new uint8_t[nbytes];
	// char_to_uint8(buf_char,buf);
	char* buf=buf_char;
	if (buf[0]==1) {
		printf("xu ly update list file\n");
		ListUpdate package(buf);
		package.printObjectPackage();
	}else if(buf[0]==3){
		printf("xu ly request list files\n");
	}else if(buf[0]==5){
		printf("xu ly request list hosts\n");
	}else{
		delete buf;
		exit(0);
	}
    
}
