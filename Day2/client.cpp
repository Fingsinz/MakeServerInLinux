#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
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

	errorif(connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1, "socket connect error");

	while (true)
	{
		char buffer[1024];
		std::cin >> buffer;
		ssize_t writeLen = write(sockfd, buffer, sizeof(buffer));

		if (writeLen == -1)
		{
			std::cout << "Socket already disconnected!\n";
			break;
		}

		bzero(buffer, sizeof(buffer));

		ssize_t readLen = read(sockfd, buffer, sizeof(buffer));

		if (readLen > 0)
		{
			std::cout << buffer << "\n";
		}

		else if (readLen == 0)
		{
			std::cout << "Server socket disconnected!\n";
			break;
		}

		else if (readLen == -1)
		{
			close(sockfd);
			errorif(true, "socket read error");
		}
	}

	close(sockfd);
	return 0;
}