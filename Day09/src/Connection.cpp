#include "Buffer.h"
#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include "util.h"
#include <unistd.h>
#include <cstring>
#include <iostream>

Connection::Connection(EventLoop *_loop, Socket *_sock) : loop(_loop), sock(_sock),
channel(nullptr), readBuffer(nullptr)
{
	channel = new Channel(loop, sock->getFd());
	std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
	channel->setCallback(cb);
	channel->enableReading();
	readBuffer = new Buffer();
}

Connection::~Connection()
{
	delete channel;
	delete sock;
}

void Connection::echo(int sockfd)
{
	char buf[1024];

	while (true)
	{
		bzero(buf, sizeof(buf));
		ssize_t readLen = read(sockfd, buf, sizeof(buf));

		if (readLen > 0)
		{
			readBuffer->append(buf, readLen);
		}

		// �ͻ��������жϡ�������ȡ
		else if (readLen == -1 and errno == EINTR)
		{
			std::cout << "[Continue]\n";
			continue;
		}

		// ������ IO����ʾ����ȫ����ȡ���
		else if (readLen == -1 and ((errno == EAGAIN) or (errno == EWOULDBLOCK)))
		{
			// �������£�������ΪEAGAIN��EWOULDBLOCK
			std::cout << "[From client " << sockfd << " ]:\t" << readBuffer->c_str() << "\n";
			errorif(write(sockfd, readBuffer->c_str(), readBuffer->size()) == -1, "Socket write error");
			readBuffer->clear();
			break;
		}

		// EOF����ʾ�ͻ��˶Ͽ�����
		else if (readLen == 0)
		{
			std::cout << "[Client " << sockfd << " ]:\tState closed\n";
			deleteConnectionCallback(sock);
			break;
		}
	}
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> _cb)
{
	deleteConnectionCallback = _cb;
}