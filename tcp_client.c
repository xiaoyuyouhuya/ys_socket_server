#include <netinet/in.h>

#include <stdio.h>  
#include <string.h>//memset strlen  
#include <stdlib.h>  

#include <sys/types.h>  
#include <sys/stat.h>  
#include <sys/socket.h>  


/* 
 * unistd.h 中所定义的接口通常都是大量针对系统调用的封装（英语：wrapper functions） 
 * 如 fork、pipe 以及各种 I/O 原语（read、write、close 等等） 
 * */  
#include <unistd.h>  


#define MAX_STR 256  
int main(){  

	char buf[MAX_STR];  
	char succ[]="success";  
	char fail[]="failed";  
	char hello[]="hello";

	int clientFd;  
	clientFd=socket(AF_INET,SOCK_STREAM,0);//0:按给定的域和套接字类型选择默认协议  

	struct sockaddr_in serverAddr;  
	//memset(&serverAddr,0,sizeof(serverAddr));  
	serverAddr.sin_family=AF_INET;  
	serverAddr.sin_port=htons(8080);  
	serverAddr.sin_addr.s_addr=inet_addr("10.0.3.15");  

	printf("Socket has been created.\n");  

	int cRes=connect(clientFd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));  

	//判断是否成功  
	if(cRes<0){  
		printf("fail to connect\n");  
		exit(1);  
	}else{  
		printf("connect success!\n");  

	}  
	send(clientFd, (void*)hello, strlen(hello), 0);
	recv(clientFd, (void*)buf, MAX_STR, 0);
	/*int recvLen=recv(clientFd,(void*)buf,MAX_STR,0);
	if(recvLen>0){  
		printf("recv success:\n");  
		printf("recv: %s\n",buf);  
		send(clientFd,(void*)succ,strlen(succ),0);//不接受带外数据，阻塞式，可将数据路由出本地网络  
	}else{  
		printf("fail to recv the server's data\n");  
		send(clientFd,(void*)fail,strlen(fail),0);  
	} */ 
	close(clientFd);  
	printf("Now the connection has been broken\n");  
	close(clientFd);  


	return 0;  
}  
