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

Server::Server()
{
	mainReactor = std::make_unique<EventLoop>();
	acceptor = std::make_unique<Acceptor>(mainReactor.get());
	std::function<void(int)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
	acceptor->setNewConnectionCallback(cb);

	unsigned int size = std::thread::hardware_concurrency();
	threadPool = std::make_unique<ThreadPool>(size);

	for (size_t i = 0; i < size; ++ i)
	{
		std::unique_ptr<EventLoop> subReactor = std::make_unique<EventLoop>();
		subReactors.emplace_back(std::move(subReactor));
	}
}

Server::~Server()
{}

void Server::start()
{
	for (size_t i = 0; i < subReactors.size(); ++ i)
	{
		std::function<void()> subLoop = std::bind(&EventLoop::loop, subReactors[i].get());
		threadPool->add(std::move(subLoop));
	}
	mainReactor->loop();
}

void Server::onConnect(std::function<void(Connection *)> fn)
{
	onConnectionCallback = std::move(fn);
}

void Server::onMessage(std::function<void(Connection *)> fn)
{
	onMessageCallback = std::move(fn);
}

FLAG Server::newConnection(int fd)
{
	errorif(fd == -1, "[Error]\t New Connection error");
	uint64_t random = fd % subReactors.size();
	std::unique_ptr<Connection> conn = std::make_unique<Connection>(subReactors[random].get(), fd);
	std::function<void(int)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);

	conn->setDeleteConnectionCallback(cb);
	conn->setOnMessageCallback(onMessageCallback);

	connections[fd] = std::move(conn);

	if (onConnectionCallback)
		onConnectionCallback(connections[fd].get());
	return FL_SUCCESS;
}

FLAG Server::deleteConnection(int fd)
{
	auto it = connections.find(fd);
	if (it != connections.end())
	{
		connections.erase(fd);
	}
	return FL_SUCCESS;
}