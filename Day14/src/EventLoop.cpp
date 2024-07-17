#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"
#include <vector>


EventLoop::EventLoop() {
    ep = std::make_unique<Epoll>();
}

EventLoop::~EventLoop() {}

void EventLoop::loop() const {
    while (true) {
        // 使用epoll实例轮询事件
        std::vector<Channel *> channels = ep->poll();

        // 处理每个Channel的事件
        for (Channel *channel : channels)
            channel->handleEvent();
    }
}

void EventLoop::updateChannel(Channel *channel) const {
    ep->updateChannel(channel);
}

void EventLoop::deleteChannel(Channel *channel) const {
    ep->deleteChannel(channel);
}