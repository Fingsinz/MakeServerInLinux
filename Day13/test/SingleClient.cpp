#include "Buffer.h"
#include "InetAddress.h"
#include "Socket.h"
#include "util.h"
#include <iostream>
#include <cstring>
#include <unistd.h>

int main()
{
	Socket *sock = new Socket();
	InetAddress *addr = new InetAddress("127.0.0.1", 1234);
	sock->connect(addr);

	int sockfd = sock->getFd();

	Buffer *sendBuffer = new Buffer();
	Buffer *readBuffer = new Buffer();

	while (true)
	{
		fprintf(stdout, "[Client sent]:\t");
		sendBuffer->getline();
		ssize_t writeLen = write(sockfd, sendBuffer->c_str(), sendBuffer->size());

		if (writeLen == -1)
		{
			fprintf(stdout, "[Error]:\tSocket already disconnected\n");
			break;
		}

		int alreadyRead = 0;
		char buf[1024];

		while (true)
		{
			bzero(buf, sizeof(buf));
			ssize_t readLen = read(sockfd, buf, sizeof(buf));

			if (readLen > 0)
			{
				readBuffer->append(buf, readLen);
				alreadyRead += readLen;
			}

			else if (readLen == 0)
			{
				fprintf(stdout, "[Error]:\tServer disconnected\n");
				exit(EXIT_SUCCESS);
			}

			if (alreadyRead >= sendBuffer->size())
			{
				fprintf(stdout, "[Message]:\t%s\n", readBuffer->c_str());
				break;
			}
		}
		readBuffer->clear();
	}

	delete addr;
	delete sock;
	return 0;
}