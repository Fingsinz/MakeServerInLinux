#include "Acceptor.h"
#include "Socket.h"
#include "Channel.h"
#include "Server.h"
#include <cassert>
#include <iostream>

Acceptor::Acceptor(EventLoop *loop)
{
	mSocket = std::make_unique<Socket>();
	assert(mSocket->socketCreate() == FL_SUCCESS);
	assert(mSocket->socketBind("127.0.0.1", 1234) == FL_SUCCESS);
	assert(mSocket->socketListen() == FL_SUCCESS);

	mChannel = std::make_unique<Channel>(loop, mSocket.get());
}

Acceptor::~Acceptor()
{
	delete mSocket;
	delete mAcceptChannel;
}

void Acceptor::acceptConnection()
{
	// 创建一个新的InetAddress对象来存储客户端地址信息
	InetAddress *clientAddr = new InetAddress();

	//通过使用客户端地址接受来自服务器套接字的连接，创建一个新的Socket对象
	Socket *clientSock = new Socket(mSocket->accept(clientAddr));

	clientSock->setNonBlocking();

	if (mNewConnectionCallback)
		mNewConnectionCallback(clientSock);
	delete clientAddr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket *)> const &callback)
{
	// 设置新的连接回调函数
	mNewConnectionCallback = callback;
}