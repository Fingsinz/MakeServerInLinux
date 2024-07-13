#include "Server.h"
#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "util.h"
#include <cassert>
#include <functional>
#include <iostream>
#include <unistd.h>

constexpr int READ_BUFFER = 1024;

Server::Server(std::string const &ip, int const port)
        : nextConnId(1) {
    mainReactor = std::make_unique<EventLoop>();
    acceptor = std::make_unique<Acceptor>(mainReactor.get(), ip, port);
    std::function<void(int)> cb = std::bind(&Server::handleNewConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);

    unsigned int size = std::thread::hardware_concurrency();
    threadPool = std::make_unique<ThreadPool>(size);

    for (size_t i = 0; i < size; ++i) {
        std::unique_ptr<EventLoop> subReactor = std::make_unique<EventLoop>();
        subReactors.emplace_back(std::move(subReactor));
    }
}

Server::~Server() {}

void Server::start() {
    for (size_t i = 0; i < subReactors.size(); ++i) {
        std::function<void()> subLoop = std::bind(&EventLoop::loop, subReactors[i].get());
        threadPool->add(subLoop);
    }
    mainReactor->loop();
}

void Server::setConnectionCallback(std::function<void(Connection *)> const &cb) {
    onConnectCallback = std::move(cb);
}

void Server::setMessageCallback(std::function<void(Connection *)> const &cb) {
    onMessageCallback = std::move(cb);
}

void Server::handleNewConnection(int fd) {
    assert(fd != -1);
    std::cout << "[INFO]\tNew Connection, fd: " << fd << std::endl;

    uint64_t random = fd % subReactors.size();

    Connection *conn = new Connection(subReactors[random].get(), fd, nextConnId);
    std::function<void(int)> cb = std::bind(&Server::handleClose, this, std::placeholders::_1);

    conn->setOnCloseCallback(cb);
    conn->setOnMessageCallback(onMessageCallback);

    connections[fd] = conn;

    ++nextConnId;
    if (nextConnId == 1000) {
        nextConnId = 1;
    }
}

void Server::handleClose(int fd) {
	auto it = connections.find(fd);
	if (it != connections.end()) {
		// delete it->second;
		// 没有析构，所以在这里进行了 close，先关闭连接
		::close(fd);
		connections.erase(it);
	}
}