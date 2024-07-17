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
    Server();
    ~Server();

    // 禁止拷贝和移动
    DISALLOW_COPY_AND_MOVE(Server);

    /**
     * @brief 启动服务器
     */
    void start();

    /**
     * @brief 设置新连接建立时的回调
     * @param fn 回调函数
     */
    void onConnect(std::function<void(Connection *)> fn);

    /**
     * @brief 设置接收消息时的回调
     * @param fn 回调函数
     */
    void onMessage(std::function<void(Connection *)> fn);

    /**
     * @brief 处理与给定文件描述符的新连接
     * @param fd 新连接的文件描述符
     * @return Flag 表示操作结果
     */
    FLAG newConnection(int fd);

    /**
     * @brief 删除与给定文件描述符的连接
     * @param fd 要删除的连接的文件描述符
     * @return Flag 表示操作结果
     */
    FLAG deleteConnection(int fd);

private:
    std::unique_ptr<EventLoop> mainReactor;              // 只负责接受连接，然后分发给一个subReactor
    std::vector<std::unique_ptr<EventLoop>> subReactors; // 处理事件循环
    std::unique_ptr<Acceptor> acceptor;                  // 连接接受器
    std::unordered_map<int, std::unique_ptr<Connection>> connections; // 存储连接及其相应的文件描述符
    std::unique_ptr<ThreadPool> threadPool;                           // 线程池

    std::function<void(Connection *)> onConnectionCallback; // 连接的业务逻辑
    std::function<void(Connection *)> onMessageCallback;    // 接收消息后的业务逻辑
};
