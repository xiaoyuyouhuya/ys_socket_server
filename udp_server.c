#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#define MAX_LEN 1024

int main(int argc, char **argv){
	if(argc != 2){
		printf("input error!\n");
	}
	
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(atoi(argv[1]));
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(my_addr.sin_zero), 8);

	int sock_fd;
	char recv_buff[MAX_LEN];

	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_fd < 0){
		perror("socket");
		exit(1);
	}
	int ret_b;
	ret_b = bind(sock_fd, (struct sockaddr_in *)&my_addr, sizeof(my_addr));
	if(ret_b < 0){
		perror("bind");
		exit(1);
	}
	
	struct sockaddr_in client_addr;
	int clent_len = sizeof(client_addr);
	int ret_recv,ret_send;

	for(;;){
		ret_recv = recvfrom(sock_fd, recv_buff, MAX_LEN, 0, (struct sockaddr_in *)&client_addr);
		if(ret_recv > 0){
			ret_send = sendto(sock_fd, recv_buff, strlen(recv_buff), 0, (struct sockaddr_in *)&client_addr, sizeof(client_addr));
			if(ret_send < 0){
				perror("sendto");
				break;
			}
		}
		else{
			perror("recvfrom");
			break;
		}
	}
	return 0;
}
