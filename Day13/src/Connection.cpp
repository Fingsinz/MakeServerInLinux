#include "Buffer.h"
#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include "util.h"
#include <unistd.h>
#include <cstring>
#include <iostream>

Connection::Connection(EventLoop *loop, Socket *sock) : mLoop(loop), mSocket(sock)
{
	if (mLoop != nullptr)
	{
		mChannel = new Channel(mLoop, mSocket);
		mChannel->useET();
		mChannel->enableReading();
	}
	mReadBuffer = new Buffer();
	mSendBuffer = new Buffer();
	mState = State::Connected;
}

Connection::~Connection()
{
	if (mLoop != nullptr)
		delete mChannel;

	delete mSocket;
	delete mReadBuffer;
	delete mSendBuffer;
}

void Connection::read()
{
	errorif(mState != State::Connected, "[Error]\tConnection state is disconnected!");
	mReadBuffer->clear();
	if (mSocket->isNonBlocking())
		readNonBlocking();
	else
		readBlocking();
}

void Connection::write()
{
	errorif(mState != State::Connected, "[Error]\tConnection state is disconnected!");

	if (mSocket->isNonBlocking())
		writeNonBlocking();
	else
		writeBlocking();

	mSendBuffer->clear();
}

void Connection::send(std::string msg)
{
	setSentBuffer(msg.c_str());
	write();
}

void Connection::setOnConnectionCallback(std::function<void(Connection *)> const &callback)
{
	mOnConnectCallback = callback;
	// mChannel->setReadCallback([this] () { mOnConnectCallback(this); });
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> const &callback)
{
	mDeleteConnectionCallback = callback;
}

void Connection::setOnMessageCallback(std::function<void(Connection *)> const &callback)
{
	mOnMessageCallback = callback;
	std::function<void()> busi = std::bind(&Connection::business, this);
	mChannel->setReadCallback(busi);
}

void Connection::business()
{
	read();
	mOnMessageCallback(this);
}

Connection::State Connection::getState()
{
	return mState;
}

void Connection::close()
{
	mDeleteConnectionCallback(mSocket);
}

void Connection::setSentBuffer(char const *str)
{
	mSendBuffer->setBuf(str);
}

Buffer *Connection::getReadBuffer()
{
	return mReadBuffer;
}

char const *Connection::readBuffer()
{
	return mReadBuffer->c_str();
}

Buffer *Connection::getSendBuffer()
{
	return mSendBuffer;
}

char const *Connection::sendBuffer()
{
	return mSendBuffer->c_str();
}

void Connection::getlineSendBuffer()
{
	mSendBuffer->getline();
}

Socket *Connection::getSocket()
{
	return mSocket;
}

void Connection::readNonBlocking()
{
	int sockfd = mSocket->getFd();
	char buf[1024];

	while (true)
	{
		bzero(buf, sizeof(buf));
		ssize_t readLen = ::read(sockfd, buf, sizeof(buf));

		if (readLen > 0)
			mReadBuffer->append(buf, readLen);

		// 客户端正常中断、继续读取
		else if (readLen == -1 and errno == EINTR)
		{
			std::cout << "[Continue Reading]\n";
			continue;
		}

		// 非阻塞 IO，表示数据全部读取完毕
		else if (readLen == -1 and ((errno == EAGAIN) or (errno == EWOULDBLOCK)))
		{
			break;
		}

		// EOF，表示客户端断开连接
		else if (readLen == 0)
		{
			std::cout << "[Client " << sockfd << " ]\tdisconnected\n";
			mState = State::Closed;
			break;
		}

		else
		{
			std::cout << "[Error]\tOther error on client fd: " << sockfd << "\n";
			mState = State::Closed;
			break;
		}
	}
}

void Connection::readBlocking()
{
	int sockfd = mSocket->getFd();
	unsigned int rcvSize = 0;
	socklen_t len = sizeof(rcvSize);

	// 获取一个套接字的选项
	// int getsockopt(int socket, int level, int optionName, void *optVal, socklen_t *optLen);
	// int socket:			套接字
	// int level:			选项所在的层，如有 SOL_SOCKET、IPPROTO_TCP 等
	// int optionName:		需要获取的套接字选项名
	// void *optVal:		指向存放选项值的空间
	// socklen_t *optLen:	指向选项值空间的长度
	getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvSize, &len);

	char *buf = new char[rcvSize];
	bzero(buf, rcvSize);

	// 从套接字读取数据到缓冲区
	ssize_t readLen = ::read(sockfd, buf, rcvSize);

	if (readLen > 0)
	{
		mReadBuffer->append(buf, readLen);
	}

	// 客户端断开连接
	else if (readLen == 0)
	{
		std::cout << "[Client " << sockfd << " ]\tdisconnected\n";
		mState = State::Closed;
	}

	else if (readLen == -1)
	{
		std::cout << "[Error]\tOther error on client fd: " << sockfd << "\n";
		mState = State::Closed;
	}

	delete[] buf;
}

void Connection::writeNonBlocking()
{
	int sockfd = mSocket->getFd();
	char *buf = new char[mSendBuffer->size()];
	memcpy(buf, mSendBuffer->c_str(), mSendBuffer->size());
	int dataSize = mSendBuffer->size();
	int dataLeft = dataSize;

	while (dataLeft > 0)
	{
		ssize_t writeLen = ::write(sockfd, buf + dataSize - dataLeft, dataLeft);

		if (writeLen == -1 and errno == EINTR)
		{
			std::cout << "[Continue Writing]\n";
			continue;
		}

		if (writeLen == -1 and errno == EAGAIN)
			break;

		if (writeLen == -1)
		{
			std::cout << "[Error]\tOther error on client fd: " << sockfd << "\n";
			mState = State::Closed;
			break;
		}
		dataLeft -= writeLen;
	}

	delete[] buf;
}

void Connection::writeBlocking()
{
	// 没有处理 mSendBuffer 数据大于TCP写缓冲区的情况，可能会有bug
	int sockfd = mSocket->getFd();
	ssize_t writeLen = ::write(sockfd, mSendBuffer->c_str(), mSendBuffer->size());

	if (writeLen == -1)
	{
		std::cout << "[Error]\tOther error on client fd: " << sockfd << "\n";
		mState = State::Closed;
	}
}