#include "Channel.h"
#include "Epoll.h"
#include "Socket.h"
#include "util.h"
#include <unistd.h>
#include <cstring>

constexpr int maxEvents = 1024;

Epoll::Epoll()
{
	mEpFd = epoll_create1(0);
	errorif(mEpFd == -1, "[Error]\tEpoll create error");
	mEvents = new epoll_event[maxEvents];
	bzero(mEvents, sizeof(epoll_event) * maxEvents);
}

Epoll::~Epoll()
{
	if (mEpFd != -1)
		close(mEpFd);

	delete[] mEvents;
}

std::vector<Channel *> Epoll::poll(int timeout) const
{
	std::vector<Channel *> activeEvents;
	int nfds = epoll_wait(mEpFd, mEvents, maxEvents, timeout);
	errorif(nfds == -1, "[Error]\tEpoll wait error");

	for (int i = 0; i < nfds; ++i)
	{
		Channel *channel = (Channel *)mEvents[i].data.ptr;
		int events = mEvents[i].events;

		if (events & EPOLLIN)
			channel->setReadyEvents(EPOLLIN);

		if (events & EPOLLOUT)
			channel->setReadyEvents(EPOLLOUT);

		if (events & EPOLLET)
			channel->setReadyEvents(EPOLLET);

		activeEvents.push_back(channel);
	}

	return activeEvents;
}

FLAG Epoll::updateChannel(Channel *channel) const
{
	int fd = channel->getFd();
	struct epoll_event ev;
	bzero(&ev, sizeof(ev));
	ev.data.ptr = channel;

	if (channel->getListenEvents() & EPOLLIN)
		ev.events |= EPOLLIN | EPOLLPRI;

	if (channel->getListenEvents() & EPOLLOUT)
		ev.events |= EPOLLOUT;

	if (channel->getListenEvents() & EPOLLET)
		ev.events |= EPOLLET;

	if (!channel->getExist())
	{
		errorif(epoll_ctl(mEpFd, EPOLL_CTL_ADD, fd, &ev) == -1, "[Error]\tEpoll add error");
		channel->setExist(true);
	}
	else
		errorif(epoll_ctl(mEpFd, EPOLL_CTL_MOD, fd, &ev) == -1, "[Error]\tEpoll update error");

	return FL_SUCCESS;
}

FLAG Epoll::deleteChannel(Channel *channel) const
{
	int fd = channel->getFd();
	errorif(epoll_ctl(mEpFd, EPOLL_CTL_DEL, fd, nullptr) == -1, "[Error]\tEpoll delete error");
	channel->setExist(false);
	return FL_SUCCESS;
}