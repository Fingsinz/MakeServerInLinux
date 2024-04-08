#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Server.h"

Acceptor::Acceptor(EventLoop *_loop) : loop(_loop)
{
	sock = new Socket();
	addr = new InetAddress("127.0.0.1", 1234);
	sock->bind(addr);
	sock->listen();
	sock->setNonBlocking();
	acceptChannel = new Channel(loop, sock->getFd());

	std::function<void()> cb = std::bind(&Acceptor::acceptChannel, this);
	acceptChannel->setCallback(cb);
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
	newConnectionCallback = _callback;
}