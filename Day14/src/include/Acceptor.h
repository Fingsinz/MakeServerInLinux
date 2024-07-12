#pragma once

#include "Common.h"
#include <functional>
#include <memory>

class Acceptor {
public:
    // 禁用拷贝和移动
    DISALLOW_COPY_AND_MOVE(Acceptor);

    explicit Acceptor(EventLoop *loop, std::string const &ip, int const port);
    ~Acceptor();

    /**
     * @brief 接受连接
     * @return 连接的情况
     */
    FLAG acceptConnection() const;

    /**
     * @brief 设置新连接的回调函数。
     * @param callback 为新连接设置的回调函数。
     */
    void setNewConnectionCallback(std::function<void(int)> const &callback);

    /**
     * @brief 创建Socket
     */
    void socketCreate();

    /**
     * @brief 绑定ip和端口到socket
     * @param ip Ip地址
     * @param port 端口
     */
    void socketBind(std::string const &ip, int const port);

    /**
     * @brief 监听socket
     */
    void socketListen();

private:
    EventLoop *mLoop;
    int mListenFd;
    std::unique_ptr<Channel> mChannel;

    // 定义一个新建连接的回调函数
    std::function<void(int)> mNewConnectionCallback;
};
