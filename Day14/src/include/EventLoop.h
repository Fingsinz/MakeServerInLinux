#pragma once

#include "Common.h"
#include <memory>

class Epoll;
class Channel;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    // 禁用拷贝和移动
    DISALLOW_COPY_AND_MOVE(EventLoop);

    /**
     * @brief 循环函数
     */
    void loop() const;

    /**
     * @brief 更新 Channel
     * @param channel 指向要更新的Channel对象的指针
     */
    void updateChannel(Channel *channel) const;

    /**
     * @brief 删除 Channel
     * @param channel 指向要删除的Channel对象的指针
     */
    void deleteChannel(Channel *channel) const;

private:
    std::unique_ptr<Epoll> ep; // 指向Epoll类实例的指针
};
