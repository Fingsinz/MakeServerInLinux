#pragma once

#include "Common.h"
#include "sys/socket.h"
#include <string>

class Socket {
public:
    // 禁止拷贝和移动
    DISALLOW_COPY_AND_MOVE(Socket);

    Socket();
    ~Socket();

    /**
     * @brief 设置Socket的文件描述符
     *
     * @param fd 文件描述符
     */
    void setFd(int fd);

    /**
     * @brief 获取Socket的文件描述符
     *
     * @return int 文件描述符
     */
    int getFd() const;

    /**
     * @brief 获取Socket的地址
     *
     * @return std::string 地址
     */
    std::string getAddr() const;

    /**
     * @brief 创建一个Socket
     *
     * @return FLAG 标记
     */
    FLAG socketCreate();

    /**
     * @brief 绑定Socket到IP和端口
     *
     * @param ip 绑定ip
     * @param port 绑定端口
     * @return FLAG 标记
     */
    FLAG socketBind(const char *ip, uint16_t port) const;

    /**
     * @brief 监听Socket上的传入连接。
     *
     * @return FLAG 标记
     */
    FLAG socketListen() const;

    /**
     * @brief 接受Socket上的传入连接。
     *
     * @param clientFd 客户端文件描述符
     * @return FLAG 标记
     */
    FLAG socketAccept(int &clientFd) const;

    /**
     * @brief 将套接字连接到特定的IP地址和端口。
     *
     * @param ip IP地址
     * @param port 端口
     * @return FLAG 标记
     */
    FLAG socketConnect(const char *ip, uint16_t port) const;

    /**
     * @brief 设置Socket非阻塞
     *
     * @return FLAG 标记
     */
    FLAG setNonBlocking() const;

    /**
     * @brief 检查Socket是否非阻塞
     *
     * @return bool 是或否
     */
    bool isNonBlocking() const;

    /**
     * @brief 获取套接字的接收缓冲区大小
     *
     * @return size_t 缓冲区大小
     */
    size_t recvBufSize() const;

private:
    int mFd{-1}; // socket 文件描述符
};
