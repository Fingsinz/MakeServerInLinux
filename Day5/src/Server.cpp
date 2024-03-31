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

		// �ͻ��������жϡ�������ȡ
		else if (readLen == -1 and errno == EINTR)
		{
			std::cout << "Continue\n";
			continue;
		}

		// ������ IO����ʾ����ȫ����ȡ���
		else if (readLen == -1 and ((errno == EAGAIN) or (errno == EWOULDBLOCK)))
		{
			// �������£�������ΪEAGAIN��EWOULDBLOCK
			std::cout << "Finish reading once\n";
			break;
		}

		// EOF����ʾ�ͻ��˶Ͽ�����
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
	// ����һ���µ�InetAddress�������洢�ͻ��˵�ַ��Ϣ
	InetAddress *clientAddr = new InetAddress();

	//ͨ��ʹ�ÿͻ��˵�ַ�������Է������׽��ֵ����ӣ�����һ���µ�Socket����
	Socket *clientSock = new Socket(serverSock->accept(clientAddr));

	// ��ӡ�й��¿ͻ������ӵ���Ϣ
	std::cout << "New client " << clientSock->getFd() << ": " <<
		inet_ntoa(clientAddr->addr.sin_addr) << " : " << ntohs(clientAddr->addr.sin_port) << "\n";

	clientSock->setNonBlocking();

	// ���¼�ѭ����Ϊ�ͻ����׽����ļ�����������һ���µ�Channel����
	Channel *clientChannel = new Channel(loop, clientSock->getFd());

	// ����һ���ص�����������ͻ����׽��ֵĶ��¼�
	std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clientSock->getFd());
	clientChannel->setCallback(cb);

	// Ϊ�ͻ���ͨ�����ö�ȡ�¼�
	clientChannel->enableReading();
}