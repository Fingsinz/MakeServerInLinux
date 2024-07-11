#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Server.h"
#include <iostream>

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
	delete addr;
}

Acceptor::~Acceptor()
{
	delete sock;
	delete acceptChannel;
}

void Acceptor::acceptConnection()
{
	// 创建一个新的InetAddress对象来存储客户端地址信息
	InetAddress *clientAddr = new InetAddress();

	//通过使用客户端地址接受来自服务器套接字的连接，创建一个新的Socket对象
	Socket *clientSock = new Socket(sock->accept(clientAddr));

	// 打印有关新客户端连接的信息
	std::cout << "[New client " << clientSock->getFd() << " ]:\t(" <<
		inet_ntoa(clientAddr->getAddr().sin_addr) << " : " << ntohs(clientAddr->getAddr().sin_port) << ")\n";

	clientSock->setNonBlocking();

	newConnectionCallback(clientSock);
	delete clientAddr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket *)> _callback)
{
	// 设置新的连接回调函数
	newConnectionCallback = _callback;
}