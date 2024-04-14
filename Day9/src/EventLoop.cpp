#include "Channel.h"
#include "Epoll.h"
#include "EventLoop.h"
#include "ThreadPool.h"
#include <vector>

EventLoop::EventLoop() : ep(nullptr), threadPool(nullptr), quit(false)
{
	ep = new Epoll();
	threadPool = new ThreadPool();
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

void EventLoop::addThread(std::function<void()> task)
{
	threadPool->add(task);
}