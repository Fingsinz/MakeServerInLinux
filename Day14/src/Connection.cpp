#include "Connection.h"
#include "Buffer.h"
#include "Channel.h"
#include "Socket.h"
#include "util.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <Common.h>

Connection::Connection(EventLoop *loop, int fd) {
    mSocket = std::make_unique<Socket>();
    mSocket->setFd(fd);

    if (loop != nullptr) {
        mChannel = std::make_unique<Channel>(loop, fd);
        mChannel->useET();
        mChannel->enableRead();
    }
    mReadBuffer = std::make_unique<Buffer>();
    mSendBuffer = std::make_unique<Buffer>();

    mState = State::Connected;
}

Connection::~Connection() {}

FLAG Connection::read() {
    if (mState != State::Connected) {
        errorif(true, "[Error]\tConnection state is disconnected!");
        return FLAG::FL_CONNECTION_ERROR;
    }
    assert(mState == State::Connected and "[Error]\tConnection state is disconnected!");

    mReadBuffer->clear();
    if (mSocket->isNonBlocking())
        return readNonBlocking();
    else
        return readBlocking();
}

FLAG Connection::write() {
    if (mState != State::Connected) {
        errorif(true, "[Error]\tConnection state is disconnected!");
        return FLAG::FL_CONNECTION_ERROR;
    }
    FLAG flag = FLAG::FL_UNDIFINED;

    if (mSocket->isNonBlocking())
        flag = writeNonBlocking();
    else
        flag = writeBlocking();

    mSendBuffer->clear();
    return flag;
}

FLAG Connection::send(std::string msg) {
    setSentBuffer(msg.c_str());
    write();
    return FLAG::FL_SUCCESS;
}

void Connection::setDeleteConnectionCallback(std::function<void(int)> const &callback) {
    mDeleteConnectionCallback = std::move(callback);
}

void Connection::setOnMessageCallback(std::function<void(Connection *)> const &callback) {
    mOnMessageCallback = std::move(callback);
    std::function<void()> busi = std::bind(&Connection::business, this);
    mChannel->setReadCallback(busi);
}

void Connection::business() {
    read();
    mOnMessageCallback(this);
}

Connection::State Connection::getState() {
    return mState;
}

void Connection::close() {
    mDeleteConnectionCallback(mSocket->getFd());
}

void Connection::setSentBuffer(char const *str) {
    mSendBuffer->setBuf(str);
}

Buffer *Connection::getReadBuffer() {
    return mReadBuffer.get();
}

Buffer *Connection::getSendBuffer() {
    return mSendBuffer.get();
}

Socket *Connection::getSocket() {
    return mSocket.get();
}

FLAG Connection::readNonBlocking() {
    int sockfd = mSocket->getFd();
    char buf[1024];

    while (true) {
        bzero(buf, sizeof(buf));
        ssize_t readLen = ::read(sockfd, buf, sizeof(buf));

        if (readLen > 0)
            mReadBuffer->append(buf, readLen);

        // 客户端正常中断、继续读取
        else if (readLen == -1 and errno == EINTR) {
            std::cout << "[Continue Reading]\n";
            continue;
        }

        // 非阻塞 IO，表示数据全部读取完毕
        else if (readLen == -1 and ((errno == EAGAIN) or (errno == EWOULDBLOCK))) {
            break;
        }

        // EOF，表示客户端断开连接
        else if (readLen == 0) {
            std::cout << "[Client " << sockfd << " disconnected]\n";
            mState = State::Closed;
            // close();	// 启用这句会段错误
            break;
        }

        else {
            std::cout << "[Error]\tOther error on client fd: " << sockfd << "\n";
            mState = State::Closed;
            close();
            break;
        }
    }
    return FLAG::FL_SUCCESS;
}

FLAG Connection::readBlocking() {
    int sockfd = mSocket->getFd();
    ssize_t dataSize = mSocket->recvBufSize();
    char buf[1024];
    ssize_t readLen = ::read(sockfd, buf, sizeof(buf));

    if (readLen > 0) {
        mReadBuffer->append(buf, readLen);
    }

    // 客户端断开连接
    else if (readLen == 0) {
        std::cout << "[Client " << sockfd << " ]\tdisconnected\n";
        mState = State::Closed;
    }

    else if (readLen == -1) {
        std::cout << "[Error]\tOther error on client fd: " << sockfd << "\n";
        mState = State::Closed;
    }

    return FLAG::FL_SUCCESS;
}

FLAG Connection::writeNonBlocking() {
    int sockfd = mSocket->getFd();
    char *buf = new char[mSendBuffer->size()];
    memcpy(buf, mSendBuffer->c_str(), mSendBuffer->size());
    int dataSize = mSendBuffer->size();
    int dataLeft = dataSize;

    while (dataLeft > 0) {
        ssize_t writeLen = ::write(sockfd, buf + dataSize - dataLeft, dataLeft);

        if (writeLen == -1 and errno == EINTR) {
            std::cout << "[Continue Writing]\n";
            continue;
        }

        if (writeLen == -1 and errno == EAGAIN)
            break;

        if (writeLen == -1) {
            std::cout << "[Error]\tOther error on client fd: " << sockfd << "\n";
            mState = State::Closed;
            break;
        }
        dataLeft -= writeLen;
    }

    delete[] buf;
    return FLAG::FL_SUCCESS;
}

FLAG Connection::writeBlocking() {
    // 没有处理 mSendBuffer 数据大于TCP写缓冲区的情况，可能会有bug
    int sockfd = mSocket->getFd();
    ssize_t writeLen = ::write(sockfd, mSendBuffer->c_str(), mSendBuffer->size());

    if (writeLen == -1) {
        std::cout << "[Error]\tOther error on client fd: " << sockfd << "\n";
        mState = State::Closed;
    }
    return FLAG::FL_SUCCESS;
}