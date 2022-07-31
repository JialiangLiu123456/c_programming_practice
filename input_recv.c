/*
File name: input_recv.c
Author: Jialiang Liu
Creation date: July 31, 2022
Contact email: jialiangliu1@yahoo.com
Description: This program will listen on indicated IP address and port as server-side and display input from the client-side. The client-side function can be implemented by using 'netcat'.
Copyright information: This program contains codes from Linux MAN page LISTEN(2). I don't know how to properly use the copyright information, please contact me if there is any copyright related issue.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/types.h>
#define IP_SIZE 16
#define BUFFER_SIZE 1024
#define BACKLOG 50
#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)
int main()
{
	char ip[IP_SIZE];
	char buffer[BUFFER_SIZE];
	int port = 0;
	printf("Please input the ip address:");
	scanf("%s", &ip);
	printf("Please input the port number:");
	scanf("%d", &port);

	int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s == -1)
		handle_error("socket");

	struct sockaddr_in svr_address, clt_address;
	memset(&svr_address, 0, sizeof(struct sockaddr_in));	// Initialize the structure
	svr_address.sin_family = AF_INET;
	svr_address.sin_port = htons(port);
	svr_address.sin_addr.s_addr = inet_addr(ip);

	if(bind(s, (struct sockaddr *) &svr_address, sizeof(struct sockaddr_in)) == -1)
		handle_error("bind");	

	if(listen(s, BACKLOG) == -1)
		handle_error("listen");
	printf("****Waiting for connection****\n\n");

	socklen_t clt_address_size = sizeof(struct sockaddr_in);
	int c = accept(s, (struct sockaddr *) &clt_address, &clt_address_size);
	if(c == -1)
		handle_error("accept");
	printf("****Connection Established****\n");
	
	while(1){
		if(read(c,buffer,sizeof(buffer)) == 0){
			printf("****Disconnected!****\n");
			break;
		}

		printf("%s", buffer);
	}

	close(s);
	return 0;
}
