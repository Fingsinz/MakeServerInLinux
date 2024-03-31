#include "EventLoop.h"
#include <vector>

EventLoop::EventLoop() : ep(nullptr), quit(false)
{
	ep = new Epoll();
}

EventLoop::~EventLoop()
{
	delete ep;
}

void EventLoop::loop()
{
	while (!quit)
	{
		// ʹ��epollʵ����ѯ�¼�
		std::vector<Channel *> channels = ep->poll();

		// ����ÿ��Channel���¼�
		for (Channel *channel : channels)
			channel->handleEvent();
	}
}

void EventLoop::updateChannel(Channel *channel)
{
	ep->updateChannel(channel);
}