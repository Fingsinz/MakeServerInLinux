#include <iostream>
#include "InetAddress.h"
#include "Socket.h"
#include "Connection.h"

int main()
{
	Socket *sock = new Socket();
	InetAddress *addr = new InetAddress("127.0.0.1", 1234);
	sock->connect(addr);

	Connection *conn = new Connection(nullptr, sock);

	while (true)
	{
		conn->getlineSendBuffer();
		conn->write();
		if (conn->getState() == Connection::State::Closed)
		{
			conn->close();
			break;
		}
		conn->read();
		std::cout << "[Received]\t"  << conn->readBuffer() << std::endl;
	}

	delete conn;
	delete sock;
	delete addr;
	return 0;
}