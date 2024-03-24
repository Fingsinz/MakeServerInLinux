#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "util.h"

int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	errorif(sockfd == -1, "socket create error");

	struct sockaddr_in serverAddr;
	bzero(&serverAddr, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(1234);

	errorif(bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1, "socket bind error");

	errorif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");

	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	bzero(&clientAddr, sizeof(clientAddr));
	int clientfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);

	errorif(clientfd == -1, "socket accept error");

	std::cout << "Client connected: " << clientfd <<
		"\tIP: " << inet_ntoa(clientAddr.sin_addr) <<
		"\tPort: " << ntohs(clientAddr.sin_port) << std::endl;

	while (true)
	{
		char buffer[1024];
		bzero(buffer, sizeof(buffer));

		size_t readLen = read(clientfd, buffer, sizeof(buffer));

		if (readLen > 0)
		{
			std::cout << "Message from client:" << clientfd << ": " << buffer << "\n";
			write(clientfd, buffer, sizeof(buffer));
		}

		else if (readLen == 0)
		{
			std::cout << "Client " << clientfd << " disconnected\n";
			close(clientfd);
			break;
		}

		else if (readLen == -1)
		{
			close(clientfd);
			errorif(true, "socket read error");
		}
	}

	close(sockfd);
	return 0;
}