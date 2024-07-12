#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "util.h"
#include <arpa/inet.h>
#include <cassert>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

Acceptor::Acceptor(EventLoop *loop, std::string const &ip, int const port)
        : mLoop(loop)
        , mListenFd(-1) {
    socketCreate();
    socketBind(ip, port);
    socketListen();

    mChannel = std::make_unique<Channel>(loop, mListenFd);
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);

    mChannel->setReadCallback(cb);
    mChannel->enableRead();
}

Acceptor::~Acceptor() {
    mLoop->deleteChannel(mChannel.get());
    ::close(mListenFd);
}

void Acceptor::socketCreate() {
    assert(mListenFd == -1);
    mListenFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    errorif(mListenFd == -1, "[Error]\t Socket Create failed");
}

void Acceptor::socketBind(std::string const &ip, int const port) {
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);
    if (::bind(mListenFd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        errorif(true, "[Error]\t Socket Bind failed");
    }
}

void Acceptor::socketListen() {
    assert(mListenFd != -1);
    if (::listen(mListenFd, SOMAXCONN) == -1) {
        errorif(true, "[Error]\t Socket Listen failed");
    }
}

FLAG Acceptor::acceptConnection() const {
    assert(mListenFd != -1);

    struct sockaddr_in client;
    socklen_t len = sizeof(client);

    int clientFd = ::accept4(mListenFd, (struct sockaddr *)&client, &len, SOCK_NONBLOCK | SOCK_CLOEXEC);

    errorif(clientFd == -1, "[Error]\t Accept failed");

    if (mNewConnectionCallback)
        mNewConnectionCallback(clientFd);

    return FL_SUCCESS;
}

void Acceptor::setNewConnectionCallback(std::function<void(int)> const &callback) {
    // 设置新的连接回调函数
    mNewConnectionCallback = std::move(callback);
}