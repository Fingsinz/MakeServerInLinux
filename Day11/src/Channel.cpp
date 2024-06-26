﻿#include "Channel.h"
#include "EventLoop.h"
#include <unistd.h>

Channel::Channel(EventLoop *_loop, int _fd) : loop(_loop), fd(_fd),
events(0), ready(0), inEpoll(false)
{}

Channel::~Channel()
{
	if (fd != -1)
	{
		close(fd);
		fd = -1;
	}
}

void Channel::handleEvent()
{
	if (ready & (EPOLLIN | EPOLLPRI))
		readCallback();

	if (ready & EPOLLOUT)
		writeCallback();
}

void Channel::enableReading()
{
	events |= EPOLLIN | EPOLLPRI;
	loop->updateChannel(this);
}

int Channel::getFd()
{
	return fd;
}

uint32_t Channel::getEvents()
{
	return events;
}

uint32_t Channel::getReady()
{
	return ready;
}

bool Channel::getInEpoll()
{
	return inEpoll;
}

void Channel::setInEpoll(bool _in)
{
	inEpoll = _in;
}

void Channel::useET()
{
	events |= EPOLLET;
	loop->updateChannel(this);
}

void Channel::setReady(uint32_t _ready)
{
	ready = _ready;
}

void Channel::setReadCallback(std::function<void()> _callback)
{
	readCallback = _callback;
}