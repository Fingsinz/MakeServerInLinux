#include <iostream>
#include "Buffer.h"
#include "Connection.h"
#include "Server.h"
#include "Socket.h"

int main()
{
	Server *server = new Server();

	server->onConnect([] (Connection *conn) {
		// 新连接
		std::cout << "[New Client " << conn->getSocket()->getFd() << " ]:\t\n";
		});

	server->onMessage([] (Connection *conn)
		{
			// Echo 服务器的业务
			std::cout << "[Received from " << conn->getSocket()->getFd() << "]\t" << conn->getReadBuffer()->c_str() << std::endl;
			if (conn->getState() == Connection::State::Connected)
				conn->send(conn->getReadBuffer()->c_str());
		});

	server->start();

	delete server;
	return 0;
}