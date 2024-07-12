#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include <unistd.h>

Channel::Channel(EventLoop *loop, int fd)
        : mFd(fd)
        , mLoop(loop)
        , mListenEvents(0)
        , mReadyEvents(0)
        , inEpoll(false) {}

Channel::~Channel() {
    if (mFd != -1) {
        close(mFd);
        mFd = -1;
    }
}

void Channel::handleEvent() const {
    if (mReadyEvents & (EPOLLIN or EPOLLPRI or EPOLLRDHUP))
        if (readCallback) {
            readCallback();
        }

    if (mReadyEvents & EPOLLOUT)
        if (writeCallback) {
            writeCallback();
        }
}

void Channel::enableRead() {
    mListenEvents |= (EPOLLIN or EPOLLPRI);
    mLoop->updateChannel(this);
}

void Channel::enableWrite() {
    mListenEvents |= EPOLLOUT;
    mLoop->updateChannel(this);
}

void Channel::useET() {
    mListenEvents |= EPOLLET;
    mLoop->updateChannel(this);
}

int Channel::getFd() const {
    return mFd;
}

uint32_t Channel::getListenEvents() const {
    return mListenEvents;
}

uint32_t Channel::getReadyEvents() const {
    return mReadyEvents;
}

void Channel::setReadyEvents(uint32_t events) {
    mReadyEvents = events;
}

bool Channel::isInEpoll() const {
    return inEpoll;
}

void Channel::setInEpoll(bool in) {
    inEpoll = in;
}

void Channel::setReadCallback(std::function<void()> const &callback) {
    readCallback = std::move(callback);
}

void Channel::setWriteCallback(std::function<void()> const &callback) {
    writeCallback = std::move(callback);
}