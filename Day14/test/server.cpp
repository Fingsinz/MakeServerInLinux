#include <iostream>
#include <arpa/inet.h>
#include "Buffer.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Server.h"
#include "Socket.h"

int main()
{
	EventLoop *loop = new EventLoop();
	Server *server = new Server(loop);

	server->newConnect([] (Connection *conn) {
		// 新连接
		std::cout << "[New Client " << conn->getSocket()->getFd() << " ]:\t\n";
		});

	server->onMessage([] (Connection *conn)
		{
			// Echo 服务器的业务
			std::cout << "[Received from " << conn->getSocket()->getFd() << "]\t" << conn->readBuffer() << std::endl;
			if (conn->getState() == Connection::State::Connected)
				conn->send(conn->readBuffer());
		});

	loop->loop();

	delete server;
	delete loop;
	return 0;
}