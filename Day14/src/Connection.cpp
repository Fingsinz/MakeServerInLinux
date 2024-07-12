#include "Connection.h"
#include "Buffer.h"
#include "Channel.h"
#include "Socket.h"
#include "util.h"
#include <Common.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <unistd.h>

Connection::Connection(EventLoop *loop, int fd, int id)
        : mConnfd(fd)
        , mConnId(id)
        , mLoop(loop) {
    if (loop != nullptr) {
        mChannel = std::make_unique<Channel>(mConnfd, loop);
        mChannel->useET();
        mChannel->setReadCallback(std::bind(&Connection::handleMessage, this));
        mChannel->enableRead();
    }
    mReadBuf = std::make_unique<Buffer>();
    mSendBuf = std::make_unique<Buffer>();
}
Connection::~Connection() {
    ::close(mConnfd);
}

FLAG Connection::read() {
    mReadBuf->clear();
    readNonBlocking();
}

FLAG Connection::write() {
    writeNonBlocking();
    mSendBuf->clear();
}

FLAG Connection::send(std::string msg) {
    setSentBuf(msg.c_str());
    write();
    return FL_SUCCESS;
}

FLAG Connection::sent(char const *msg) {
    setSentBuf(msg);
    write();
}

void Connection::setOnMessageCallback(std::function<void(Connection *)> const &callback) {
    onMessageCb = std::move(callback);
}

void Connection::setOnCloseCallback(std::function<void(int)> const &callback) {
    onCloseCb = std::move(callback);
}

void Connection::setSentBuf(char const *str) {
    mSendBuf->setBuf(str);
}

Buffer *Connection::getReadBuf() {
    return mReadBuf.get();
}

Buffer *Connection::getSentBuf() {
    return mSendBuf.get();
}

void Connection::handleMessage() {
    read();
    if (onCloseCb) {
        onMessageCb(this);
    }
}

void Connection::handleClose() {
    if (mState != State::Disconnected) {
        mState = State::Disconnected;
        if (onCloseCb) {
            onCloseCb(mConnId);
        }
    }
}

Connection::State Connection::getState() const {
    return mState;
}

EventLoop *Connection::getLoop() const {
    return mLoop;
}

int Connection::getFd() const {
    return mConnfd;
}

int Connection::getId() const {
    return mConnId;
}

void Connection::readNonBlocking() {
    char buf[1026];
    while (true) {
        memset(buf, 0, sizeof(buf));
        ssize_t bytesRead = ::read(mConnfd, buf, sizeof(buf));

        if (bytesRead > 0) {
            mReadBuf->append(buf, bytesRead);
        } else if (bytesRead == -1 and errno == EINTR) {
            continue;
        } else if (bytesRead == -1 and (errno == EAGAIN or errno == EWOULDBLOCK)) {
            break;
        } else if (bytesRead == 0) {
            handleClose();
            break;
        } else {
            handleClose();
            break;
        }
    }
}

void Connection::writeNonBlocking() {
    char buf[mSendBuf->size()];

    memcpy(buf, mSendBuf->c_str(), mSendBuf->size());

    int dataSize = mSendBuf->size();
    int dataLeft = dataSize;

    while (dataLeft > 0) {
        ssize_t bytesWrite = ::write(mConnfd, buf + dataSize - dataLeft, dataLeft);

        if (bytesWrite == -1 and errno == EINTR) {
            continue;
        } else if (bytesWrite == -1 and errno == EAGAIN) {
            break;
        } else if (bytesWrite == -1) {
            handleClose();
            break;
        } else {
            dataLeft -= bytesWrite;
        }
    }
}