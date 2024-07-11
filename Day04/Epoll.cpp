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

void Epoll::addFd(int fd, uint32_t op)
{
	struct epoll_event ev;
	bzero(&ev, sizeof(ev));
	ev.data.fd = fd;
	ev.events = op;
	errorif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add event error");
}

vector<epoll_event> Epoll::poll(int timeout)
{
	vector<epoll_event> activeEvents;
	int nfds = epoll_wait(epfd, events, maxEvents, timeout);
	for (int i = 0; i < nfds; ++i)
		activeEvents.push_back(events[i]);

	return activeEvents;
}