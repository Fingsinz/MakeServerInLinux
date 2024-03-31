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
		// 使用epoll实例轮询事件
		std::vector<Channel *> channels = ep->poll();

		// 处理每个Channel的事件
		for (Channel *channel : channels)
			channel->handleEvent();
	}
}

void EventLoop::updateChannel(Channel *channel)
{
	ep->updateChannel(channel);
}