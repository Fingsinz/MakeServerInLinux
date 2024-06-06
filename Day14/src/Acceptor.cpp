#include "Acceptor.h"
#include "Socket.h"
#include "Channel.h"
#include "Server.h"
#include <fcntl.h>
#include <cassert>
#include <iostream>

Acceptor::Acceptor(EventLoop *loop)
{
	mSocket = std::make_unique<Socket>();
	assert(mSocket->socketCreate() == FL_SUCCESS);
	assert(mSocket->socketBind("127.0.0.1", 1234) == FL_SUCCESS);
	assert(mSocket->socketListen() == FL_SUCCESS);

	mChannel = std::make_unique<Channel>(loop, mSocket->getFd());
	std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);

	mChannel->setReadCallback(cb);
	mChannel->enableRead();
}

Acceptor::~Acceptor()
{}

FLAG Acceptor::acceptConnection() const
{
	int clientFd = -1;

	//通过使用客户端地址接受来自服务器套接字的连接，创建一个新的Socket对象
	if (mSocket->socketAccept(clientFd) != FL_SUCCESS)
		return FL_ACCEPTOR_ERROR;

	// 新接受的连接设置为非阻塞
	fcntl(clientFd, F_SETFL, fcntl(clientFd, F_GETFL) | O_NONBLOCK);

	if (mNewConnectionCallback)
		mNewConnectionCallback(clientFd);
	return FL_SUCCESS;
}

void Acceptor::setNewConnectionCallback(std::function<void(int)> const &callback)
{
	// 设置新的连接回调函数
	mNewConnectionCallback = std::move(callback);
}