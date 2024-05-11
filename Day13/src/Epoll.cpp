#include "Channel.h"
#include "Epoll.h"
#include "Socket.h"
#include "util.h"
#include <unistd.h>
#include <cstring>

constexpr int maxEvents = 1024;

Epoll::Epoll() : mEpFd(-1), mEvents(nullptr)
{
	mEpFd = epoll_create1(0);
	errorif(mEpFd == -1, "epoll create error");
	mEvents = new epoll_event[maxEvents];
	bzero(mEvents, sizeof(epoll_event) * maxEvents);
}

Epoll::~Epoll()
{
	if (mEpFd != -1)
	{
		close(mEpFd);
		mEpFd = -1;
	}
	delete[] mEvents;
}

vector<Channel *> Epoll::poll(int timeout)
{
	vector<Channel *> activeEvents;
	int nfds = epoll_wait(mEpFd, mEvents, maxEvents, timeout);
	errorif(nfds == -1, "epoll wait error");

	for (int i = 0; i < nfds; ++i)
	{
		Channel *channel = (Channel *)mEvents[i].data.ptr;
		int events = mEvents[i].events;

		if(events & EPOLLIN)
			channel->setReadyEvents(EPOLLIN);

		if (events & EPOLLOUT)
			channel->setReadyEvents(EPOLLOUT);

		if(events & EPOLLET)
			channel->setReadyEvents(EPOLLET);

		activeEvents.push_back(channel);
	}

	return activeEvents;
}

void Epoll::updateChannel(Channel *channel)
{
	int fd = channel->getSocket()->getFd();
	struct epoll_event ev;
	bzero(&ev, sizeof(ev));
	ev.data.ptr = channel;
	
	if(channel->getListenEvents() & EPOLLIN)
		ev.events |= EPOLLIN | EPOLLPRI;

	if (channel->getListenEvents() & EPOLLOUT)
		ev.events |= EPOLLOUT;

	if (channel->getListenEvents() & EPOLLET)
		ev.events |= EPOLLET;
	
	if (!channel->getExist())
	{
		errorif(epoll_ctl(mEpFd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
		channel->setExist(true);
	}
	else
		errorif(epoll_ctl(mEpFd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll update error");
}

void Epoll::deleteChannel(Channel *channel)
{
	int fd = channel->getSocket()->getFd();
	errorif(epoll_ctl(mEpFd, EPOLL_CTL_DEL, fd, nullptr) == -1, "epoll delete error");
	channel->setExist(false);
}