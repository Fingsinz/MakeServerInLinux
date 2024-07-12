#pragma once

#include "Common.h"

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif //  OS_LINUX

#include <vector>

class Epoll {
public:
    // 禁用拷贝和移动
    DISALLOW_COPY_AND_MOVE(Epoll);

    Epoll();
    ~Epoll();

    /**
     * @brief 更新Channel
     * @param channel 指定Channel
     * @return 操作标记FLAG
     */
    FLAG updateChannel(Channel *channel) const;

    /**
     * @brief 删除Channel
     * @param channel 指定Channel
     * @return 操作标记FLAG
     */
    FLAG deleteChannel(Channel *channel) const;

    /**
     * @brief poll操作
     * @param timeout 超时
     * @return 调用完epoll_wait的通道事件
     */
    std::vector<Channel *> poll(int timeout = -1) const;

private:
    int mEpFd{-1};                        // epoll文件描述符
    struct epoll_event *mEvents{nullptr}; // epoll事件
};
