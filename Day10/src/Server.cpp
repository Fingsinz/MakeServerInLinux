#include "Acceptor.h"
#include "Server.h"
#include "Socket.h"
#include "Connection.h"
#include <functional>
#include <unistd.h>

constexpr int READ_BUFFER = 1024;

Server::Server(EventLoop *_loop) : loop(_loop), acceptor(nullptr)
{
	// 使用给定的事件循环创建一个新的Acceptor对象
	acceptor = new Acceptor(loop);

	// 定义一个回调函数cb，它接受一个Socket指针作为参数
	std::function<void(Socket *)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);

	// 将acceptor的新连接回调设置为已定义的回调函数
	acceptor->setNewConnectionCallback(cb);
}

Server::~Server()
{
	delete acceptor;
}

void Server::handleReadEvent(int fd)
{}

void Server::newConnection(Socket *_socket)
{
	Connection *conn = new Connection(loop, _socket);
	std::function<void(int)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
	conn->setDeleteConnectionCallback(cb);
	connections[_socket->getFd()] = conn;
}

void Server::deleteConnection(int sockfd)
{
	if (sockfd != -1)
	{
		auto it = connections.find(sockfd);
		if (it != connections.end())
		{
			Connection *conn = connections[sockfd];
			connections.erase(sockfd);
			close(sockfd);
			// delete conn;			// 会段错误
		}
	}
}