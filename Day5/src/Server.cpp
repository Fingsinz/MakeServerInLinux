#include "Server.h"
#include <functional>
#include <cstring>
#include <unistd.h>
#include <iostream>

constexpr int READ_BUFFER = 1024;

Server::Server(EventLoop *_loop) : loop(_loop)
{
	Socket *serverSock = new Socket();
	InetAddress *serverAddr = new InetAddress("127.0.0.1", 1234);
	serverSock->bind(serverAddr);
	serverSock->listen();
	serverSock->setNonBlocking();

	Channel *serverChannel = new Channel(loop, serverSock->getFd());
	std::function<void()> cb = std::bind(&Server::newConnection, this, serverSock);
	serverChannel->setCallback(cb);
	serverChannel->enableReading();
}

Server::~Server()
{}

void Server::handleReadEvent(int fd)
{
	char buf[READ_BUFFER];

	while (true)
	{
		bzero(buf, sizeof(buf));
		ssize_t readLen = read(fd, buf, sizeof(buf));

		if (readLen > 0)
		{
			std::cout << buf << "\n";
			write(fd, buf, sizeof(buf));
		}

		else if (readLen == -1 and errno == EINTR)
		{
			std::cout << "Continue\n";
			continue;
		}

		else if (readLen == -1 and ((errno == EAGAIN) or (errno == EWOULDBLOCK)))
		{
			std::cout << "Finish reading once\n";
			break;
		}

		else if (readLen == 0)
		{
			std::cout << "Client quit\n";
			close(fd);
			break;
		}
	}
}

void Server::newConnection(Socket *sock)
{
	InetAddress *clientAddr = new InetAddress();
	Socket *clientSock = new Socket();
	clients.push_back({ clientAddr, clientSock });

	std::cout << "New client " << clientSock->getFd() << ": " <<
		inet_ntoa(clientAddr->addr.sin_addr) << " : " << ntohs(clientAddr->addr.sin_port) << "\n";
	clientSock->setNonBlocking();

	Channel *clientChannel = new Channel(loop, clientSock->getFd());
	std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clientSock->getFd());
	clientChannel->setCallback(cb);
	clientChannel->enableReading();
}