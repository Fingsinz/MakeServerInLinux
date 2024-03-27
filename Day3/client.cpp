#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "util.h"

#define BUFFER_SIZE 1024

int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	errorif(sockfd == -1, "socket create error");

	struct sockaddr_in serverAddr;
	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(1234);

	errorif(connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1, "socket connect error");

	while (true)
	{
		char buffer[BUFFER_SIZE];
		bzero(buffer, sizeof(buffer));
		std::cin >> buffer;
		ssize_t bytesWrite = write(sockfd, buffer, sizeof(buffer));
		if (bytesWrite == -1)
		{
			std::cout << "Socket already disconnected" << std::endl;
			break;
		}

		bzero(buffer, sizeof(buffer));
		ssize_t bytesRead = read(sockfd, buffer, sizeof(buffer));

		if (bytesRead > 0)
		{
			std::cout << buffer << std::endl;
			break;
		}
		else if (bytesRead == 0)
		{
			std::cout << "Server socket disconnected\n";
			break;
		}
		else if (bytesRead == -1)
		{
			close(sockfd);
			errorif(true, "Socket read error");
		}
	}

	close(sockfd);
	return 0;
}