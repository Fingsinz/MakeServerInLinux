#include "Acceptor.h"
#include "EventLoop.h"
#include "Server.h"
#include "Socket.h"
#include "Connection.h"
#include "ThreadPool.h"
#include <functional>
#include <unistd.h>

constexpr int READ_BUFFER = 1024;

Server::Server(EventLoop *_loop) : mainReactor(_loop), acceptor(nullptr)
{
	// 使用给定的事件循环创建一个新的Acceptor对象
	acceptor = new Acceptor(mainReactor);

	// 定义一个回调函数cb，它接受一个Socket指针作为参数
	std::function<void(Socket *)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);

	// 将acceptor的新连接回调设置为已定义的回调函数
	acceptor->setNewConnectionCallback(cb);

	int size = std::thread::hardware_concurrency();
	threadPool = new ThreadPool(size);
	for (int i = 0; i < size; ++ i)
		subReactors.emplace_back(new EventLoop());

	for (int i = 0; i < size; ++ i)
	{
		std::function<void()> subLoop = std::bind(&EventLoop::loop, subReactors[i]);
		threadPool->add(subLoop);
	}
}

Server::~Server()
{
	delete acceptor;
	delete threadPool;
}

void Server::handleReadEvent(int fd)
{}

void Server::newConnection(Socket *_socket)
{
	if (_socket->getFd() != -1)
	{
		int random = _socket->getFd() % subReactors.size();
		Connection *conn = new Connection(subReactors[random], _socket);
		std::function<void(int)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
		conn->setDeleteConnectionCallback(cb);
		connections[_socket->getFd()] = conn;
	}
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