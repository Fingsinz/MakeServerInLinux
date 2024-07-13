#include "Buffer.h"
#include "Connection.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(fd, (struct sockaddr *)&addr, sizeof(addr));

    Connection *conn = new Connection(nullptr, fd, 1);

    while (true) {
        std::string input;
        std::getline(std::cin, input);
        conn->setSentBuf(input.c_str());
        conn->write();
        if (conn->getState() == Connection::State::Disconnected) {
            conn->handleClose();
            break;
        }
        conn->read();
        std::cout << "[Received]\t" << conn->getReadBuf()->c_str() << std::endl;
    }

    delete conn;
    return 0;
}