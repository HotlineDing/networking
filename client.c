#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char **argv) {
		if(argc!=3) {printf("Usage ./client [hostname] [port]\n"), exit(1);}
		char* host = argv[1];
		char* port = argv[2];

		int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

		struct addrinfo hints, *result;
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;

		int s = getaddrinfo(host, port, &hints, &result);
		if(s!=0){
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
			exit(1);
		}

		if(connect(socket_fd, result->ai_addr, result->ai_addrlen) == -1){
			perror("connect");
			exit(2);
		}

		char* buffer = "GET / HTTP/1.0\r\n\r\n";
		printf("SENDING: %s", buffer);
		printf("===\n");

		write(socket_fd, buffer, strlen(buffer));

		char response[1000];
		int len = read(socket_fd, response, 999);
		response[len] = '\0';
		printf("%s\n", response);

    return 0;
}
