#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLIENTS 12


int main(int argc, char** argv){
	char* port = "1234";
	if(argc!=1) port = argv[1];


	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd==-1){perror(NULL); exit(1);}

	struct addrinfo hints, *result;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int s = getaddrinfo(NULL, port, &hints, &result);
	if(s!=0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(1);
	}

	if(bind(socket_fd, result->ai_addr, result->ai_addrlen) != 0){
		perror("bind()");
		exit(1);
	}

	if(listen(socket_fd, MAX_CLIENTS) != 0){
		perror("listen()");
		exit(1);
	}

	struct sockaddr_in* result_addr = (struct sockaddr_in*) result->ai_addr;
	printf("Listening on file descriptor %d on port %d\n", socket_fd, ntohs(result_addr->sin_port));

	printf("Waiting for connection...\n");
	int client_fd = accept(socket_fd, NULL, NULL);
	printf("Connection made: client_fd=%d\n", client_fd);

	char buffer[1000];
	int len = read(client_fd, buffer, sizeof(buffer)-1);
	buffer[len] = '\0';

	printf("Read %d chars\n", len);
	printf("===\n");
	printf("%s\n", buffer);

	return 0;
}
