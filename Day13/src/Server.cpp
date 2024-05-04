#include "Acceptor.h"
#include "EventLoop.h"
#include "Server.h"
#include "Socket.h"
#include "Connection.h"
#include "ThreadPool.h"
#include "util.h"
#include <functional>
#include <unistd.h>

constexpr int READ_BUFFER = 1024;

Server::Server(EventLoop *loop) : mainReactor(loop), acceptor(nullptr), threadPool(nullptr)
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

void Server::onConnect(std::function<void(Connection *)> fn)
{
	onConnectionCallback = std::move(fn);
}

void Server::newConnection(Socket *socket)
{
	errorif(socket == nullptr, "[Error]\t New Connection error");
	int random = socket->getFd() % subReactors.size();
	Connection *conn = new Connection(subReactors[random], socket);
	std::function<void(Socket *)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
	conn->setDeleteConnectionCallback(cb);
	conn->setOnConnectionCallback(onConnectionCallback);
	connections[socket->getFd()] = conn;
}

void Server::deleteConnection(Socket *sock)
{
	int sockfd = sock->getFd();
	auto it = connections.find(sockfd);
	if (it != connections.end())
	{
		Connection *conn = connections[sockfd];
		connections.erase(sockfd);
		delete conn;
		conn = nullptr;
	}
}