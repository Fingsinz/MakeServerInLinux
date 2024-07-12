#pragma once

#include "Common.h"
#include <functional>
#include <memory>

class Buffer;
class EventLoop;
class Socket;
class Channel;

class Connection {
public:
    enum State {
        Invalid = 0, // 初始无效状态
        Connected,   // 连接建立
        Disconnected
    };

public:
    // 禁止拷贝和移动
    DISALLOW_COPY_AND_MOVE(Connection);
    explicit Connection(EventLoop *loop, int fd, int id);
    ~Connection();

    /**
     * @brief 读操作
     * @return FLAG 操作结果标记
     */
    FLAG read();

    /**
     * @brief 写操作
     * @return FLAG 操作结果标记
     */
    FLAG write();

    /**
     * @brief 通过连接发送消息
     * @param msg 要发送的消息
     * @return FLAG 操作结果标记
     */
    FLAG send(std::string msg);

    FLAG sent(char const *msg);

    /**
     * @brief 设置一个回调函数，在连接上接收到消息时调用
     * @param callback 回调函数
     */
    void setOnMessageCallback(std::function<void(Connection *)> const &callback);

    /**
     * @brief 设置一个回调函数，在需要断开连接时调用
     * @param callback 回调函数
     */
    void setOnCloseCallback(std::function<void(int)> const &callback);

    /**
     * @brief 设置发送缓冲区
     * @param str 指向要设置为发送缓冲区的字符串的常量指针。
     */
    void setSentBuf(char const *str);

    /**
     * @brief 获取读取缓冲区的指针
     * @return Buffer* 返回读取缓冲区的指针。
     */
    Buffer *getReadBuf();

    /**
     * @brief 获取发送缓冲区的指针
     * @return Buffer* 返回发送缓冲区的指针。
     */
    Buffer *getSentBuf();

    /**
     * @brief 处理接收到的消息
     */
    void handleMessage();

    /**
     * @brief 处理连接关闭事件
     */
    void handleClose();

    /**
     * @brief 获取当前状态
     * @return State 返回当前状态。
     */
    State getState() const;

    /**
     * @brief 获取事件循环的指针
     * @return EventLoop* 返回事件循环的指针。
     */
    EventLoop *getLoop() const;

    /**
     * @brief 获取文件描述符
     * @return int 返回文件描述符。
     */
    int getFd() const;

    /**
     * @brief 获取唯一标识符
     * @return int 返回唯一标识符。
     */
    int getId() const;

private:
    int mConnfd;      // 连接的文件描述符
    int mConnId;      // 连接的唯一标识符
    State mState;     // 当前连接的状态
    EventLoop *mLoop; // 所属的事件循环

    std::unique_ptr<Channel> mChannel; // 用于监听连接上的事件
    std::unique_ptr<Buffer> mReadBuf;  // 用于存储读取的数据
    std::unique_ptr<Buffer> mSendBuf;  // 用于存储待发送的数据

    std::function<void(int)> onCloseCb;            // 连接关闭时的回调函数
    std::function<void(Connection *)> onMessageCb; // 接收到消息时的回调函数

    /**
     * @brief 从连接中非阻塞地读取数据，并将数据存储到读取缓冲区中。
     */
    void readNonBlocking();

    /**
     * @brief 尝试非阻塞地将发送缓冲区中的数据写入到连接中。
     */
    void writeNonBlocking();
};
