#include "Epoll.h"
#include "Channel.h"
#include "Socket.h"
#include "util.h"
#include <cstring>
#include <unistd.h>

constexpr int maxEvents = 1024;

Epoll::Epoll() {
    mEpFd = epoll_create1(0);
    errorif(mEpFd == -1, "[Error]\tEpoll create error");
    mEvents = new epoll_event[maxEvents];
    bzero(mEvents, sizeof(epoll_event) * maxEvents);
}

Epoll::~Epoll() {
    if (mEpFd != -1)
        close(mEpFd);

    delete[] mEvents;
}

std::vector<Channel *> Epoll::poll(int timeout) const {
    std::vector<Channel *> activeEvents;
    int nfds = epoll_wait(mEpFd, mEvents, maxEvents, timeout);
    errorif(nfds == -1, "[Error]\tEpoll wait error");

    for (int i = 0; i < nfds; ++i) {
        Channel *channel = (Channel *)mEvents[i].data.ptr;
        int events = mEvents[i].events;

        channel->setReadyEvents(events);
        activeEvents.push_back(channel);
    }

    return activeEvents;
}

FLAG Epoll::updateChannel(Channel *channel) const {
    int fd = channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;

    if (!channel->isInEpoll()) {
        errorif(epoll_ctl(mEpFd, EPOLL_CTL_ADD, fd, &ev) == -1, "[Error]\tEpoll add error");
        channel->setInEpoll(true);
    } else
        errorif(epoll_ctl(mEpFd, EPOLL_CTL_MOD, fd, &ev) == -1, "[Error]\tEpoll update error");

    return FLAG::FL_SUCCESS;
}

FLAG Epoll::deleteChannel(Channel *channel) const {
    int fd = channel->getFd();
    errorif(epoll_ctl(mEpFd, EPOLL_CTL_DEL, fd, nullptr) == -1, "[Error]\tEpoll delete error");
    channel->setInEpoll(false);
    return FLAG::FL_SUCCESS;
}