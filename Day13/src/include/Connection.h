#pragma once

#include "Macros.h"
#include <functional>

class Buffer;
class EventLoop;
class Socket;
class Channel;

class Connection
{
private:
	EventLoop *mLoop;		// EventLoop指针
	Socket *mSocket;		// Socket指针
	Channel *mChannel;		// Channel指针
	Buffer *mReadBuffer;	// 读缓冲区

	// 删除连接的回调函数
	std::function<void(int)> mDeleteConnectionCallback;

public:
	explicit Connection(EventLoop *loop, Socket *sock);
	~Connection();

	// 禁止拷贝和移动
	DISALLOW_COPY_AND_MOVE(Connection);

	/**
	 * @brief 回显sockfd发来的数据
	 *
	 * @param sockfd 通过套接字文件描述符发回数据
	 */
	void echo(int sockfd);

	/**
	 * @brief 设置删除连接时要调用的回调函数
	 *
	 * @param _callback 删除连接时要调用的回调函数
	 */
	void setDeleteConnectionCallback(std::function<void(int)> const &callback);

	/**
	 * @brief 通过指定的套接字发送数据。
	 *
	 * @param sockfd 套接字文件描述符
	 */
	void send(int sockfd);
};
