#include "Channel.h"
#include "Epoll.h"
#include "util.h"
#include <unistd.h>
#include <cstring>

constexpr int maxEvents = 1024;

Epoll::Epoll() : epfd(-1), events(nullptr)
{
	epfd = epoll_create1(0);
	errorif(epfd == -1, "epoll create error");
	events = new epoll_event[maxEvents];
	bzero(events, sizeof(epoll_event) * maxEvents);
}

Epoll::~Epoll()
{
	if (epfd != -1)
	{
		close(epfd);
		epfd = -1;
	}
	delete[] events;
}

vector<Channel *> Epoll::poll(int timeout)
{
	vector<Channel *> activeEvents;
	int nfds = epoll_wait(epfd, events, maxEvents, timeout);
	errorif(nfds == -1, "epoll wait error");
	for (int i = 0; i < nfds; ++i)
	{
		Channel *channel = (Channel *)events[i].data.ptr;
		channel->setRevents(events[i].events);
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
		errorif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
		channel->setInEpoll();
	}
	else
	{
		errorif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll update error");
	}
}