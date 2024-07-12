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

    void setSentBuf(char const *str);

    Buffer *getReadBuf();

    Buffer *getSentBuf();

    void handleMessage();

    void handleClose();

    State getState() const;

    EventLoop *getLoop() const;

    int getFd() const;

    int getId() const;

private:
    int mConnfd;
    int mConnId;

    State mState;

    EventLoop *mLoop;

    std::unique_ptr<Channel> mChannel;
    std::unique_ptr<Buffer> mReadBuf;
    std::unique_ptr<Buffer> mSendBuf;

    std::function<void(int)> onCloseCb;
    std::function<void(Connection *)> onMessageCb;

    void readNonBlocking();
    void writeNonBlocking();
};
