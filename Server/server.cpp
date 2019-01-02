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
#include "message.h"
#define BLOCK_SIZE 2048;
static void *doit(void *arg);
int main(int argc, char const *argv[])
{
    //created argument for thread
	pthread_t tid;
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
	sockaddr.sin_port=htons(6789);
	sockaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	printf("%d\n%d\n",6789,inet_addr("127.0.0.1"));
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
	uint8_t type;
    type=ReadHeader(newfd);
    printf("%d",type);
    if(type==0){
        // printf("")
        struct FileListUpdateRequest package0;
        package0.ReadPayload(newfd);

        struct FileListUpdateResponse package1;
        package1.n_files=package0.n_files;
        for(int i=0;i<package0.n_files;i++){
            Filestatus temp;
            temp.filename=package0.file_list.at(i).filename;
            temp.filename_length=package0.file_list.at(i).filename_length;
            temp.status=0;
            package1.filestatus_list.push_back(temp);
        }
        package1.print();
        package1.Write(newfd);
        close(newfd);
        return NULL;
    }else if(type==2){
        struct ListFilesResponse package3;
        package3.n_files=1;
        Filesize temp;
        temp.filename_length=1;
        temp.filename="a";
        temp.file_size=1;
        package3.filesize_list.push_back(temp);
        package3.Write(newfd);
        close(newfd);
        return NULL;
    }else if(type ==4){
        struct ListHostsResponse package5;
        package5.n_hosts=2;
        package5.IP_addr_list.push_back(inet_addr("127.0.0.1"));
        package5.IP_addr_list.push_back(inet_addr("192.156.0.2"));
        close(newfd);
        return NULL;
    }else{
        close(newfd);
        return NULL;
    }
    
            
        // case 2:
        //     ListFilesResponse package3;
        //     package3.n_files=2;
        //     Filesize r3;
        //     r3.filename_length=4;
        //     r3.filename="ab.o";
        //     r3.file_size=1024;
        //     package3.filesize_list.push_back(r3);
        //     r3.filename_length=6;
        //     r3.filename="abcd.o";
        //     r3.file_size=2048;
        //     package3.filesize_list.push_back(r3);
        // case 4:
        //     ListHostsResponse package4;
        //     package4.n_hosts=2;
        //     package4.IP_addr_list.push_back(inet_addr("127.0.0.2"));
        //     package4.IP_addr_list.push_back(inet_addr("127.0.0.3"));
        
}
