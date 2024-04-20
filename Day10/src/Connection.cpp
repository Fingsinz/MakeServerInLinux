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
	channel->useET();
	channel->enableReading();

	std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
	channel->setReadCallback(cb);
	channel->setUseThreadPool(true);
	readBuffer = new Buffer();
}

Connection::~Connection()
{
	delete channel;
	delete sock;
	delete readBuffer;
}

void Connection::echo(int sockfd)
{
	char buf[1024];

	while (true)
	{
		bzero(buf, sizeof(buf));
		ssize_t readLen = read(sockfd, buf, sizeof(buf));

		if (readLen > 0)
			readBuffer->append(buf, readLen);

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
			// errorif(write(sockfd, readBuffer->c_str(), readBuffer->size()) == -1, "Socket write error");
			send(sockfd);
			readBuffer->clear();
			break;
		}

		// EOF����ʾ�ͻ��˶Ͽ�����
		else if (readLen == 0)
		{
			std::cout << "[Client " << sockfd << " ]:\tState closed\n";
			deleteConnectionCallback(sockfd);
			break;
		}
	}
}

void Connection::setDeleteConnectionCallback(std::function<void(int)> _cb)
{
	deleteConnectionCallback = _cb;
}

void Connection::send(int sockfd)
{
	// ����һ��������������Ҫ���͵����ݣ����С����readBuffer�Ĵ�С
	char buf[readBuffer->size()];
	strcpy(buf, readBuffer->c_str());

	// �洢Ҫ���͵����ݵ��ܴ�С
	int dataSize = readBuffer->size();

	// ��Ҫ���͵�ʣ�����ݳ�ʼ��Ϊ�����ݴ�С
	int dataLeft = dataSize;

	while (dataLeft > 0)
	{
		// ���Է�������
		ssize_t writeLen = write(sockfd, buf + dataSize - dataLeft, dataLeft);

		// ������
		if (writeLen == -1 and errno == EAGAIN)
			break;

		// ����д�������Ҫ���͵�ʣ������
		dataLeft -= writeLen;
	}
}