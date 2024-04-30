#include "Channel.h"
#include "Epoll.h"
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
		channel->setReady(mEvents[i].events);
		activeEvents.push_back(channel);
	}

	return activeEvents;
}

void Epoll::updateChannel(Channel *channel)
{
	int fd = channel->getFd();
	struct epoll_event ev;
	bzero(&ev, sizeof(ev));
	ev.data.ptr = channel;
	ev.events = channel->getEvents();
	if (!channel->getInEpoll())
	{
		errorif(epoll_ctl(mEpFd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
		channel->setInEpoll();
	}
	else
	{
		errorif(epoll_ctl(mEpFd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll update error");
	}
}

void Epoll::deleteChannel(Channel *channel)
{
	int fd = channel->getFd();
	errorif(epoll_ctl(mEpFd, EPOLL_CTL_DEL, fd, nullptr) == -1, "epoll delete error");
	channel->setInEpoll(false);
}