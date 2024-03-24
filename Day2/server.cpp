#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

void errorif(bool condition, const char *errmsg)
{
	if (condition)
	{
		perror(errmsg);
		exit(EXIT_FAILURE);
	}
}

int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;
	bzero(&serverAddr, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(1234);

	bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

	while (1)
	{
		listen(sockfd, SOMAXCONN);

		struct sockaddr_in clientAddr;
		socklen_t clientAddrLen = sizeof(clientAddr);
		bzero(&clientAddr, sizeof(clientAddr));
		int clientfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);

		std::cout << "Client connected: " << clientfd <<
			"\tIP: " << inet_ntoa(clientAddr.sin_addr) <<
			"\tPort: " << ntohs(clientAddr.sin_port) << std::endl;
	}

	return 0;
}