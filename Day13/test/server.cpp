#include <iostream>
#include "Buffer.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Server.h"
#include "Socket.h"

int main()
{
	EventLoop *loop = new EventLoop();
	Server *server = new Server(loop);
	
	server->onConnect([] (Connection *conn)
		{
			// 从客户端读取数据
			conn->read();
			if(conn->getState() == Connection::State::Closed)
			{
				conn->close();
				return;
			}
			std::cout << "[Received from " << conn->getSocket()->getFd() << "]\t" << conn->readBuffer() << std::endl;
			conn->setSentBuffer(conn->readBuffer());
			// 回写数据
			conn->write();
		});

	loop->loop();

	delete server;
	delete loop;
	return 0;
}