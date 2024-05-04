#include "Buffer.h"
#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include <unistd.h>
#include <cstring>
#include <iostream>

Connection::Connection(EventLoop *loop, Socket *sock) : mLoop(loop), mSocket(sock),
mChannel(nullptr), mReadBuffer(nullptr)
{
	mChannel = new Channel(mLoop, mSocket->getFd());
	mChannel->useET();
	mChannel->enableReading();

	std::function<void()> cb = std::bind(&Connection::echo, this, mSocket->getFd());
	mChannel->setReadCallback(cb);
	mReadBuffer = new Buffer();
}

Connection::~Connection()
{
	delete mChannel;
	delete mSocket;
	delete mReadBuffer;
}

void Connection::echo(int sockfd)
{
	char buf[1024];

	while (true)
	{
		bzero(buf, sizeof(buf));
		ssize_t readLen = read(sockfd, buf, sizeof(buf));

		if (readLen > 0)
			mReadBuffer->append(buf, readLen);

		// 客户端正常中断、继续读取
		else if (readLen == -1 and errno == EINTR)
		{
			std::cout << "[Continue]\n";
			continue;
		}

		// 非阻塞 IO，表示数据全部读取完毕
		else if (readLen == -1 and ((errno == EAGAIN) or (errno == EWOULDBLOCK)))
		{
			// 非阻塞下，错误码为EAGAIN或EWOULDBLOCK
			std::cout << "[From client " << sockfd << " ]:\t" << mReadBuffer->c_str() << "\n";
			send(sockfd);
			mReadBuffer->clear();
			break;
		}

		// EOF，表示客户端断开连接
		else if (readLen == 0)
		{
			std::cout << "[Client " << sockfd << " ]:\tdisconnected\n";
			mDeleteConnectionCallback(sockfd);
			break;
		}

		else
		{
			std::cout << "[Error]:\tConnection reset by peer\n";
			mDeleteConnectionCallback(sockfd);
			break;
		}
	}
}

void Connection::setDeleteConnectionCallback(std::function<void(int)> const &_cb)
{
	mDeleteConnectionCallback = _cb;
}

void Connection::send(int sockfd)
{
	// 创建一个缓冲区来保存要发送的数据，其大小等于readBuffer的大小
	char buf[mReadBuffer->size()];
	strcpy(buf, mReadBuffer->c_str());

	// 存储要发送的数据的总大小
	int dataSize = mReadBuffer->size();

	// 将要发送的剩余数据初始化为总数据大小
	int dataLeft = dataSize;

	while (dataLeft > 0)
	{
		// 尝试发送数据
		ssize_t writeLen = write(sockfd, buf + dataSize - dataLeft, dataLeft);

		// 错误处理
		if (writeLen == -1 and errno == EAGAIN)
			break;

		// 更新写入操作后要发送的剩余数据
		dataLeft -= writeLen;
	}
}