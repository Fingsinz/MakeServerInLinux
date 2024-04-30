#pragma once

#include "Macros.h"
#include <functional>

class EventLoop;
class Socket;
class InetAddress;
class Channel;

class Acceptor
{
private:
	EventLoop *mLoop;			// 用于事件处理的EventLoop指针
	Socket *mSocket;			// 用于处理套接字操作的套接字指针
	Channel *mAcceptChannel;	// 用于接受连接的 Channel 指针

	// 定义一个新建连接的回调函数
	std::function<void(Socket *)> mNewConnectionCallback;

public:
	explicit Acceptor(EventLoop *loop);
	~Acceptor();

	// 禁用拷贝和移动
	DISALLOW_COPY_AND_MOVE(Acceptor);

	/**
	 * @brief 接受新连接
	 */
	void acceptConnection();

	/**
	 * @brief 设置新连接的回调函数。
	 *
	 * @param callback 为新连接设置的回调函数。
	 */
	void setNewConnectionCallback(std::function<void(Socket *)> const &callback);
};
