#pragma once

#include "Common.h"
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

class Acceptor;
class EventLoop;
class Connection;
class Socket;
class ThreadPool;

class Server {
public:
    Server(std::string const &ip, int const port);
    ~Server();

    // 禁止拷贝和移动
    DISALLOW_COPY_AND_MOVE(Server);

    /**
     * @brief 启动服务器
     */
    void start();

    /**
     * @brief 设置连接回调函数
     * @param cb 回调函数，接受一个Connection指针作为参数，无返回值。
     */
    void setConnectionCallback(std::function<void(Connection *)> const &cb);

    /**
     * @brief 设置消息回调函数
     * @param cb 回调函数，接受一个Connection指针作为参数，无返回值。
     */
    void setMessageCallback(std::function<void(Connection *)> const &cb);

    /**
     * @brief 处理新的连接
     * @param fd 新连接的文件描述符。
     */
    void handleNewConnection(int fd);

    /**
     * @brief 关闭连接
     * @param fd 需要关闭的连接的文件描述符。
     */
    void handleClose(int fd);

private:
    std::unique_ptr<EventLoop> mainReactor;              // 只负责接受连接，然后分发给一个subReactor
    std::vector<std::unique_ptr<EventLoop>> subReactors; // 处理事件循环
    std::unique_ptr<Acceptor> acceptor;                  // 连接接受器
    int nextConnId;                                      // 下一个连接的ID
    std::unordered_map<int, Connection *> connections;   // 存储连接及其相应的文件描述符
    std::unique_ptr<ThreadPool> threadPool;              // 线程池

    std::function<void(Connection *)> onConnectCallback; // 连接的业务逻辑
    std::function<void(Connection *)> onMessageCallback; // 接收消息后的业务逻辑
};
