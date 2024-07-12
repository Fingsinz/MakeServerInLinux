#pragma once

#include "Common.h"
#include <functional>
#include <sys/epoll.h>

class Channel {
public:
    // 禁止拷贝和移动
    DISALLOW_COPY_AND_MOVE(Channel);
    explicit Channel(EventLoop *loop, int fd);
    ~Channel();

    /**
     * @brief 处理事件
     */
    void handleEvent() const;

    /**
     * @brief 允许读
     */
    void enableRead();

    /**
     * @brief 允许写
     */
    void enableWrite();

    /**
     * @brief 使用ET
     */
    void useET();

    /**
     * @brief 禁止写
     */
    void disableWrite();

    /**
     * @brief 获取Socket fd
     * @return 返回Socket fd
     */
    int getFd() const;

    /**
     * @brief 获取监听事件
     * @return 监听事件
     */
    uint32_t getListenEvents() const;

    /**
     * @brief 获取就绪事件
     * @return 就绪事件
     */
    uint32_t getReadyEvents() const;

    /**
     * @brief 设置就绪事件
     * @param 就绪事件
     */
    void setReadyEvents(uint32_t events);

    /**
     * @brief 判断当前Channel是否在Epoll中
     * @return 如果存在则为true，否则为false
     */
    bool isInEpoll() const;

    /**
     * @brief 设置当前Channel在Epoll中
     * @param in 如果存在为真，否则为假
     */
    void setInEpoll(bool in = true);

    /**
     * @brief 设置读回调函数。
     * @param _callback要设置的回调函数。
     */
    void setReadCallback(std::function<void()> const &callback);

    /**
     * @brief 设置写回调函数。
     * @param _callback要设置的回调函数。
     */
    void setWriteCallback(std::function<void()> const &callback);

private:
    int mFd;                             // 与之关联的Socket fd
    EventLoop *mLoop;                    // 指向与之关联的事件循环
    uint32_t mListenEvents{0};           // 监听的事件
    uint32_t mReadyEvents{0};            // 就绪事件
    bool inEpoll{false};                 // 指示该Channel是否存在有效
    std::function<void()> readCallback;  // 读回调
    std::function<void()> writeCallback; // 写回调
};
