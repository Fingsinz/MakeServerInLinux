#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Server.h"

Acceptor::Acceptor(EventLoop *_loop) : loop(_loop)
{
	// 创建一个Socket对象并设置地址
	sock = new Socket();
	addr = new InetAddress("127.0.0.1", 1234);
	sock->bind(addr);

	// 监听传入的连接并将套接字设置为非阻塞模式
	sock->listen();
	sock->setNonBlocking();

	// 创建一个接受连接的新Channel
	acceptChannel = new Channel(loop, sock->getFd());

	// 定义一个用于接受Channel的回调函数，并设置
	std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
	acceptChannel->setCallback(cb);

	// 在接受通道上启用读事件
	acceptChannel->enableReading();
}

Acceptor::~Acceptor()
{
	delete sock;
	delete addr;
	delete acceptChannel;
}

void Acceptor::acceptConnection()
{
	// 新建连接时，调用回调函数
	newConnectionCallback(sock);
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket *)> _callback)
{
	// 设置新的连接回调函数
	newConnectionCallback = _callback;
}