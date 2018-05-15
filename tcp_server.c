#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX_LEN 1024

int close_flag = 0;

int tcp_socket_init(unsigned short port){
	//1.socket创建套接字
	int ret_s;
	ret_s = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == ret_s){
		fprintf(stderr, "socket create error!\n");
		return -1;
	}

	struct sockaddr_in my_addr;
	//memset(&my_addr, 0, sizeof(sockaddr_in));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(my_addr.sin_zero), 8);

	//2.bind绑定端口
	int ret_b;
	ret_b = bind(ret_s, (struct sockaddr_in *)&my_addr, sizeof(my_addr));
	if(-1 == ret_b){
		fprintf(stderr, "bind link error!\n");
		return -1;
	}

	//3.listen监听
	int ret_l;
	ret_l = listen(ret_s, 10);
	if(-1 == ret_l){
		fprintf(stderr, "listen error!\n");
		return -1;
	}

	printf("TCP connect created!\n");
	return ret_s;
}

void close_server(int num){
	close_flag = 1;
}

int connect_task(int listener){
	signal(SIGINT, close_server);
	for(;;){

		if(close_flag)
			break;

		char recv_buff[MAX_LEN];
		//4.accept处理
		struct sockaddr_in client_addr;
		unsigned int clientlen = sizeof(client_addr);
		int acc_fd;

		acc_fd = accept(listener, (struct sockaddr *)&client_addr, (socklen_t*)&clientlen);
		printf("Now the link has been connected.\n");
		if(acc_fd >= 0){
			pid_t PID;
			PID = fork();
			if(PID > 0){
				close(acc_fd);
				continue;
			}
			else if(PID == 0){
				printf("server listening...\n");
				int ret_r;
				for(;;){
				ret_r = recv(acc_fd, (void*)recv_buff, MAX_LEN, 0);
				if(!ret_r){
					close(acc_fd);
					break;
				}
				send(acc_fd, (void*)recv_buff, strlen(recv_buff), 0);
				}
			}
			else{
				fprintf(stderr, "process creat error!\n");
				return -1;
			}

		}
		else{
			fprintf(stderr, "accept error!\n");
			return -1;
		} 

	}
	return 1;
}
int main(int argc, char **argv){
	int server_fd;
	if(argc != 2){
		fprintf(stderr, "input error!\n");
		return -1;
	}
	int port;
	sscanf(argv[1], "%d", &port);
	server_fd = tcp_socket_init((unsigned short)port);
	connect_task(server_fd);
	return 0;
}
