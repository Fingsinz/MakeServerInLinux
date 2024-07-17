#include "Buffer.h"
#include "Connection.h"
#include "Socket.h"
#include <iostream>


int main() {
    Socket *sock = new Socket();
    sock->socketCreate();
    sock->socketConnect("127.0.0.1", 1234);

    Connection *conn = new Connection(nullptr, sock->getFd());

    while (true) {
        std::string input;
        std::getline(std::cin, input);
        conn->setSentBuffer(input.c_str());
        conn->write();
        if (conn->getState() == Connection::State::Closed) {
            conn->close();
            break;
        }
        conn->read();
        std::cout << "[Received]\t" << conn->getReadBuffer()->c_str() << std::endl;
    }

    delete conn;
    delete sock;
    return 0;
}