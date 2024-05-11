#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include <unistd.h>

Channel::Channel(EventLoop *loop, Socket *socket) : mLoop(loop), mSocket(socket)
{}

Channel::~Channel()
{
	mLoop->deleteChannel(this);
}

void Channel::handleEvent()
{
	if (mReadyEvents & EPOLLIN)
		readCallback();

	if (mReadyEvents & EPOLLOUT)
		writeCallback();
}

void Channel::enableRead()
{
	mListenEvents |= EPOLLIN;
	mLoop->updateChannel(this);
}

void Channel::enableWrite()
{
	mListenEvents |= EPOLLOUT;
	mLoop->updateChannel(this);
}

void Channel::useET()
{
	mListenEvents |= EPOLLET;
	mLoop->updateChannel(this);
}

Socket *Channel::getSocket() const
{
	return mSocket;
}

uint32_t Channel::getListenEvents()
{
	return mListenEvents;
}

uint32_t Channel::getReadyEvents()
{
	return mReadyEvents;
}

void Channel::setReadyEvents(uint32_t events)
{
	if (events & EPOLLIN)
		mReadyEvents |= EPOLLIN;
	if (events & EPOLLOUT)
		mReadyEvents |= EPOLLOUT;
	if (events & EPOLLET)
		mReadyEvents |= EPOLLET;
}

bool Channel::getExist() const
{
	return exist;
}

void Channel::setExist(bool _exist)
{
	exist = _exist;
}

void Channel::setReadCallback(std::function<void()> const &callback)
{
	readCallback = callback;
}