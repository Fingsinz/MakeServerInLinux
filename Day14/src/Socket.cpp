#include "Socket.h"
#include "util.h"
#include <arpa/inet.h>
#include <cassert>
#include <cstring>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

Socket::Socket()
        : mFd(-1) {}

Socket::~Socket() {
    if (mFd != -1) {
        close(mFd);
        mFd = -1;
    }
}

void Socket::setFd(int fd) {
    mFd = fd;
}

int Socket::getFd() const {
    return mFd;
}

std::string Socket::getAddr() const {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    socklen_t len = sizeof(addr);

    if (getpeername(mFd, (struct sockaddr *)&addr, &len) == -1)
        return "";

    std::string ret(inet_ntoa(addr.sin_addr));
    ret += ":";
    ret += std::to_string(ntohs(addr.sin_port));
    return ret;
}

FLAG Socket::socketCreate() {
    assert(mFd == -1);
    mFd = socket(AF_INET, SOCK_STREAM, 0);
    if (mFd == -1) {
        perror("[Error]\tSocket create failed");
        return FL_SOCKET_ERROR;
    }
    return FL_SUCCESS;
}

FLAG Socket::socketBind(const char *ip, uint16_t port) const {
    assert(mFd != -1);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    if (::bind(mFd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("[Error]\tSocket bind failed");
        return FL_SOCKET_ERROR;
    }
    return FL_SUCCESS;
}

FLAG Socket::socketListen() const {
    assert(mFd != -1);
    if (::listen(mFd, SOMAXCONN) == -1) {
        perror("[Error]\tSocket listen failed");
        return FL_SOCKET_ERROR;
    }
    return FL_SUCCESS;
}

FLAG Socket::socketAccept(int &clientFd) const {
    assert(mFd != -1);
    clientFd = ::accept(mFd, NULL, NULL);
    if (clientFd == -1) {
        perror("[Error]\tSocket accept failed");
        return FL_SOCKET_ERROR;
    }
    return FL_SUCCESS;
}

FLAG Socket::socketConnect(const char *ip, uint16_t port) const {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    if (::connect(mFd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("[Error]\tSocket connect failed");
        return FL_SOCKET_ERROR;
    }
    return FL_SUCCESS;
}

FLAG Socket::setNonBlocking() const {
    if (fcntl(mFd, F_SETFL, fcntl(mFd, F_GETFL) | O_NONBLOCK) == -1) {
        perror("[Error]\tSocket set non-blocking failed");
        return FL_SOCKET_ERROR;
    }
    return FL_SUCCESS;
}

bool Socket::isNonBlocking() const {
    return (fcntl(mFd, F_GETFL) & O_NONBLOCK) != 0;
}

size_t Socket::recvBufSize() const {
    size_t size = -1;
    if (ioctl(mFd, FIONREAD, &size) == -1)
        perror("[Error]\tSocket get recv buf size failed");
    return size;
}