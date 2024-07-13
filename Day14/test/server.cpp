#include "Server.h"
#include "Buffer.h"
#include "Connection.h"
#include "Socket.h"
#include <iostream>

int main() {
    Server *server = new Server("127.0.0.1", 1234);

    server->setMessageCallback([](Connection *conn) {
        std::cout << "Message from client " << conn->getId() << " : " << conn->getReadBuf()->c_str() << std::endl;
        conn->send(conn->getReadBuf()->c_str());
    });

	server->start();

	delete server;
    return 0;
}