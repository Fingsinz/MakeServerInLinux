#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Server.h"
#include <iostream>

Acceptor::Acceptor(EventLoop *loop) : mLoop(loop), mSocket(nullptr), mAcceptChannel(nullptr)
{
	// 创建一个Socket对象并设置地址
	mSocket = new Socket();
	InetAddress *addr = new InetAddress("127.0.0.1", 1234);
	mSocket->bind(addr);

	// 监听传入的连接
	mSocket->listen();

	// 创建一个接受连接的新Channel
	mAcceptChannel = new Channel(mLoop, mSocket);

	// 定义一个用于接受Channel的回调函数，并设置
	std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
	mAcceptChannel->setReadCallback(cb);

	// 在接受通道上启用读事件
	mAcceptChannel->enableRead();

	delete addr;
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