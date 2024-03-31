#include "Channel.h"
#include "InetAddress.h"
#include "Server.h"
#include "Socket.h"
#include <cstring>
#include <functional>
#include <iostream>
#include <unistd.h>

constexpr int READ_BUFFER = 1024;

Server::Server(EventLoop *_loop) : loop(_loop)
{
	serverSock = new Socket();
	serverAddr = new InetAddress("127.0.0.1", 1234);
	serverSock->bind(serverAddr);
	serverSock->listen();
	serverSock->setNonBlocking();

	serverChannel = new Channel(loop, serverSock->getFd());
	std::function<void()> cb = std::bind(&Server::newConnection, this, serverSock);
	serverChannel->setCallback(cb);
	serverChannel->enableReading();
}

Server::~Server()
{
	delete serverSock;
	delete serverAddr;
	delete serverChannel;

	for (std::pair<Socket *, InetAddress * > client : clients)
		delete client.first, delete client.second;
}

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

		// 客户端正常中断、继续读取
		else if (readLen == -1 and errno == EINTR)
		{
			std::cout << "Continue\n";
			continue;
		}

		// 非阻塞 IO，表示数据全部读取完毕
		else if (readLen == -1 and ((errno == EAGAIN) or (errno == EWOULDBLOCK)))
		{
			// 非阻塞下，错误码为EAGAIN或EWOULDBLOCK
			std::cout << "Finish reading once\n";
			break;
		}

		// EOF，表示客户端断开连接
		else if (readLen == 0)
		{
			std::cout << "Client " << fd << " quit\n";
			close(fd);
			break;
		}
	}
}

void Server::newConnection(Socket *sock)
{
	// 创建一个新的InetAddress对象来存储客户端地址信息
	InetAddress *clientAddr = new InetAddress();

	//通过使用客户端地址接受来自服务器套接字的连接，创建一个新的Socket对象
	Socket *clientSock = new Socket(serverSock->accept(clientAddr));

	// 打印有关新客户端连接的信息
	std::cout << "New client " << clientSock->getFd() << ": " <<
		inet_ntoa(clientAddr->addr.sin_addr) << " : " << ntohs(clientAddr->addr.sin_port) << "\n";

	clientSock->setNonBlocking();

	// 在事件循环中为客户端套接字文件描述符创建一个新的Channel对象
	Channel *clientChannel = new Channel(loop, clientSock->getFd());

	// 定义一个回调函数来处理客户端套接字的读事件
	std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clientSock->getFd());
	clientChannel->setCallback(cb);

	// 为客户端通道启用读取事件
	clientChannel->enableReading();
}