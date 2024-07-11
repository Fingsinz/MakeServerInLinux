#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>

int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;
	bzero(&serverAddr, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(1234);

	connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

	return 0;
}