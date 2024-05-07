#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include <unistd.h>

Channel::Channel(EventLoop *loop, Socket *socket) : mLoop(loop), mSocket(socket),
mEvents(0), mReady(0), inEpoll(false)
{}

Channel::~Channel()
{
	if (mSocket->getFd() != -1)
	{
		close(mSocket->getFd());
	}
}

void Channel::handleEvent()
{
	if (mReady & (EPOLLIN | EPOLLPRI))
		readCallback();

	if (mReady & EPOLLOUT)
		writeCallback();
}

void Channel::enableReading()
{
	mEvents |= EPOLLIN | EPOLLPRI;
	mLoop->updateChannel(this);
}

Socket *Channel::getSocket()
{
	return mSocket;
}

uint32_t Channel::getEvents()
{
	return mEvents;
}

uint32_t Channel::getReady()
{
	return mReady;
}

bool Channel::getInEpoll()
{
	return inEpoll;
}

void Channel::setInEpoll(bool in)
{
	inEpoll = in;
}

void Channel::useET()
{
	mEvents |= EPOLLET;
	mLoop->updateChannel(this);
}

void Channel::setReady(uint32_t ready)
{
	mReady = ready;
}

void Channel::setReadCallback(std::function<void()> const &callback)
{
	readCallback = callback;
}